/*
* commands.h
*
* Author: 208968032 Gilad Shimoni
* Author: 324234756 Marom Cohen
*/
#ifndef COMMANDS_H_
#define COMMANDS_H_


#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include "timeseries.h"

using namespace std;


class DefaultIO{
public:
    virtual string read()=0;
    virtual void write(string text)=0;
    virtual void write(float f)=0;
    virtual void read(float* f)=0;
    virtual ~DefaultIO(){}

    // you may add additional methods here
};



/***
 * class cliRan
 * will hold all data and
 * methods that have to do
 * with a cliRun instance.
 */
class CliRun{
public:
    TimeSeries train;
    TimeSeries test;
    float correlation_th= 0.9;
    vector<AnomalyReport> ar;
    bool keepAlive = true;

    CliRun(){
        this->train = TimeSeries();
        this->test = TimeSeries();
    }


    /***
     * uploadCsvlines
     * will get lines through dio and write to locoalFile
     * @param prompt string write to user
     * @param fileName local filename to write to
     * @param dio I/O
     */
    virtual void uploadCsvLines(string prompt, string fileName, DefaultIO* dio){
        dio->write(prompt);
        string line = dio->read();
        std::ofstream file(fileName);
        while (line != "done"){
            file<<line<<endl;
            line = dio->read();
        }
        dio->write("Upload complete.\n");
    }
};




// you may edit this class
class Command{
protected:
    string description;
	DefaultIO* dio{};
    CliRun* cliRun{};
public:
	Command(DefaultIO* dio, CliRun* CliRun1):dio(dio),cliRun(CliRun1){}
	virtual void execute()=0;
    virtual string getDescription(){ return description;};
	virtual ~Command(){}

};

/***
 * Enter ts,
 * to enter time series to server
 */
class EnterTs:public Command{
public:
    EnterTs(DefaultIO* dio, CliRun* CliRun1): Command(dio, CliRun1){
        description = "upload a time series csv file";
    }
    void execute() override{
        string prompt = "Please upload your local train CSV file.\n";
        cliRun->uploadCsvLines(prompt, "anomalyTrain.csv", dio);
        prompt = "Please upload your local test CSV file.\n";
        cliRun->uploadCsvLines(prompt, "anomalyTest.csv", dio);
        cliRun->train = TimeSeries("anomalyTrain.csv");
        cliRun->test = TimeSeries("anomalyTest.csv");
    }
};

/***
 * Settings
 * to adjust detector setting
 */
class Settings: public Command{
public:
    Settings(DefaultIO* dio, CliRun* CliRun1): Command(dio, CliRun1){
        description = "algorithm settings";
    }
    void execute() override {
        dio->write("The current correlation threshold is " + to_string(cliRun->correlation_th) + "\n");
        dio->write("Type a new threshold\n");
        while (true) {
            float inp;
            dio->read(&inp);
            if (inp < 1 && inp > 0) {
                cliRun->correlation_th = inp;
                return;
            }
            dio->write("please choose a value between 0 and 1.\n");
        }
    }
};

/***
 * DetectAnomalies
 * detect anomaly
 */
class DetectAnomalies: public Command{
public:
    DetectAnomalies(DefaultIO* dio, CliRun* CliRun1): Command(dio, CliRun1){
        description = "detect anomalies";
    }
    void execute() override{
        HybridAnomalyDetector anomalyDetector;
        anomalyDetector.setThreshold(cliRun->correlation_th);
        anomalyDetector.learnNormal(cliRun->train);
        cliRun->ar =  anomalyDetector.detect(cliRun->test);
        dio->write("anomaly detection complete.\n");
    }
};

/***
 * DisplayResults
 * display results of ar vector
 */
class DisplayResults: public Command{
public:
    DisplayResults(DefaultIO* dio, CliRun* CliRun1): Command(dio, CliRun1){
        description = "display results";
    }
    void execute() override{
        for(AnomalyReport report: cliRun->ar){
            dio->write(to_string(report.timeStep) +"   "+ report.description+"\n");
        }
        dio->write("Done.\n");
    }
};


/***
 * compare results
 * compare results to user anomaly timesteps
 */
class CompareResults: public Command{
    vector<Point> myStamps;

public:
    CompareResults(DefaultIO* dio, CliRun* CliRun1): Command(dio, CliRun1){
        description = "upload anomalies and analyze results";
    }

    /***
     * calculate detector timeStamps
     */
    void callcTimestams(){
        int x = cliRun->ar[0].timeStep;
        int y = cliRun->ar[0].timeStep;
        for(int i = 1; i < cliRun->ar.size(); i++){
            if(cliRun->ar[i].timeStep == cliRun->ar[i-1].timeStep + 1 &&
            cliRun->ar[i].description == cliRun->ar[i-1].description){
                y++;
                continue;
            } else{
                myStamps.push_back(Point(float(x), float(y)));
                x = cliRun->ar[i].timeStep;
                y = cliRun->ar[i].timeStep;
            }
        }
        myStamps.push_back(Point(float(x), float(y)));
    }



    void execute() override{
        callcTimestams();
        vector<Point> inpStamps;
        dio->write("Please upload your local anomalies file.\n");
        //insert timestamps from dio
        string line = dio->read();
        while(line != "done") {
            string x = line.substr(0, line.find(','));
            string y = line.substr(line.find(',')+1, line.size());
            inpStamps.emplace_back(stof(x), stof(y));
            line = dio->read();
        }
        dio->write("Upload complete.\n");

        //calc tp Rate and fp rate
        float p = inpStamps.size();
        float n = cliRun->test.getColumn(0).size();
        for(Point point: inpStamps){
            n = n - (point.y - point.x);
        }
        float fp = 0;
        float tp = 0;
        for (Point p1: myStamps){
            bool isPositve = false;
            for(Point p2: inpStamps){
                if(int(p1.y) < int(p2.x) || int(p2.y) < int(p1.x)) {
                    continue;
                } else{
                    isPositve = true;
                    break;
                }
            }
            if(isPositve){
                tp++;
            } else{
                fp++;
            }
        }
        float tpRate = ((int)(1000.0*tp/p))/1000.0f;
        float faRate = ((int)(1000.0*fp/n))/1000.0f;

        //print answers
        dio->write("True Positive Rate: ");
        dio->write(tpRate);
        dio->write("\nFalse Positive Rate: ");
        dio->write(faRate);
        dio->write("\n");
        myStamps.clear();
        inpStamps.clear();
    }


};

//6 exit
/***
 * will exit cliRun
 */
class Exit: public Command{
public:
    Exit(DefaultIO* dio, CliRun* CliRun1): Command(dio, CliRun1){
        description = "exit";
    }

    void execute() override{
        cliRun->keepAlive = false;
    }
};



#endif /* COMMANDS_H_ */
