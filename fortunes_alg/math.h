#ifndef LAB03_OOP_MATH_H
#define LAB03_OOP_MATH_H
#include <iostream>
#include <vector>
using namespace std;

class Point_Fortunes{
private:
    double x_, y_;

    struct CompareXY {
        bool operator()(const Point_Fortunes &p1, const Point_Fortunes &p2) const;
    };

public:
    static const double Inf;
    static CompareXY xy_compare;

    Point_Fortunes(double x = 0.0, double y = 0.0);
    Point_Fortunes(const Point_Fortunes &point);

    Point_Fortunes& operator=(const Point_Fortunes& other);
    bool operator==(const Point_Fortunes& other) const;
    bool operator!=(const Point_Fortunes& other) const;

    Point_Fortunes operator-() const;
    Point_Fortunes operator+(const Point_Fortunes& other) const;
    Point_Fortunes operator-(const Point_Fortunes& other) const;
    Point_Fortunes operator*(double scalar) const;
    Point_Fortunes operator/(double scalar) const;

    Point_Fortunes& operator+=(const Point_Fortunes& other);
    Point_Fortunes& operator-=(const Point_Fortunes& other);
    Point_Fortunes& operator*=(double scalar);
    Point_Fortunes& operator/=(double scalar);

    double operator[](int index) const;

    friend ostream &operator<<(ostream &stream, const Point_Fortunes &p);
    friend vector<Point_Fortunes> &operator<<(vector<Point_Fortunes> &v, const Point_Fortunes &p);

    double getX() const;
    double getY() const;
    void setX(double x);
    void setY(double y);

    bool isVertical() const;
    bool isHorizontal() const;
    bool isValid() const;
    double norm() const;
    double norm2() const;

    Point_Fortunes normalized() const;
    void normalize();

    Point_Fortunes getRotated90CW() const;
    Point_Fortunes getRotated90CCW() const;

    static bool isLeftTurn(const Point_Fortunes &p1, const Point_Fortunes &p2, const Point_Fortunes &p3);
    static bool isRightTurn(const Point_Fortunes &p1, const Point_Fortunes &p2, const Point_Fortunes &p3);
};

double dotProduct(const Point_Fortunes &p1, const Point_Fortunes &p2);
double crossProduct(const Point_Fortunes &p1, const Point_Fortunes &p2);

bool equal(const Point_Fortunes &p1, const Point_Fortunes &p2, double EPSILON = 1.0e-6);
bool equal(double v1, double v2, double EPSILON = 1.0e-6);

#endif //LAB03_OOP_MATH_H
