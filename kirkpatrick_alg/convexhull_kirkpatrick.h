#ifndef LAB03_OOP_CONVEXHULL_KIRPATRICK_H
#define LAB03_OOP_CONVEXHULL_KIRPATRICK_H


#include "Point_kirkpatrick.h"
#include <vector>

using namespace std;
class ConvexHull {
public:
    void printHull(Point points[], int n);

private:
    int orientation(Point p, Point q, Point r);
    vector<Point> convexHull(Point points[], int n);
};

#endif //LAB03_OOP_CONVEXHULL_KIRPATRICK_H
