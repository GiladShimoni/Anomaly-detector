/*
* SimpleAnomalyDetector.h
*
* Author: 208968032 Gilad Shimoni
* Author: 324234756 Marom Cohen
*/

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

#define DEV_MULTIPLIER 1.1
#define CORALITION_TH 0.9

using namespace std;

struct correlatedFeatures{
    string feature1,feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    Point p = Point(0,0);
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
public:
    vector<correlatedFeatures> cf;
    float coralition_th;
    SimpleAnomalyDetector();
    SimpleAnomalyDetector(vector<correlatedFeatures> cf);
    virtual ~SimpleAnomalyDetector();
    virtual void addCf(TimeSeries ts, int i, int Colartive_index, float correltion);
    virtual void setThreshold(float th);
    virtual bool anomalyDetected(Point p, correlatedFeatures currCf);
    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    vector<correlatedFeatures> getNormalModel(){
        return cf;
    }
    void printAD();
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
