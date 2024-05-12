#include "recursive_alg/ConvexHull.h"
#include "graham_alg/convexhull_graham.h"
#include "andrew_jarvis_alg/ConvexHullJarvis.h"
#include "kirkpatrick_alg/convexhull_kirkpatrick.h"
#include <cstdlib>

using namespace std;



int main() {
    const int numPoints = 10;

    Point a[] = {{53, 100}, {60, 700}, {190, 300}, {400, 100},
                 {700, 20}, {480, 480}, {200, 200}, {90, 6}, {690, 690}};
    int n = sizeof(a) / sizeof(a[0]);

    ConvexHull convexHull;
   convexHull.printHull(a, n);

    Point_for_graham points[numPoints];
    for (int i = 0; i < numPoints; ++i) {
        int x = rand() % 800;
        int y = rand() % 700;
        points[i] = Point_for_graham(x, y);
    }

    ConvexHullGraham convexHullGraham;
    convexHullGraham.convexHull(points, numPoints);

    convexHullGraham.visualizeConvexHull(points, numPoints);



   /* Point_for_graham points[] = {{2, 8}, {1, 1}, {9, 90}, {4, 4},
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

    cout << "---------"<<endl;

    Point_for_kir b[] = {{2, 8}, {1, 1}, {9, 90}, {4, 4}, {0, 0}, {1, 2}, {3, 1}, {3, 3}, {7, 12}};

    ConvexHullKirkpatrick convexh;
    vector<Point_for_kir> hullkir = convexh.printHull(b, n); */

    return 0;
}