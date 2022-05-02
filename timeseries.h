/*
*timeseries.h
*
* Author: 208968032 Gilad Shimoni
* Author: 324234756 Marom Cohen
*/
#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;


struct column{
    string name;
    vector<float> values;
};



class TimeSeries{
    vector<column> data;

public:
    TimeSeries();
    TimeSeries(const char* CSVfileName);
    vector<float> getColumn(const string& columnName);
    vector<float> getColumn(int i);
//    vector<string> getNamesByValue(float i);
//    string getNameByValue(float i);
    string getName(int i);
    bool doesExist(column col, float num);
    float getElement(int cellRow, int cellColumn);
    float getElement(string vectorName, int cellColumn);
    void printData();
    int dataSize();
};



#endif /* TIMESERIES_H_ */
