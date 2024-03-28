#include "ConvexHullJarvis.h"

using namespace std;



int ConvexHullJarvis::orientation(Point_for_Jarvis p, Point_for_Jarvis q, Point_for_Jarvis r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}
