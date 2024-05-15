#include "convexhull_graham.h"

Point_for_graham ConvexHullGraham::p0;

Point_for_graham ConvexHullGraham::nextToTop(std::stack<Point_for_graham> &S) {
    Point_for_graham p = S.top();
    S.pop();
    Point_for_graham res = S.top();
    S.push(p);
    return res;
}

int ConvexHullGraham::distSq(Point_for_graham p1, Point_for_graham p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

int ConvexHullGraham::orientation(Point_for_graham p, Point_for_graham q, Point_for_graham r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0)
        return 0; // collinear
    return (val > 0) ? 1 : 2; // clock or counterclockwise
}

int ConvexHullGraham::compare(const void *vp1, const void *vp2) {
    Point_for_graham *p1 = (Point_for_graham *) vp1;
    Point_for_graham *p2 = (Point_for_graham *) vp2;

    int o = orientation(p0, *p1, *p2);
    if (o == 0)
        return (distSq(p0, *p2) >= distSq(p0, *p1)) ? -1 : 1;

    return (o == 2) ? -1 : 1;
}

void ConvexHullGraham::convexHull(Point_for_graham points[], int n) {
    int ymin = points[0].y, min = 0;
    for (int i = 1; i < n; i++) {
        int y = points[i].y;
        if ((y < ymin) || (ymin == y && points[i].x < points[min].x))
            ymin = points[i].y, min = i;
    }

    std::swap(points[0], points[min]);
    p0 = points[0];
    qsort(&points[1], n - 1, sizeof(Point_for_graham), compare);

    int m = 1;
    for (int i = 1; i < n; i++) {
        while (i < n - 1 && orientation(p0, points[i], points[i + 1]) == 0)
            i++;
        points[m] = points[i];
        m++;
    }

    if (m < 3)
        return;

    std::stack<Point_for_graham> S;
    S.push(points[0]);
    S.push(points[1]);
    S.push(points[2]);

    for (int i = 3; i < m; i++) {
        while (S.size() > 1 && orientation(nextToTop(S), S.top(), points[i]) != 2)
            S.pop();
        S.push(points[i]);
    }

    while (!S.empty()) {
        Point_for_graham p = S.top();
        std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
        S.pop();
    }
}

void ConvexHullGraham::visualizeConvexHull(Point_for_graham points[], int n) {
    std::stack<Point_for_graham> hullStack;

    int lowestY = 0;
    for (int i = 1; i < n; ++i) {
        if (points[i].y < points[lowestY].y || (points[i].y == points[lowestY].y && points[i].x < points[lowestY].x)) {
            lowestY = i;
        }
    }

    std::swap(points[0], points[lowestY]);

    std::sort(points + 1, points + n, [&](const Point_for_graham& a, const Point_for_graham& b) {
        int orientation = this->orientation(points[0], a, b);
        if (orientation == 0) {
            return this->distSq(points[0], a) < this->distSq(points[0], b);
        }
        return orientation == 2;
    });

    hullStack.push(points[0]);
    hullStack.push(points[1]);

    for (int i = 2; i < n; ++i) {
        while (hullStack.size() > 1 && this->orientation(nextToTop(hullStack), hullStack.top(), points[i]) != 2) {
            hullStack.pop();
        }
        hullStack.push(points[i]);
    }

    sf::RenderWindow window(sf::VideoMode(800, 800), "Convex Hull(Graham)");
    window.clear(sf::Color::White); // Set background color to white

    sf::CircleShape shape(2.f);
    shape.setFillColor(sf::Color::Black); // Set point color to black
    for (int i = 0; i < n; ++i) {
        shape.setPosition(points[i].x, points[i].y);
        window.draw(shape);
    }

    sf::VertexArray lines(sf::LinesStrip);
    std::vector<Point_for_graham> hullPoints;
    while (!hullStack.empty()) {
        hullPoints.push_back(hullStack.top());
        hullStack.pop();
    }
    std::reverse(hullPoints.begin(), hullPoints.end());

    for (const auto& p : hullPoints) {
        lines.append(sf::Vertex(sf::Vector2f(p.x, p.y), sf::Color::Blue)); // Set line color to black
    }

    if (!hullPoints.empty()) {
        lines.append(sf::Vertex(sf::Vector2f(hullPoints[0].x, hullPoints[0].y), sf::Color::Black)); // Close the loop
    }

    window.draw(lines);
    window.display();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
}

