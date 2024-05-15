#include "recursive_alg/ConvexHull.h"
#include "graham_alg/convexhull_graham.h"
#include "andrew_jarvis_alg/ConvexHullJarvis.h"
#include "kirkpatrick_alg/convexhull_kirkpatrick.h"
#include <cstdlib>

using namespace std;



int main() {
    const int numPoints = 50;

    Point aa[numPoints];
    for (int i = 0; i < numPoints; ++i) {
        int x = rand() % 800;
        int y = rand() % 700;
        aa[i] = Point(x, y);
    }

    int n = sizeof(aa) / sizeof(aa[0]);

    ConvexHull convexHull;
   convexHull.printHull(aa, n);

    Point_for_graham points[numPoints];
    for (int i = 0; i < numPoints; ++i) {
        int x = rand() % 800;
        int y = rand() % 700;
        points[i] = Point_for_graham(x, y);
    }

    ConvexHullGraham convexHullGraham;
    convexHullGraham.convexHull(points, numPoints);

    convexHullGraham.visualizeConvexHull(points, 50);

    Point_for_Jarvis b[numPoints];
    for (int i = 0; i < numPoints; ++i) {
        int x = rand() % 800;
        int y = rand() % 600;
        b[i] = Point_for_Jarvis(x, y);
    }
    vector<Point_for_Jarvis> hull_jarvis = ConvexHullJarvis::convexHull(b, n);
    ConvexHullJarvis hull_algorithm;
    hull_algorithm.visualizeConvexHullJarvis(b, numPoints);


    Point_for_kir c[numPoints];
    for (int i = 0; i < numPoints; ++i) {
        int x = rand() % 800;
        int y = rand() % 600;
        c[i] = Point_for_kir(x, y);
    }
    vector<Point_for_kir> h = ConvexHullKirkpatrick::convexHull(c, n);
    ConvexHullKirkpatrick alg;
    alg.visualizeConvexHullKirkpatrick(c, numPoints);

    return 0;
}