#include "convexhull_kirkpatrick.h"
#include <algorithm>
#include <iostream>
using namespace std;

int ConvexHullKirkpatrick::orientation(Point_for_kir p, Point_for_kir q, Point_for_kir r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}


vector<Point_for_kir> ConvexHullKirkpatrick::convexHull(Point_for_kir points[], int n) {
    if (n < 3) return {};

    sort(points, points + n, [](const Point_for_kir& a, const Point_for_kir& b) {
        return a.y < b.y || (a.y == b.y && a.x < b.x);
    });
    Point_for_kir leftmost = points[0], rightmost = points[n - 1];

    vector<Point_for_kir> leftPart, rightPart;
    for (int i = 1; i < n - 1; ++i) {
        int o = orientation(leftmost, points[i], rightmost);
        if (o == 1)
            rightPart.push_back(points[i]);
        else if (o == 2)
            leftPart.push_back(points[i]);
    }

    vector<Point_for_kir> hull;
    hull.push_back(leftmost);
    for (auto& p : leftPart) {
        while (hull.size() > 1 && orientation(hull[hull.size() - 2], hull[hull.size() - 1], p) != 2)
            hull.pop_back();
        hull.push_back(p);
    }
    hull.push_back(rightmost);
    for (int i = rightPart.size() - 1; i >= 0; --i) {
        Point_for_kir p = rightPart[i];
        while (hull.size() > 1 && orientation(hull[hull.size() - 2], hull[hull.size() - 1], p) != 2)
            hull.pop_back();
        hull.push_back(p);
    }

    return hull;
}

vector<Point_for_kir> ConvexHullKirkpatrick::printHull(Point_for_kir points[], int n) {

    vector<Point_for_kir> hull = convexHull(points, n);

    for (auto& p : hull)
        cout << "(" << p.x << ", " << p.y << ")\n";

    return hull;

}

void ConvexHullKirkpatrick::visualizeConvexHullKirkpatrick(Point_for_kir points[], int n) {
    ConvexHullKirkpatrick convex;
    std::vector<Point_for_kir> hull = convex.convexHull(points, n);

    sf::RenderWindow window(sf::VideoMode(800, 800), "Convex Hull (Kir)");
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);


        sf::CircleShape pointShape(2.f);
        pointShape.setFillColor(sf::Color::Black);
        for (int i = 0; i < n; ++i) {
            pointShape.setPosition(points[i].x, points[i].y);
            window.draw(pointShape);
        }


        if (hull.size() > 1) {
            sf::VertexArray lines(sf::LinesStrip);
            for (const auto& point : hull) {
                sf::Vertex vertex(sf::Vector2f(point.x, point.y), sf::Color::Blue);
                lines.append(vertex);
            }

            sf::Vertex closingLine(sf::Vector2f(hull[0].x, hull[0].y), sf::Color::Blue);
            lines.append(closingLine);
            window.draw(lines);
        }

        window.display();
    }
}