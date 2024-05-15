#pragma once
#include <iostream>
class Vector;
Vector operator-(Vector lhs, const Vector& rhs);


class Vector
{
public:
    double x;
    double y;

    Vector(double x = 0.0, double y = 0.0);



    Vector operator-() const;
    Vector& operator+=(const Vector& other);
    Vector& operator-=(const Vector& other);
    Vector& operator*=(double t);



    Vector GetOrthogonal() const;
    double dot(const Vector& other) const;
    double GetNorm() const;
    double GetDistance(const Vector& other) const;
    double GetDet(const Vector& other) const;
};


Vector operator+(Vector lhs, const Vector& rhs);
Vector operator-(Vector lhs, const Vector& rhs);
Vector operator*(double t, Vector vec);
Vector operator*(Vector vec, double t);
std::ostream& operator<<(std::ostream& os, const Vector& vec);