#ifndef LAB03_OOP_CONVEXHULL_H
#define LAB03_OOP_CONVEXHULL_H

#ifndef CONVEXHULL_H
#define CONVEXHULL_H

#include <iostream>
#include <stack>
#include "Point.h"

class ConvexHull {
private:
    Point *points;
    int n;
    Point p0;

    Point nextToTop(std::stack<Point> &S);
    void swap(Point &p1, Point &p2);
    int distSq(Point p1, Point p2);
    int orientation(Point p, Point q, Point r);
    static int compare(const void *vp1, const void *vp2);

public:
    ConvexHull(Point points[], int n);
    ~ConvexHull();
    void computeConvexHull();
};

#endif


#endif //LAB03_OOP_CONVEXHULL_H
