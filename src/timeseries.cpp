/*
*timeseries.cpp
*
* Author: 208968032 Gilad Shimoni
* Author: 324234756 Marom Cohen
*/
#include <iostream>
#include "timeseries.h"

/**
 * This is a default constructor, that initializes instances when no data is entered.
 */
TimeSeries::TimeSeries(){
    this->data = std::vector<column>{};
}

/**
 * This is the second constructor.
 * @param CSVfileName the file from which we read the data.
 */
TimeSeries::TimeSeries(const char *CSVfileName) {
    vector<string> stringVec;
    vector<float> vec;
    string line, head,value;
    fstream fin;
    fin.open((string)CSVfileName, ios::in);
    getline(fin, line);
    stringstream headers(line);
    // going over the file's first line (for getting the features names)
    while (getline(headers,head,',')){
        column* c = new column();
        c->name = head;
        c->values = std::vector<float>{};
        data.push_back(*c);
    }
    // going over the rest of the file's lines
    while(getline(fin,line)){
        stringstream values(line);
        int i = 0;
        // dividing the data in the line according to commas.
        while (getline(values,value,',')){
            data[i].values.push_back(std::stof(value));
            i++;
        }
    }
}

/**
 * This method prints all the data that appears in our data vector.
 */
void TimeSeries::printData() {
    int i = 0;
    // going over the data vector and printing it
    while(i < data.size()){
        cout<<data[i].name;
        cout<<":::";
        int j = 0;
        while (j<data[i].values.size()){
            cout<<data[i].values[j];
            j++;
        }
        cout<<"\n";
        i++;
    }
}

int TimeSeries::dataSize() {
    return data.size();
}


/**
 * This method returns all the values of a certain feature (according to the features name)
 * @param columnName the name of the feature we check for all of it's values.
 * @return a vector of all the feature's values
 */
vector<float> TimeSeries::getColumn(const string& columnName) {
    // going over the features names stored in data.
    for (int i = 0; i < data.size(); i++) {
        // checking if the current name is the feature we're searching for
        if (data[i].name == columnName) {
            return data[i].values;
        }
    }
    // if we didn't find what we were looking for (return an empty vector)
    vector<float> empty = {};
    return empty;
}

/**
 * This method returns a features values vector (according to the features placement in the data vector)
 * @param i the place of the feature we want it's values
 * @return a vector of values of the requested feature.
 */
vector<float> TimeSeries::getColumn(int i) {
    return data[i].values;
}


/**
 * This is an auxiliary method which checks if a certain value appears in a features values vector.
 * @param col the vector of the searched feature
 * @param num the number we search for in the features data
 * @return true (if the number is one of the features values) or false
 */
bool TimeSeries::doesExist(column col, float num) {
    // going over all the values in the current feature
    for (int i = 0; i < col.values.size(); i++) {
        // checking if the current value is the searched num
        if (col.values[i] == num) {
            return true;
        }
    }
    return false;
}

/**
 * This method returns the name of the feature that appears in the given placement in the data vector
 * @param i the place from which we want to get the feature's name the lies there
 * @return the feature's name in the wanted location
 */
string TimeSeries::getName(int i) {
    return data[i].name;
}

/**
 * this method returns the element that appears in the given place (according to row and column)
 * @param cellRow the row from which we want the element
 * @param cellColumn the column from which we want the element
 * @return the element in the searched place
 */
float TimeSeries::getElement(int cellRow, int cellColumn) {
    return data[cellRow].values[cellColumn];
}

/**
 * This method returns an element in the searched place (according to the feature's name and the column of the element)
 * @param vectorName
 * @param cellColumn
 * @return
 */
float TimeSeries::getElement(string vectorName, int cellColumn) {
    vector<float> value = getColumn(vectorName);
    return value[cellColumn];
}
