#ifndef LAB03_OOP_POINT_H
#define LAB03_OOP_POINT_H
#include <iostream>

using namespace std;
class Point {
private:
    int x, y;

public:
    Point(int x, int y);
    int getX() const;
    int getY() const;
    friend ostream& operator<<(ostream& os, const Point& point);
    bool operator==(const Point& other) const;
};

#endif //LAB03_OOP_POINT_H