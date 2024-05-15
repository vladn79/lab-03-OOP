#include "ConvexHullJarvis.h"

using namespace std;

vector<Point_for_Jarvis> ConvexHullJarvis::convexHull(Point_for_Jarvis points[], int n) {
    vector<Point_for_Jarvis> hull;
    if (n < 3) return hull;

    int l = 0;
    for (int i = 1; i < n; i++)
        if (points[i].x < points[l].x)
            l = i;

    int p = l, q;
    do {
        hull.push_back(points[p]);
        q = (p + 1) % n;
        for (int i = 0; i < n; i++) {
            if (orientation(points[p], points[i], points[q]) == 2)
                q = i;
        }
        p = q;
    } while (p != l);

    return hull;
}


int ConvexHullJarvis::orientation(Point_for_Jarvis p, Point_for_Jarvis q, Point_for_Jarvis r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

void ConvexHullJarvis::visualizeConvexHullJarvis(Point_for_Jarvis points[], int n) {
    ConvexHullJarvis convexHullJarvis;
    std::vector<Point_for_Jarvis> hull = convexHullJarvis.convexHull(points, n);

    sf::RenderWindow window(sf::VideoMode(800, 800), "Convex Hull (Jarvis)");
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
