#include "math.h"
#include <cmath>

#if defined(_WIN64) || defined(_WIN32)
#define isnan(x) _isnan(x)
#endif

using namespace std;

const double Point_Fortunes::Inf = std::numeric_limits<double>::infinity();

Point_Fortunes::Point_Fortunes_XY_Compare Point_Fortunes::xy_compare = Point_Fortunes::Point_Fortunes_XY_Compare();

Point_Fortunes::Point_Fortunes(double _x, double _y) : x(_x), y(_y) {
}

Point_Fortunes::Point_Fortunes(const Point_Fortunes &point) : x(point.x), y(point.y) {
}

double dotProduct(const Point_Fortunes &p1, const Point_Fortunes &p2) {
    return p1.x * p2.x + p1.y * p2.y;
}

double crossProduct(const Point_Fortunes &p1, const Point_Fortunes &p2) {
    return p1.x * p2.y - p1.y * p2.x;
}

Point_Fortunes operator+(const Point_Fortunes &p1, const Point_Fortunes &p2) {
    return Point_Fortunes(p1.x + p2.x, p1.y + p2.y);
}

Point_Fortunes operator-(const Point_Fortunes &p1, const Point_Fortunes &p2) {
    return Point_Fortunes(p1.x - p2.x, p1.y - p2.y);
}

Point_Fortunes operator/(const Point_Fortunes &p1, const Point_Fortunes &p2) {
    return Point_Fortunes(p1.x / p2.x, p1.y / p2.y);
}

Point_Fortunes operator*(const Point_Fortunes &p, double value) {
    return Point_Fortunes(p.x * value, p.y * value);
}

Point_Fortunes operator*(double value, const Point_Fortunes &p) {
    return Point_Fortunes(p.x * value, p.y * value);
}

Point_Fortunes operator/(const Point_Fortunes &p, double value) {
    return Point_Fortunes(p.x / value, p.y / value);
}

Point_Fortunes operator-(const Point_Fortunes &p) {
    return Point_Fortunes(-p.x, -p.y);
}

std::ostream &operator<<(std::ostream &stream, const Point_Fortunes &p) {
    stream << "(" << p.x << "," << p.y << ")";
    return stream;
}

std::vector<Point_Fortunes> &operator<<(std::vector<Point_Fortunes> &v, const Point_Fortunes &p) {
    v.push_back(p);
    return v;
}

Point_Fortunes &Point_Fortunes::operator-=(const Point_Fortunes &p) {
    x -= p.x;
    y -= p.y;
    return *this;
}

Point_Fortunes &Point_Fortunes::operator+=(const Point_Fortunes &p) {
    x += p.x;
    y += p.y;
    return *this;
}

Point_Fortunes &Point_Fortunes::operator*=(double value) {
    x *= value;
    y *= value;
    return *this;
}

Point_Fortunes &Point_Fortunes::operator/=(double value) {
    x /= value;
    y /= value;
    return *this;
}

double Point_Fortunes::operator[](int i) {
    if (i==0) return x;
    else return y;
}

void Point_Fortunes::setX(double x) {
    this->x = x;
}

void Point_Fortunes::setY(double y) {
    this->y = y;
}

bool Point_Fortunes::isVertical() {
    return (y == Inf && !isnan(x) && x != Inf);
}

bool Point_Fortunes::isHorizontal() {
    return (x == Inf && !isnan(y) && y != Inf);
}

bool Point_Fortunes::isValid() {
    if (x == Inf && y == Inf)
        return false;
    return (!isnan(x) && !isnan(y));
}

Point_Fortunes Point_Fortunes::normalized() {
    return (*this) / this->norm();
}

void Point_Fortunes::normalize() {
    double n = norm();
    x /= n;
    y /= n;
}

double Point_Fortunes::norm() {
    return sqrt(x * x + y * y);
}

double Point_Fortunes::norm2() {
    return x *x + y * y;
}

Point_Fortunes Point_Fortunes::getRotated90CW() {
    return Point_Fortunes(y, -x);
}

Point_Fortunes Point_Fortunes::getRotated90CCW() {
    return Point_Fortunes(-y, x);
}

bool Point_Fortunes::isLeftTurn(const Point_Fortunes &p1, const Point_Fortunes &p2, const Point_Fortunes &p3) {
    return (crossProduct(p2 - p1, p3 - p2) > 0.0);
}

bool Point_Fortunes::isRightTurn(const Point_Fortunes &p1, const Point_Fortunes &p2, const Point_Fortunes &p3) {
    return (crossProduct(p2 - p1, p3 - p2) < 0.0);
}

bool equal(const Point_Fortunes &p1, const Point_Fortunes &p2, double EPSILON) {
    return (fabs(p1.x - p2.x) < EPSILON && fabs(p1.y - p2.y) < EPSILON);
}

bool equal(double v1, double v2, double EPSILON) {
    return fabs(v1 - v2) < EPSILON;
}


bool findCircleCenter(const Point_Fortunes &p1, const Point_Fortunes &p2, const Point_Fortunes &p3, Point_Fortunes &center) {

    Point_Fortunes u1 = (p1 - p2).normalized(), u2 = (p3 - p2).normalized();

    double cross = crossProduct(u1, u2);
    if (fabs(cross) < CIRCLE_CENTER_EPSILON) {
        return false;
    }
    Point_Fortunes pc1 = 0.5 * (p1 + p2), pc2 = 0.5 * (p2 + p3);

    double b1 = dotProduct(u1, pc1), b2 = dotProduct(u2, pc2);
    center.x = (b1 * u2.y - b2 * u1.y) / cross;
    center.y = (u1.x * b2 - u2.x * b1) / cross;

    return true;
}
