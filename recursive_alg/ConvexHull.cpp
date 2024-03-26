#include "ConvexHull.h"
using namespace std;

ConvexHull::ConvexHull() {}

int ConvexHull::findSide(const Point& p1, const Point& p2, const Point& p) {
    int val = (p.getY() - p1.getY()) * (p2.getX() - p1.getX()) -
              (p2.getY() - p1.getY()) * (p.getX() - p1.getX());

    if (val > 0)
        return 1;
    if (val < 0)
        return -1;
    return 0;
}

int ConvexHull::lineDist(const Point& p1, const Point& p2, const Point& p) {
    return abs((p.getY() - p1.getY()) * (p2.getX() - p1.getX()) -
               (p2.getY() - p1.getY()) * (p.getX() - p1.getX()));
}

void ConvexHull::quickHull(Point a[], int n, const Point& p1, const Point& p2, int side) {
    int ind = -1;
    int max_dist = 0;

    for (int i = 0; i < n; i++) {
        int temp = lineDist(p1, p2, a[i]);
        if (findSide(p1, p2, a[i]) == side && temp > max_dist) {
            ind = i;
            max_dist = temp;
        }
    }

    if (ind == -1) {
        hull.push_back(p1);
        hull.push_back(p2);
        return;
    }

    quickHull(a, n, a[ind], p1, -findSide(a[ind], p1, p2));
    quickHull(a, n, a[ind], p2, -findSide(a[ind], p2, p1));
}

void ConvexHull::printHull(Point a[], int n) {
    if (n < 3) {
        cout << "Convex hull not possible" << endl;
        return;
    }

    int min_x = 0, max_x = 0;
    for (int i = 1; i < n; i++) {
        if (a[i].getX() < a[min_x].getX())
            min_x = i;
        if (a[i].getX() > a[max_x].getX())
            max_x = i;
    }

    quickHull(a, n, a[min_x], a[max_x], 1);
    quickHull(a, n, a[min_x], a[max_x], -1);

    cout << "The points in Convex Hull are:"<< endl;
    for (const auto& point : hull) {
        cout << point << " ";
    }
    cout << endl;
}


