#include "ConvexHull.h"


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


    std::vector<Point> hull;
void ConvexHull::drawPointsAndLines(sf::RenderWindow& window, const std::vector<Point>& points) {

        sf::CircleShape shape(2.f);
        shape.setFillColor(sf::Color::Red);
        for (const auto& point : points) {
            shape.setPosition(point.x, point.y);
            window.draw(shape);
        }

        sf::VertexArray lines(sf::LinesStrip, hull.size() + 1);
        for (size_t i = 0; i < hull.size(); ++i) {
            lines[i].position = sf::Vector2f(hull[i].x, hull[i].y);
            lines[i].color = sf::Color::Blue;
        }

        lines[hull.size()].position = sf::Vector2f(hull[0].x, hull[0].y);
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
            if (a[i].x < a[min_x].x)
                min_x = i;
            if (a[i].x > a[max_x].x)
                max_x = i;
        }

        quickHull(a, n, a[min_x], a[max_x], 1);
        quickHull(a, n, a[min_x], a[max_x], -1);


        sf::RenderWindow window(sf::VideoMode(800, 800), "Convex Hull");

        drawPointsAndLines(window, std::vector<Point>(a, a + n));


        window.display();

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
        }
    }
