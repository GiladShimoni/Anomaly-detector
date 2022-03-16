/*
* minCircle.cpp
*
* Author: 208968032 Gilad Shimoni
* Author: 324234756 Marom Cohen
*/
#include <algorithm>
#include <math.h>
#include <vector>
#include "minCircle.h"
using namespace std;

// Defining infinity
const float INF = 1e18;

/**
 * This method calculates the distance between two points
 * @param point1 first point
 * @param point2 second point
 * @return the distance between the two points
 */
float dist(const Point& point1, const Point& point2)
{
    return sqrt(pow(point1.x - point2.x, 2)
                + pow(point1.y - point2.y, 2));
}

/**
 * This method checks if the given point appears inside the circle
 * @param circle the circle we check to see if the point is inside
 * @param point the point we check to see if is inside the circle
 * @return true (if the point is inside) or false (if the point isnt inside the circle)
 */
bool is_inside(const Circle& circle, const Point& point)
{
    return dist(circle.center, point) <= circle.radius;
}

/**
 * This is an auxiliary method used to find the center of the circle using two points
 * @param point1X first point x coordinate
 * @param point1Y first point y coordinate
 * @param point2X second point x coordinate
 * @param point2Y second point y coordinate
 * @return the center point calculated
 */
Point get_circle_center(float point1X, float point1Y,
                        float point2X, float point2Y)
{
    float eq1 = point1X * point1X + point1Y * point1Y;
    float eq2 = point2X * point2X + point2Y * point2Y;
    float eq3 = point1X * point2Y - point1Y * point2X;
    return { (point2Y * eq1 - point1Y * eq2) / (2 * eq3),
             (point1X * eq2 - point2X * eq1) / (2 * eq3) };
}

/**
 * This method creates a circle using 3 points
 * @param point1 first point
 * @param point2 second point
 * @param point3 third point
 * @return the circle created from the 3 points
 */
Circle createCircle3(const Point& point1, const Point& point2,
                   const Point& point3)
{
    Point center = get_circle_center(point2.x - point1.x, point2.y - point1.y, point3.x - point1.x,
                                point3.y - point1.y);

    center.x += point1.x;
    center.y += point1.y;
    return {center, dist(center, point1)};
}

/**
 * This method creates a circle using 2 points
 * @param point1 first point
 * @param point2 second point
 * @return the circle created from the 2 points
 */
Circle createCircle2(const Point& point1, const Point& point2)
{
    // Set the center to be the midpoint of A and B
    Point center = { (point1.x + point2.x) / 2, (point1.y + point2.y) / 2 };

    // Set the radius to be half the distance AB
    return { center, dist(point1, point2) / 2 };
}

/**
 * This method checks if the given circle contains all the points
 * @param circle the circle we check to see if contains all points
 * @param points the vector that holds all the points
 * @return true (if the circle contains all the points) or false (if the circle dosent contain all the points)
 */
bool containsAll(const Circle& circle,
                     const vector<Point>& points)
{

    // Iterating through all the points
    // to check whether the points
    // lie inside the circle or not
    for (const Point& point : points)
        if (!is_inside(circle, point))
            return false;
    return true;
}

/**
 * This method finds the minimum circle in the trivial conditions (there are no points, or there are 2 / 3 points)
 * @param points the vector of points
 * @return a circle created from the points
 */
Circle trivialCircle(vector<Point>& points)
{
    if (points.empty()) {
        return { { 0, 0 }, 0 };
    }
    else if (points.size() == 1) {
        return { points[0], 0 };
    }
    else if (points.size() == 2) {
        return createCircle2(points[0], points[1]);
    }

    // checking to see if the minimum circle can be determined by only two points
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {

            Circle circle = createCircle2(points[i], points[j]);
            if (containsAll(circle, points))
                return circle;
        }
    }
    return createCircle3(points[0], points[1], points[2]);
}

/**
 * This is an auxiliary method to the main "findMinCircle" method. this method helps calculate the circle from the
 * given points
 * @param points vector of points
 * @param save the vector in which we save all the points that appear outside the current calculated circle
 * @param n number of points we haven't gone over yet
 * @return the minimum circle calculated
 */
Circle minCircleHelper(vector<Point>& points,
                    vector<Point> save, int n)
{
    // Base case when all points processed or |R| = 3
    if (n == 0 || save.size() == 3) {
        return trivialCircle(save);
    }

    // Pick a random point randomly
    int idx = rand() % n;
    Point p = points[idx];

    // Put the picked point at the end of P
    // since it's more efficient than
    // deleting from the middle of the vector
    swap(points[idx], points[n - 1]);

    // Get the MEC circle d from the
    // set of points P - {p}
    Circle d = minCircleHelper(points, save, n - 1);

    // If d contains p, return d
    if (is_inside(d, p)) {
        return d;
    }

    // Otherwise, must be on the boundary of the MEC
    save.push_back(p);

    // Return the MEC for P - {p} and save U {p}
    return minCircleHelper(points, save, n - 1);
}

/**
 * This method returnes a minimum circle (using auxiliary methods) the contains all the points
 * @param points an array of points
 * @param size the size of the array
 * @return a minimum circle that contains all the points
 */
Circle findMinCircle(Point** points,size_t size) {
    vector<Point> dataVector;
    dataVector.reserve(size);
    for (int i = 0; i < size; i++) {
        dataVector.push_back(*points[i]);
    }
    return minCircleHelper(dataVector, {}, dataVector.size());
}

