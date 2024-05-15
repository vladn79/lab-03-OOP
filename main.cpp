#include "recursive_alg/ConvexHull.h"
#include "graham_alg/convexhull_graham.h"
#include "andrew_jarvis_alg/ConvexHullJarvis.h"
#include "kirkpatrick_alg/convexhull_kirkpatrick.h"
#include "fortunes_alg/realization.h"
#include <cstdlib>
#include <vector>
#include <chrono>
#include <random>
#include <SFML/Graphics.hpp>

using namespace std;

constexpr float WINDOW_WIDTH = 800.0f;
constexpr float WINDOW_HEIGHT = 800.0f;
constexpr float POINT_RADIUS = 0.005f;
constexpr float OFFSET = 1.0f;

std::vector<Vector> generatePoints(int nbPoints)
{
    uint64_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::cout << "seed: " << seed << '\n';
    std::default_random_engine generator(seed);
    std::uniform_real_distribution<double> distribution (0.0, 1.0);

    std::vector<Vector> points;
    for (int i = 0; i < nbPoints; ++i)
        points.push_back(Vector{distribution(generator), distribution(generator)});

    return points;
}

void drawPoint(sf::RenderWindow& window, Vector point, sf::Color color)
{
    sf::CircleShape shape(POINT_RADIUS);
    shape.setPosition(sf::Vector2f(point.x - POINT_RADIUS, 1 - point.y - POINT_RADIUS));
    shape.setFillColor(color);
    window.draw(shape);
}

void drawEdge(sf::RenderWindow& window, Vector origin, Vector destination, sf::Color color)
{
    sf::Vertex line[] =
            {
                    sf::Vertex(sf::Vector2f(origin.x, 1.0f - origin.y), color),
                    sf::Vertex(sf::Vector2f(destination.x, 1.0f - destination.y), color)
            };
    window.draw(line, 2, sf::Lines);
}

void drawPoints(sf::RenderWindow& window, VoronoiDiagram& diagram)
{
    for (std::size_t i = 0; i < diagram.getNbSites(); ++i)
        drawPoint(window, diagram.getSite(i)->point, sf::Color::Black);
}

void drawDiagram(sf::RenderWindow& window, VoronoiDiagram& diagram)
{
    for (std::size_t i = 0; i < diagram.getNbSites(); ++i)
    {
        const VoronoiDiagram::Site* site = diagram.getSite(i);
        Vector center = site->point;
        VoronoiDiagram::Face* face = site->face;
        VoronoiDiagram::HalfEdge* halfEdge = face->outerComponent;
        if (halfEdge == nullptr)
            continue;
        while (halfEdge->prev != nullptr)
        {
            halfEdge = halfEdge->prev;
            if (halfEdge == face->outerComponent)
                break;
        }
        VoronoiDiagram::HalfEdge* start = halfEdge;
        while (halfEdge != nullptr)
        {
            if (halfEdge->origin != nullptr && halfEdge->destination != nullptr)
            {
                Vector origin = (halfEdge->origin->point - center) * OFFSET + center;
                Vector destination = (halfEdge->destination->point - center) * OFFSET + center;
                drawEdge(window, origin, destination, sf::Color::Black);
            }
            halfEdge = halfEdge->next;
            if (halfEdge == start)
                break;
        }
    }
}

VoronoiDiagram generateRandomDiagram(std::size_t nbPoints)
{

    std::vector<Vector> points = generatePoints(nbPoints);


    FortuneAlgorithm algorithm(points);
    auto start = std::chrono::steady_clock::now();
    algorithm.construct();
    auto duration = std::chrono::steady_clock::now() - start;
    std::cout << "construction: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << '\n';


    start = std::chrono::steady_clock::now();
    algorithm.bound(Field{-0.05, -0.05, 1.05, 1.05}); // Take the bounding box slightly bigger than the intersection box
    duration = std::chrono::steady_clock::now() - start;
    std::cout << "bounding: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << '\n';
    VoronoiDiagram diagram = algorithm.getDiagram();


    start = std::chrono::steady_clock::now();
    bool valid = diagram.intersect(Field{0.0, 0.0, 1.0, 1.0});
    duration = std::chrono::steady_clock::now() - start;
    std::cout << "intersection: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << '\n';
    if (!valid)
        throw std::runtime_error("An error occured in the box intersection algorithm");

    return diagram;
}


int main() {

    std::size_t nbPoints = 50;
    VoronoiDiagram diagram = generateRandomDiagram(nbPoints);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Fortune's algorithm", sf::Style::Default, settings);
    window.setView(sf::View(sf::FloatRect(-0.01f, -0.01f, 1.2f, 1.2f)));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Key::N)
                diagram = generateRandomDiagram(nbPoints);
        }

        window.clear(sf::Color::White);

        drawDiagram(window, diagram);
        drawPoints(window, diagram);

        window.display();
    }







    const int numPoints = 50;

    Point aa[numPoints];
    for (int i = 0; i < numPoints; ++i) {
        int x = rand() % 800;
        int y = rand() % 700;
        aa[i] = Point(x, y);
    }

    int n = sizeof(aa) / sizeof(aa[0]);

    ConvexHull convexHull;
   convexHull.printHull(aa, n);

    Point_for_graham points[numPoints];
    for (int i = 0; i < numPoints; ++i) {
        int x = rand() % 800;
        int y = rand() % 700;
        points[i] = Point_for_graham(x, y);
    }

    ConvexHullGraham convexHullGraham;
    convexHullGraham.convexHull(points, numPoints);

    convexHullGraham.visualizeConvexHull(points, 50);

    Point_for_Jarvis b[numPoints];
    for (int i = 0; i < numPoints; ++i) {
        int x = rand() % 800;
        int y = rand() % 700;
        b[i] = Point_for_Jarvis(x, y);
    }
    vector<Point_for_Jarvis> hull_jarvis = ConvexHullJarvis::convexHull(b, n);
    ConvexHullJarvis hull_algorithm;
    hull_algorithm.visualizeConvexHullJarvis(b, numPoints);


    Point_for_kir c[numPoints];
    for (int i = 0; i < numPoints; ++i) {
        int x = rand() % 800;
        int y = rand() % 700;
        c[i] = Point_for_kir(x, y);
    }
    vector<Point_for_kir> h = ConvexHullKirkpatrick::convexHull(c, n);
    ConvexHullKirkpatrick alg;
    alg.visualizeConvexHullKirkpatrick(c, numPoints);

    return 0;
}