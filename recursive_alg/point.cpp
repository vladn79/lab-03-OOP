#include "Point.h"

using namespace std;


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

bool Point::operator==(const Point& other) const {
    return x == other.x && y == other.y;
}


