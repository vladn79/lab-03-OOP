#include "ConvexHull.h"

int main() {
    Point a[] = {{2, 8}, {1, 1}, {9, 90}, {4, 4},
                 {0, 0}, {1, 2}, {3, 1}, {3, 3}, {7, 12}, {5, 1}};
    int n = sizeof(a) / sizeof(a[0]);

    ConvexHull convexHull;
    convexHull.printHull(a, n);

    return 0;
}
