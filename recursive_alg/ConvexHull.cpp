#include "ConvexHull.h"

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

int main(){
    std::cout << "s"; // testing if everything right connected to CMakeLists
}


