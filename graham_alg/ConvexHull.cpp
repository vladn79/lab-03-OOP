#include "ConvexHull.h"

ConvexHull::ConvexHull(Point points[], int n) {
    this->points = new Point[n];
    for (int i = 0; i < n; i++) {
        this->points[i] = points[i];
    }
    this->n = n;
}
