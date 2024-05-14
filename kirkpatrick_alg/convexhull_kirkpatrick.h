#ifndef LAB03_OOP_CONVEXHULL_KIRPATRICK_H
#define LAB03_OOP_CONVEXHULL_KIRPATRICK_H


#include "Point_kirkpatrick.h"
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;
class ConvexHullKirkpatrick {
public:
    vector<Point_for_kir> printHull(Point_for_kir points[], int n);
    void visualizeConvexHullKirkpatrick(Point_for_kir points[], int n);
    static vector<Point_for_kir> convexHull(Point_for_kir points[], int n);


private:
    static int orientation(Point_for_kir p, Point_for_kir q, Point_for_kir r);
};

#endif //LAB03_OOP_CONVEXHULL_KIRPATRICK_H
