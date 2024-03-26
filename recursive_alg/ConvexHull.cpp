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

int main(){
    std::cout << "s"; // testing if everything right connected to CMakeLists
}


