#ifndef LAB03_OOP_CONVEXHULL_KIRPATRICK_H
#define LAB03_OOP_CONVEXHULL_KIRPATRICK_H


#include "Point_kirkpatrick.h"
#include <vector>

using namespace std;
class ConvexHullKirkpatrick {
public:
    vector<Point_for_kir> printHull(Point_for_kir points[], int n);

private:
    int orientation(Point_for_kir p, Point_for_kir q, Point_for_kir r);
    vector<Point_for_kir> convexHull(Point_for_kir points[], int n);
};

#endif //LAB03_OOP_CONVEXHULL_KIRPATRICK_H
