#include "ConvexHullJarvis.h"

using namespace std;

vector<Point_for_Jarvis> ConvexHullJarvis::convexHull(Point_for_Jarvis points[], int n) {
    vector<Point_for_Jarvis> hull;
    if (n < 3) return hull;

    int l = 0;
    for (int i = 1; i < n; i++)
        if (points[i].x < points[l].x)
            l = i;

    int p = l, q;
    do {
        hull.push_back(points[p]);
        q = (p + 1) % n;
        for (int i = 0; i < n; i++) {
            if (orientation(points[p], points[i], points[q]) == 2)
                q = i;
        }
        p = q;
    } while (p != l);

    return hull;
}


int ConvexHullJarvis::orientation(Point_for_Jarvis p, Point_for_Jarvis q, Point_for_Jarvis r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}
