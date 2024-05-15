#include "ConvexHull.h"
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <algorithm>

using namespace std;

ConvexHull::ConvexHull() {}

int ConvexHull::findSide(const Point& p1, const Point& p2, const Point& p) {
    int val = (p.getY() - p1.getY()) * (p2.getX() - p1.getX()) -
              (p2.getY() - p1.getY()) * (p.getX() - p1.getX());

    if (val > 0)
        return 1;
    if (val < 0)
        return -1;
    return 0;
}

int ConvexHull::lineDist(const Point& p1, const Point& p2, const Point& p) {
    return abs((p.getY() - p1.getY()) * (p2.getX() - p1.getX()) -
               (p2.getY() - p1.getY()) * (p.getX() - p1.getX()));
}

void ConvexHull::quickHull(Point a[], int n, const Point& p1, const Point& p2, int side) {
    int ind = -1;
    int max_dist = 0;

    for (int i = 0; i < n; i++) {
        int temp = lineDist(p1, p2, a[i]);
        if (findSide(p1, p2, a[i]) == side && temp > max_dist) {
            ind = i;
            max_dist = temp;
        }
    }

    if (ind == -1) {
        hull.push_back(p1);
        hull.push_back(p2);
        return;
    }

    quickHull(a, n, a[ind], p1, -findSide(a[ind], p1, p2));
    quickHull(a, n, a[ind], p2, -findSide(a[ind], p2, p1));
}

void ConvexHull::drawPointsAndLines(sf::RenderWindow& window, const std::vector<Point>& points, const std::vector<Point>& hull) {
    window.clear(sf::Color::White);

    sf::CircleShape shape(2.f);
    shape.setFillColor(sf::Color::Black);
    for (const auto& point : points) {
        shape.setPosition(point.getX(), point.getY());
        window.draw(shape);
    }


    sf::VertexArray lines(sf::LinesStrip, hull.size() + 1);
    for (size_t i = 0; i < hull.size(); ++i) {
        lines[i].position = sf::Vector2f(hull[i].getX(), hull[i].getY());
        lines[i].color = sf::Color::Blue;
    }


    lines[hull.size()].position = sf::Vector2f(hull[0].getX(), hull[0].getY());
    lines[hull.size()].color = sf::Color::Blue;

    window.draw(lines);
}

void ConvexHull::printHull(Point a[], int n) {
    if (n < 3) {
        std::cout << "Convex hull not possible" << std::endl;
        return;
    }

    int min_x = 0, max_x = 0;
    for (int i = 1; i < n; i++) {
        if (a[i].getX() < a[min_x].getX())
            min_x = i;
        if (a[i].getX() > a[max_x].getX())
            max_x = i;
    }

    hull.clear();
    quickHull(a, n, a[min_x], a[max_x], 1);
    quickHull(a, n, a[min_x], a[max_x], -1);


    std::sort(hull.begin(), hull.end(), [](const Point& p1, const Point& p2) {
        if (p1.getX() != p2.getX()) return p1.getX() < p2.getX();
        return p1.getY() < p2.getY();
    });
    hull.erase(std::unique(hull.begin(), hull.end()), hull.end());


    Point p0 = hull[0];
    std::sort(hull.begin() + 1, hull.end(), [p0](const Point& p1, const Point& p2) {
        int orientation = (p1.getY() - p0.getY()) * (p2.getX() - p0.getX()) - (p2.getY() - p0.getY()) * (p1.getX() - p0.getX());
        if (orientation == 0) {
            return (p0.getX() - p1.getX()) * (p0.getX() - p1.getX()) + (p0.getY() - p1.getY()) * (p0.getY() - p1.getY()) <
                   (p0.getX() - p2.getX()) * (p0.getX() - p2.getX()) + (p0.getY() - p2.getY()) * (p0.getY() - p2.getY());
        }
        return orientation < 0;
    });

    sf::RenderWindow window(sf::VideoMode(800, 800), "Convex Hull(Recursive)");

    drawPointsAndLines(window, std::vector<Point>(a, a + n), hull);

    window.display();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
}