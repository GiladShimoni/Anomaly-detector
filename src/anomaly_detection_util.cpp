/*
* animaly_detection_util.cpp
*
* Author: 208968032 Gilad Shimoni
* Author: 324234756 Marom Cohen
*/

#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>

using namespace std;





/**
 * This method calculates the sum of all the values in the array.
 * @param array the array that we calculates all it's elemnts sum
 * @param size the size of the array
 * @return the sum of all of the array's elements
 */
float sum(float* array, int size){
    float sum = 0;
    // going over the array
    for (int i = 0 ; i < size ; i++) { sum = sum + array[i]; }
    return sum;
}

/**
 * This method calculates the average of all of the array's values
 * @param array the array of values we use for calculation
 * @param size the size of the array
 * @return the average of all the array's elements
 */
float avg(float* array , int size){
    // checking if the array is not empty
    if(size > 0){ return sum(array,size) / (float) size ; }
    if (size == 0) { return 0; }
    else throw std::invalid_argument("size of array is negative.");
}


/**
 * This method returns the variance of X and Y.
 * @param x a dynamic array of float values
 * @param size the size of the array
 * @return variance of X and Y.
 */
float var(float* x, int size){
    if( size <= 0){ throw std::invalid_argument("size is not positive"); }
    float var = 0;
    float xAvg = avg(x, size);
    for (int i = 0; i < size ; i++ ) {var = var + pow(x[i] - xAvg, 2);}
    return var / ( float )size;
}

/**
 * this method returns the covariance of X and Y
 * @param x first array of float elements
 * @param y second array of float elements
 * @param size size of the arrays
 * @return covariance of X and Y
 */
float cov(float* x, float* y, int size){
    float c[size];
    float avgX = avg(x,size);
    float avgY = avg(y, size);
    // going over the elements of the arrays and calculating the covariance
    for (int i = 0; i < size ; ++i) {
        c[i] = (x[i] - avgX)*(y[i] - avgY);
    }
    return avg(c,size);
}

/**
 * this method returns the Pearson correlation coefficient of X and Y
 * @param x first array of float elements
 * @param y second array of float elements
 * @param size size of the arrays
 * @return Pearson correlation coefficient of X and Y
 */
float pearson(float* x, float* y, int size){
    return cov(x, y, size) / (sqrt(var(x,size)) * sqrt(var(y,size)));
}

class Line{
public:
    float a,b;
    Line():a(0),b(0){}
    Line(float a, float b):a(a),b(b){}
    float f(float x){
        return a*x+b;
    }
};

class Point{
public:
    float x,y;
    Point(float x, float y):x(x),y(x){}
};

/**
 * This method  performs a linear regression and return s the line equation
 * @param points a two dimensional array of the points (with which we calculate the line).
 * @param size size of the arrays
 * @return linear regression and return s the line equation
 */
Line linear_reg(Point** points, int size){
    float X[size];
    float Y[size];
    for(int i = 0 ; i<size; i++) {
        X[i] = points[i]->x;
        Y[i] = points[i]->y;
    }
    float a = cov(X,Y,size)/ var(X,size);
    float b = avg(Y,size) - a*avg(X,size);
    Line line(a,b);
    return line;
}

/**
 * This method performs a linear regression and returns the line equation
 * @param X first array of float elements (which we use to calculate the line)
 * @param Y second array of float elements (which we use to calculate the line)
 * @param size size of the arrays
 * @return linear regression line equation
 */
Line linear_reg(float* X, float* Y , int size){
    float a = cov(X,Y,size)/ var(X,size);
    float b = avg(Y,size) - a*avg(X,size);
    Line line(a,b);
    return line;
}
/**
 * This method returns the deviation between point p and the line
 * @param p the point which we use for calculation
 * @param l the line which we use for calculation
 * @return deviation between point p and the line
 */
float dev(Point p,Line l){
    float y = p.x*l.a + l.b;
    return fabs(y - p.y);
}


/**
 * This method returns the deviation between point p and the line equation of the points
 * @param p the point according to which we calculate the deviation
 * @param points the array of points which we use for the line equation calculation
 * @param size  the size of the Points array
 * @return deviation between point p and the line equation of the points
 */
float dev(Point p,Point** points, int size){return dev(p, linear_reg(points,size)); }

/**
 * This method turns the vector given into an array
 * @param vec the vector we want to turn to an array
 * @return an array (of the vector's elements)
 */
float* vectorToArray(vector<float> vec){
    return vec.data();
}


/**
 * This method calculates the distance between two points
 * @param p1 first point
 * @param p2 second point
 * @return the distance between the two points
 */
float distance(const Point& p1, const Point& p2){
    float x_dist = pow(p2.x - p1.x, 2);
    float y_dist = pow(p2.y - p1.y, 2);
    return sqrt(x_dist+y_dist);
}
