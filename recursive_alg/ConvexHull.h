#ifndef LAB03_OOP_CONVEXHULL_H
#define LAB03_OOP_CONVEXHULL_H

#include <vector>
#include "Point.h"
#include <SFML/Graphics.hpp>
using namespace std;

class ConvexHull {
private:
    vector<Point> hull;
    int findSide(const Point& p1, const Point& p2, const Point& p);
    int lineDist(const Point& p1, const Point& p2, const Point& p);
    void quickHull(Point a[], int n, const Point& p1, const Point& p2, int side);

public:
    ConvexHull();
    void drawPointsAndLines(sf::RenderWindow& window, const std::vector<Point>& points, const std::vector<Point>& hull);
    void printHull(Point a[], int n);
};

#endif //LAB03_OOP_CONVEXHULL_H
