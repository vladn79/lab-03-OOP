#ifndef LAB03_OOP_CONVEXHULL_GRAHAM_H
#define LAB03_OOP_CONVEXHULL_GRAHAM_H

#include <iostream>
#include <stack>
#include "point_for_graham.h"

class ConvexHullGraham {
private:
    static Point_for_graham p0;

    static Point_for_graham nextToTop(std::stack<Point_for_graham> &S);
    static int distSq(Point_for_graham p1, Point_for_graham p2);
    static int orientation(Point_for_graham p, Point_for_graham q, Point_for_graham r);
    static int compare(const void *vp1, const void *vp2);

public:
    void convexHull(Point_for_graham points[], int n);
};

#endif //LAB03_OOP_CONVEXHULL_GRAHAM_H
