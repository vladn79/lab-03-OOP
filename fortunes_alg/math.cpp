#include "math.h"

#include <cmath>

Vector::Vector(double x, double y) : x(x), y(y)
{
}


Vector Vector::operator-() const
{
    return Vector(-x, -y);
}

Vector& Vector::operator+=(const Vector& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Vector& Vector::operator-=(const Vector& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector& Vector::operator*=(double t)
{
    x *= t;
    y *= t;
    return *this;
}

Vector Vector::GetOrthogonal() const
{
    return Vector(-y, x);
}

double Vector::dot(const Vector& other) const
{
    return x * other.x + y * other.y;
}

double Vector::GetNorm() const
{
    return std::sqrt(x * x + y * y);
}

double Vector::GetDistance(const Vector& other) const
{
    return (*this - other).getNorm();
}

double Vector::GetDet(const Vector& other) const
{
    return x * other.y - y * other.x;
}

Vector operator+(Vector lhs, const Vector& rhs)
{
    lhs += rhs;
    return lhs;
}

Vector operator-(Vector lhs, const Vector& rhs)
{
    lhs -= rhs;
    return lhs;
}

Vector operator*(double t, Vector vec)
{
    vec *= t;
    return vec;
}

Vector operator*(Vector vec, double t)
{
    return t * vec;
}

std::ostream& operator<<(std::ostream& os, const Vector& vec)
{
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}