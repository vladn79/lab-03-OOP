#include "Point.h"

using namespace std;

Point::Point(int x, int y) : x(x), y(y) {}

int Point::getX() const {
    return x;
}

int Point::getY() const {
    return y;
}

ostream& operator<<(ostream& os, const Point& point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}
