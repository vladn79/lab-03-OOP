#ifndef LAB03_OOP_CONVEXHULLJARVIS_H
#define LAB03_OOP_CONVEXHULLJARVIS_H

#include <vector>
#include "Point_for_Jarvis.h"
#include <SFML/Graphics.hpp>

using namespace std;
class ConvexHullJarvis {
public:
    static vector<Point_for_Jarvis> convexHull(Point_for_Jarvis points[], int n);
    void visualizeConvexHullJarvis(Point_for_Jarvis points[], int n);

private:
    static int orientation(Point_for_Jarvis p, Point_for_Jarvis q, Point_for_Jarvis r);
};

#endif //LAB03_OOP_CONVEXHULLJARVIS_H
