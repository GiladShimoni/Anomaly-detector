/*
* HybridAnomalyDetector.cpp
*
* Author: 208968032 Gilad Shimoni
* Author: 324234756 Marom Cohen
*/



#include "HybridAnomalyDetector.h"


/**
 * Auto-generated constructor
 */
HybridAnomalyDetector::HybridAnomalyDetector() {}


/**
 * Auto-generated destructor stub
 */
HybridAnomalyDetector::~HybridAnomalyDetector() {}


/**
 * if a correalaition is detected, add CF to list, else leave.
 * @param ts timeseries
 * @param i feature1 index in ts
 * @param Colartive_index  i feature2 index in ts
 * @param correlation float
 */
void HybridAnomalyDetector::addCf(TimeSeries ts, int i, int Colartive_index, float correlation){
    //if corallition is smaller thar correlation th
    if(correlation < CIRCLE_CORELATION){
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

    if (correlation > coralition_th){
        // going over the vector of the two features that had a correlation and creating points from their values
        for (int j = 0; j < size; j++){
            float x = ts.getElement(i,j);
            float y = ts.getElement(Colartive_index,j);
            Point p(x,y);
            float cDev = dev(p,newCf.lin_reg)*DEV_MULTIPLIER;
            if (cDev > newCf.threshold) {newCf.threshold = cDev; }
        }

    } else {
        Point* points[size];
        for(int j = 0; j < size; j++){
            points[j] = new Point(holdX[j], holdY[j]);
        }
        Circle circle = findMinCircle(points,size);
        newCf.p.x = circle.center.x;
        newCf.p.y = circle.center.y;
        newCf.threshold = circle.radius * DEV_MULTIPLIER;
    }
    cf.push_back(newCf);
}


/**
 * method will get point p and correlatedFeatures and return if there is an anomaly
 * @param p Point
 * @param currCf correlatedFeatures
 * @return bool if p is an anomaly
 */
bool HybridAnomalyDetector::anomalyDetected(Point p, correlatedFeatures currCf){
    float check = 0 ;
    //if coralition is of liner corelation threshhold
    if(currCf.corrlation > coralition_th){
        check = dev(p, currCf.lin_reg);
    } else {
        //else the correlation is of circle
        check = distance(currCf.p, p);
    }
    return check > currCf.threshold;
}

