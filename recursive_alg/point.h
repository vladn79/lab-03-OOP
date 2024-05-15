#ifndef LAB03_OOP_POINT_H
#define LAB03_OOP_POINT_H
#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>

using namespace std;
class Point {

public:
    Point(int x = 0, int y = 0) : x(x), y(y) {}

    int getX() const;
    int getY() const;
    friend ostream& operator<<(ostream& os, const Point& point);
    bool operator==(const Point& other) const;

    int x;
    int y;

};


#endif //LAB03_OOP_POINT_H