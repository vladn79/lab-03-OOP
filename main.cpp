#include "recursive_alg/ConvexHull.h"
#include "graham_alg/convexhull_graham.h"
#include "andrew_jarvis_alg/ConvexHullJarvis.h"

using namespace std;

int main() {
    Point a[] = {{2, 8}, {1, 1}, {9, 90}, {4, 4},
                 {0, 0}, {1, 2}, {3, 1}, {3, 3}, {7, 12}};
    int n = sizeof(a) / sizeof(a[0]);

    ConvexHull convexHull;
    convexHull.printHull(a, n);

    Point_for_graham points[] = {{2, 8}, {1, 1}, {9, 90}, {4, 4},
                                 {0, 0}, {1, 2}, {3, 1}, {3, 3}, {7, 12}};
    ConvexHullGraham hull;
    hull.convexHull(points, n);

    cout << "---------"<<endl;

    Point_for_Jarvis point[] = {{2, 8}, {1, 1}, {9, 90}, {4, 4},
                                 {0, 0}, {1, 2}, {3, 1}, {3, 3}, {7, 12}};
    vector<Point_for_Jarvis> hull_jarvis = ConvexHullJarvis::convexHull(point, n);

    for (const Point_for_Jarvis& p : hull_jarvis) {
        cout << "(" << p.x << ", " << p.y << ")\n";
    }

    return 0;
}