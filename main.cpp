#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include<windows.h>
#include <cmath>
using namespace std;

int w = 900;
int h = 900;

sf::RenderWindow window(sf::VideoMode(w, h), "Lab3");

class Point {
public:
    float r=4;
    float x;
    float y;
    sf::CircleShape shape;
    Point(float x, float y, sf::Color color) : x(x), y(y), shape(r) {
        shape.setFillColor(color);
        shape.setPosition(x, y);
    }

    void draw() {
        window.draw(shape);
    }

    void change_color(sf::Color color) {
        shape.setFillColor(color);
    }

    bool operator ==(const Point &other) {
        return (x == other.x && y == other.y);
    }

};

class Line {
public:
    Point v_1;
    Point v_2;
    float x_1, y_1, x_2, y_2;
    Line(Point &a, Point &b) : x_1(a.x + a.r), y_1(a.y + a.r), x_2(b.x + b.r), y_2(b.y + b.r), v_1(a), v_2(b) {}

    sf::Vertex line[2] =
            {
                    sf::Vertex(sf::Vector2f(x_1, y_1)),
                    sf::Vertex(sf::Vector2f(x_2, y_2))
            };

    void draw() {
        window.draw(line, 2, sf::Lines);
    }

    bool operator == (const Line& other) {
        return (v_1 == other.v_1 && v_2 == other.v_2) || (v_2 == other.v_1 && v_1 == other.v_2);
    }
};

vector<Point> points;

bool in_points(Point& p) {
    bool res = false;
    for (int i = 0; i < points.size(); i++) {
        if (p == points[i]) {
            res = true;
            break;
        }
    }
    return res;
}



class Rand_Val{
public:
    int x1 = 10;
    int a1 = 2;
    int b1 = 3;
    int m = 867;
    int x2 = 3;
    int d2 = 12;
    int a2 = 17;
    int c2 = 7;

    int getVal() {
        x1 = (a1 * x1 + b1) % m;
        x2 = (d2 * x2 * x2 + a2 * x2 + c2) % m;

        return (x1 - x2 + m) % m;
    }
};






void file_download() {
    ifstream file("points.txt");

    do
    {
        float x;
        float y;
        file >> x >> y;
        Point p{ x, y, sf::Color::White };
        if (!in_points(p)) {
            points.push_back(p);
        }
    } while (!file.eof());

    file.close();
}

void random_points() {
    float x;
    float y;
    Rand_Val r;
    for (int i = 0; i < 30; i++) {
        x = r.getVal();
        y = r.getVal();
        Point p{ x, y, sf::Color::White };
        if (!in_points(p)) {
            points.push_back(p);
        }
    }
}


void draw_points() {
    for (auto& p : points) {
        p.draw();
    }
}

float classify(Point& p1, Point& p2, Point& p3) {
    float res = (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);
    return res;
};

vector<Point> hull;
vector<int> P;
vector<int> S;
int num = 2;
void graham() {
    int n = points.size();

    if (num >= n) {
        num = n;
    }

    if (P.empty()) {
        for (int i = 0; i < n; i++) {
            P.push_back(i);
        }


        for (int i = 1; i < n; i++) {
            if (points[P[i]].x < points[P[0]].x) {
                int tmp = P[0];
                P[0] = P[i];
                P[i] = tmp;

            }
        }





        for (int i = 2; i < n; i++) {
            int j = i;
            while (j > 1 && classify(points[P[0]], points[P[(j - 1)]], points[P[j]]) < 0) {
                int tmp = P[j];
                P[j] = P[(j - 1)];
                P[(j - 1)] = tmp;
                j--;
            }
        }
    }



    S.push_back(P[0]);
    S.push_back(P[1]);


    for (int i = 2; i < num; i++) {
        while (classify(points[S[(S.size() - 2)]], points[S[(S.size() - 1)]], points[P[i]]) < 0) {
            S.pop_back();
        }
        S.push_back(P[i]);
    }

    if (num == n) {
        S.push_back(P[0]);
    }

    int s_size = S.size();
    for (int i = 0; i < s_size; i++) {
        hull.push_back(points[S[i]]);
    }

}

vector<Line> lines;



void hull_lines() {
    int n = hull.size();
    if (n > 1) {
        for (int i = 0; i < n - 1; i++) {
            Line line{ hull[i], hull[(i + 1)] };
            lines.push_back(line);
        }
    }
}


void draw_lines() {
    for (auto& l : lines) {
        l.draw();
    }
}

class Triangle {
public:
    vector<Line> edges;
    vector<Point> nodes;
    Point c_c;
    double r;
    Triangle(const Point &a, const Point &b, const Point &c) : c_c(circum_center(a, b, c)) {
        nodes.push_back(a);
        nodes.push_back(b);
        nodes.push_back(c);
        Line l1{ nodes[0], nodes[1] };
        Line l2{ nodes[0], nodes[2] };
        Line l3{ nodes[1], nodes[2] };
        edges.push_back(l1);
        edges.push_back(l2);
        edges.push_back(l3);
        r = radius();
    }

    Point circum_center(const Point &a, const Point &b, const Point &c) {
        float numerator_x = a.y * (b.x * b.x + b.y * b.y - c.x * c.x - c.y * c.y) + b.y * (c.x * c.x + c.y * c.y - a.x * a.x - a.y * a.y) + c.y * (a.x * a.x + a.y * a.y - b.x * b.x - b.y * b.y);
        float numerator_y = a.x * (b.x * b.x + b.y * b.y - c.x * c.x - c.y * c.y) + b.x * (c.x * c.x + c.y * c.y - a.x * a.x - a.y * a.y) + c.x * (a.x * a.x + a.y * a.y - b.x * b.x - b.y * b.y);
        float denomirator = a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
        float x = -0.5 * numerator_x / denomirator;
        float y = 0.5 * numerator_y / denomirator;
        Point c_c{x, y, sf::Color::Red};
        return c_c;
    }

    double radius() {
        double r = sqrt((nodes[0].x - c_c.x) * (nodes[0].x - c_c.x) + (nodes[0].y - c_c.y) * (nodes[0].y - c_c.y));
        return r;
    }

    bool inside_circle(const Point& p) {
        double d = sqrt((p.x - c_c.x) * (p.x - c_c.x) + (p.y - c_c.y) * (p.y - c_c.y));
        return d < r;
    }

    int common_nodes(const Triangle &other) {
        int n = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (nodes[i] == other.nodes[j]) {
                    n++;
                }
            }
        }
        return n;

    }

    bool operator == (const Triangle &other) {
        int n = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (nodes[i] == other.nodes[j]) {
                    n++;
                }
            }
        }
        return (n == 3);
    }

    bool have_edge(const Line& other) {
        bool res = false;
        for (auto& l : edges) {
            if (l == other) {
                res = true;
            }
        }
        return res;
    }

    void draw() {
        for (auto& l : edges) {
            l.draw();
        }
        for (auto& n : nodes) {
            n.draw();
        }
    }
};


vector<Triangle> triangulation;
Point s_1{ -1000, -1000, sf::Color::Blue };
Point s_2{ float(3 * w), 0, sf::Color::Blue };
Point s_3{ 0, float(3 * h), sf::Color::Blue };
Triangle super{ s_1, s_2, s_3 };


int p = 0;
void BowyerWatson() {

    if (p == 0) {
        triangulation.push_back(super);
    }
    int n = points.size();
    if (p < n) {
        vector<Triangle> bad_triangles;
        for (int t = 0; t < triangulation.size(); t++) {
            if (triangulation[t].inside_circle(points[p])) {
                bad_triangles.push_back(triangulation[t]);
            }
        }
        vector<Line> polygon;

        for (int b = 0; b < bad_triangles.size(); b++) {
            for (int e = 0; e < 3; e++) {
                bool res = false;
                for (int j = 0; j < bad_triangles.size(); j++) {
                    if (bad_triangles[b] == bad_triangles[j]) {
                        continue;
                    }
                    if (bad_triangles[j].have_edge(bad_triangles[b].edges[e])) {
                        res = true;
                        break;
                    }
                }
                if (res == false) {
                    polygon.push_back(bad_triangles[b].edges[e]);
                }
            }
        }

        for (int b = 0; b < bad_triangles.size(); b++) {
            for (int it = triangulation.size() - 1; it > -1; it--) {
                if (bad_triangles[b] == triangulation[it]) {
                    triangulation.erase(triangulation.begin() + it);
                }
            }
        }
        polygon.size();
        for (int p_e = 0; p_e < polygon.size(); p_e++) {
            Triangle t_n{ polygon[p_e].v_1, polygon[p_e].v_2, points[p] };
            triangulation.push_back(t_n);
        }

    }


    if (p == n) {
        for (int i = triangulation.size() - 1; i > -1; i--) {
            if (triangulation[i].common_nodes(super) != 0) {
                triangulation.erase(triangulation.begin() + i);
            }
        }
    }

}


void triangulation_draw() {
    for (auto& tri : triangulation) {
        tri.draw();
    }
}

void voronoi() {
    int n = triangulation.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (triangulation[i].common_nodes(triangulation[j]) == 2) {
                Line l{ triangulation[i].c_c, triangulation[j].c_c };
                l.draw();
            }
        }
        triangulation[i].c_c.draw();
    }
}




bool points_ready = false;
int choice = 0;

bool before_delaney = false;
bool before_voronoi = false;
int main()
{
    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1 && !points_ready) {
                    file_download();
                }
                else if (event.key.code == sf::Keyboard::Num2 && !points_ready) {
                    random_points();
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    points_ready = true;


                }
                else if (event.key.code == sf::Keyboard::Num3 && points_ready) {
                    choice = 1;
                }
                else if (event.key.code == sf::Keyboard::Num4 && points_ready) {
                    choice = 2;
                }
                else if (event.key.code == sf::Keyboard::Num5 && points_ready) {
                    choice = 3;
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && !points_ready) {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    float x, y;
                    x = event.mouseButton.x;
                    y = event.mouseButton.y;
                    Point p{ x, y, sf::Color::White };
                    if (!in_points(p)) {
                        points.push_back(p);
                    }
                }

            }
        }
        window.clear();



        switch (choice) {
            case 0:
                draw_points();
                break;
            case 1:
                p = 0;
                triangulation.clear();
                draw_lines();
                hull.clear();
                lines.clear();
                S.clear();
                graham();
                hull_lines();
                num++;
                Sleep(500);
                break;
            case 2:
                if (before_voronoi) {
                    p = 0;
                    triangulation.clear();
                    before_voronoi = false;
                }
                before_delaney = true;
                num = 2;
                hull.clear();
                lines.clear();
                S.clear();
                P.clear();
                BowyerWatson();
                triangulation_draw();
                p++;
                Sleep(500);
                break;
            case 3:
                if (before_delaney) {
                    p = 0;
                    triangulation.clear();
                    before_delaney = false;
                }
                before_voronoi = true;
                num = 2;
                hull.clear();
                lines.clear();
                S.clear();
                P.clear();
                if (p <= points.size()+1) {
                    BowyerWatson();
                    triangulation_draw();
                    p++;
                    Sleep(500);
                }
                if (p >= points.size()) {
                    voronoi();
                    p++;
                    Sleep(500);
                }
                break;

        }



        draw_points();

        window.display();
    }

    return 0;
}