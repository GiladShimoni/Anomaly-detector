/*
* SimpleAnomalyDetector.cpp
*
* Author: 208968032 Gilad Shimoni
* Author: 324234756 Marom Cohen
*/


#include "SimpleAnomalyDetector.h"
#include <vector>
#include <iostream>
#include "anomaly_detection_util.h"
using namespace std;

/**
 * This is a default constructor method that builds the instances of the class
 */
SimpleAnomalyDetector::SimpleAnomalyDetector() {
    this->cf = std::vector<correlatedFeatures>{};
    this->coralition_th = CORALITION_TH;

}

/**
 * THis is the second constructor of the class
 * @param cf the vector of correlatedFeatures we insert into the class cf field
 */
SimpleAnomalyDetector::SimpleAnomalyDetector(vector<correlatedFeatures> cf) {
    this->cf = cf;
    this-> coralition_th = CORALITION_TH;
}

/**
 * This method sets the value of the threshold
 * @param th the value given to be set for the threshold
 */
void SimpleAnomalyDetector::setThreshold(float th) {
    this->coralition_th = th;
}

/**
 * This is a destructor method to destroy instances of the class
 */
SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    cf.clear();
}

/**
 * This method sets the value of the "normal" correlation between two points.
 * the method gets a TimeSeries, and checks the correlation between every two points.
 * @param ts the instance that holds all the values we use to calculate the correlation.
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    TimeSeries ts_cpy =  ts;
    // going over each element column vector of the data table (with all the features values)
    for (int i = 0; i < ts_cpy.dataSize(); i++){
        float maxPearson = 0;
        int Colartive_index = -1;
        // comparing each element i and comparing it the rest of the values of the rest of the features
        for(int j =i+1 ; j<ts_cpy.dataSize(); j++){
            float p = fabs(pearson(ts_cpy.getColumn(i).data(), ts_cpy.getColumn(j).data(), ts_cpy.getColumn(i).size()));
            // checking if the current correlation is bigger the max one
            if(p > maxPearson) {
                maxPearson = p;
                Colartive_index = j;
            }
        }

        addCf(ts_cpy, i , Colartive_index , maxPearson);
    }
}



/**
 * if a correalaition is detected, add CF to list, else leave.
 * @param ts timeseries
 * @param i feature1 index in ts
 * @param Colartive_index  i feature2 index in ts
 * @param correlation float
 */
void SimpleAnomalyDetector::addCf(TimeSeries ts, int i, int Colartive_index, float correlation){
    //if corallition is smaller thar correlation th
    if(correlation < coralition_th){
        return;
    }
    struct correlatedFeatures newCf;
    // initializing the new instance
    newCf.feature1 = ts.getName(i);
    newCf.feature2 = ts.getName(Colartive_index);
    newCf.threshold = 0;
    newCf.corrlation = correlation;
    vector<float> holdX = ts.getColumn(i);
    vector<float> holdY = ts.getColumn(Colartive_index);
    float* Y = holdY.data();
    float* X = holdX.data();
    newCf.lin_reg = linear_reg(X,Y,ts.getColumn(i).size());
    int size = min(ts.getColumn(newCf.feature1).size(),ts.getColumn(newCf.feature2).size());
    // going over the vector of the two features that had a correlation and creating points from their values
    for (int j = 0; j < size; j++){
        float x = ts.getElement(i,j);
        float y = ts.getElement(Colartive_index,j);
        Point p(x,y);
        float cDev = dev(p,newCf.lin_reg)*DEV_MULTIPLIER;
        if (cDev > newCf.threshold) {newCf.threshold = cDev; }
        }
    cf.push_back(newCf);
}


/**
 * method will get point p and correlatedFeatures and return if there is an anomaly
 * @param p Point
 * @param currCf correlatedFeatures
 * @return bool if p is an anomaly
 */
 bool SimpleAnomalyDetector::anomalyDetected(Point p, correlatedFeatures currCf){
    float currentDev = dev(p, currCf.lin_reg);
    if(currentDev > currCf.threshold){
        return true;
    }
     return false;
}


/**
 * This method gets a new TimeSeries element and checks if the correlations in the instance is normal according to the
 * correlation line set by the original data.
 * @param ts the new TimeSeries that holds new data we check for correlation
 * @return the vector of AnomalyReport instances which have unormal correlations (correlation bigger than set to be)
 */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    vector<AnomalyReport> ar;
    TimeSeries ts_cpy =  ts;
    // going over all the correlation couples saved in cf
    for(int i = 0; i< cf.size(); i++){
        string description = cf[i].feature1 +"-"+ cf[i].feature2;
        int size = ts_cpy.getColumn(cf[i].feature1).size();
        vector<float> xVec = ts_cpy.getColumn(cf[i].feature1);
        vector<float> yVec = ts_cpy.getColumn(cf[i].feature2);
        float* X = &xVec[0];
        float* Y = &yVec[0];
        Line line = linear_reg(X,Y,size);
        for(int j = 0; j < size; j++){
            float x = X[j];
            float y = Y[j];
            Point p(x,y);
            // checking if the current dev is bigger than the one that already exists the current element
            anomalyDetected(p, cf[i]);
            if( anomalyDetected(p, cf[i])){
                AnomalyReport currentAR(description , j+1);
                ar.push_back(currentAR);
            }
        }
    }
    return ar;
}

/**
 * This method prints the data table
 */
void SimpleAnomalyDetector::printAD() {
    cout<<cf.data()->feature1;
}

