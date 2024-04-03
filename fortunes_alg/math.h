#include <iostream>
#include <vector>
#include <limits>
#include <cmath>

#define POINT_EPSILON 1.0e-6

#define CIRCLE_CENTER_EPSILON 1.0e-7


class Point_Fortunes {

    struct Point_Fortunes_XY_Compare {
        bool operator()(const Point_Fortunes &p1, const Point_Fortunes &p2) {
            return (p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y));
        }
    };

public:
    double x, y;

    const static double Inf;
    static Point_Fortunes_XY_Compare xy_compare;

    Point_Fortunes(double x = 0.0, double y = 0.0);
    Point_Fortunes(const Point_Fortunes &point);

    friend double dotProduct(const Point_Fortunes &p1, const Point_Fortunes &p2);
    friend double crossProduct(const Point_Fortunes &p1, const Point_Fortunes &p2);

    friend Point_Fortunes operator+(const Point_Fortunes &p1, const Point_Fortunes &p2);
    friend Point_Fortunes operator-(const Point_Fortunes &p1, const Point_Fortunes &p2);
    friend Point_Fortunes operator/(const Point_Fortunes &p1, const Point_Fortunes &p2);
    friend Point_Fortunes operator*(const Point_Fortunes &p, double value);
    friend Point_Fortunes operator*(double value, const Point_Fortunes &p);
    friend Point_Fortunes operator/(const Point_Fortunes &p, double value);
    friend Point_Fortunes operator-(const Point_Fortunes &p);

    friend std::ostream &operator<<(std::ostream &stream, const Point_Fortunes &p);
    friend std::vector<Point_Fortunes> &operator<<(std::vector<Point_Fortunes> &v, const Point_Fortunes &p);

    Point_Fortunes &operator-=(const Point_Fortunes &p);
    Point_Fortunes &operator+=(const Point_Fortunes &p);
    Point_Fortunes &operator*=(double value);
    Point_Fortunes &operator/=(double value);

    Point_Fortunes normalized();
    void normalize();
    double norm();
    double norm2();

    Point_Fortunes getRotated90CW();
    Point_Fortunes getRotated90CCW();

    static bool isLeftTurn(const Point_Fortunes &p1, const Point_Fortunes &p2, const Point_Fortunes &p3);
    static bool isRightTurn(const Point_Fortunes &p1, const Point_Fortunes &p2, const Point_Fortunes &p3);

    double operator[](int i);

    void setX(double x);
    void setY(double y);

    bool isVertical();
    bool isHorizontal();
    bool isValid();

};

double dotProduct(const Point_Fortunes &p1, const Point_Fortunes &p2);

double crossProduct(const Point_Fortunes &p1, const Point_Fortunes &p2);

bool equal(const Point_Fortunes &p1, const Point_Fortunes &p2, double EPSILON = POINT_EPSILON);
bool equal(double v1, double v2, double EPSILON = POINT_EPSILON);

bool findCircleCenter(const Point_Fortunes &p1, const Point_Fortunes &p2, const Point_Fortunes &p3, Point_Fortunes &center);

int intersectionPointsNum(const Point_Fortunes &f1, const Point_Fortunes &f2, double directrix);
std::vector<Point_Fortunes> findIntersectionPoints(const Point_Fortunes &f1, const Point_Fortunes &f2, double directrix);


