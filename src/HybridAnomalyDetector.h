/*
* SimpleAnomalyDetector.h
*
* Author: 208968032 Gilad Shimoni
* Author: 324234756 Marom Cohen
*/


#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

#define CIRCLE_CORELATION 0.5

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
    virtual void addCf(TimeSeries ts, int i, int Colartive_index, float correltion);
    virtual bool anomalyDetected(Point p, correlatedFeatures currCf);



};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
