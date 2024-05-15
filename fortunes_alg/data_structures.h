#pragma once

#include <array>
#include <limits>
#include <vector>
#include <list>
#include "math.h"

class Field
{
public:
    enum class Side : int {LEFT, BOTTOM, RIGHT, TOP};

    struct Intersection
    {
        Side side;
        Vector point;
    };

    double left;
    double bottom;
    double right;
    double top;

    bool contains(const Vector& point) const;
    Intersection getFirstIntersection(const Vector& origin, const Vector& direction) const;
    int getIntersections(const Vector& origin, const Vector& destination, std::array<Intersection, 2>& intersections) const;

private:
    static constexpr double EPSILON = std::numeric_limits<double>::epsilon();
};


class FortuneAlgorithm;

class VoronoiDiagram
{
public:
    struct HalfEdge;
    struct Face;

    struct Site
    {
        std::size_t index;
        Vector point;
        Face* face;
    };

    struct Vertex
    {
        Vector point;

    private:
        friend VoronoiDiagram;
        std::list<Vertex>::iterator it;
    };

    struct HalfEdge
    {
        Vertex* origin = nullptr;
        Vertex* destination = nullptr;
        HalfEdge* twin = nullptr;
        Face* incidentFace;
        HalfEdge* prev = nullptr;
        HalfEdge* next = nullptr;

    private:
        friend VoronoiDiagram;
        std::list<HalfEdge>::iterator it;
    };

    struct Face
    {
        Site* site;
        HalfEdge* outerComponent;
    };

    VoronoiDiagram(const std::vector<Vector>& points);

    VoronoiDiagram(const VoronoiDiagram&) = delete;
    VoronoiDiagram& operator=(const VoronoiDiagram&) = delete;

    VoronoiDiagram(VoronoiDiagram&&) = default;
    VoronoiDiagram& operator=(VoronoiDiagram&&) = default;

    Site* getSite(std::size_t i);
    std::size_t getNbSites() const;
    Face* getFace(std::size_t i);
    const std::list<Vertex>& getVertices() const;
    const std::list<HalfEdge>& getHalfEdges() const;

    bool intersect(Field box);

private:
    std::vector<Site> mSites;
    std::vector<Face> mFaces;
    std::list<Vertex> mVertices;
    std::list<HalfEdge> mHalfEdges;


    friend FortuneAlgorithm;

    Vertex* createVertex(Vector point);
    Vertex* createCorner(Field box, Field::Side side);
    HalfEdge* createHalfEdge(Face* face);


    void link(Field box, HalfEdge* start, Field::Side startSide, HalfEdge* end, Field::Side endSide);
    void removeVertex(Vertex* vertex);
    void removeHalfEdge(HalfEdge* halfEdge);
};

class Event;

struct Arc
{
    enum class Color{RED, BLACK};


    Arc* parent;
    Arc* left;
    Arc* right;

    VoronoiDiagram::Site* site;
    VoronoiDiagram::HalfEdge* leftHalfEdge;
    VoronoiDiagram::HalfEdge* rightHalfEdge;
    Event* event;

    Arc* prev;
    Arc* next;
    Color color;
};

class Event
{
public:
    enum class Type{SITE, CIRCLE};

    Event(VoronoiDiagram::Site* site);
    Event(double y, Vector point, Arc* arc);

    const Type type;
    double y;
    int index;
    VoronoiDiagram::Site* site;
    Vector point;
    Arc* arc;

};

bool operator<(const Event& lhs, const Event& rhs);
std::ostream& operator<<(std::ostream& os, const Event& event);


class Beachline
{
public:
    Beachline();
    ~Beachline();

    Beachline(const Beachline&) = delete;
    Beachline& operator=(const Beachline&) = delete;
    Beachline(Beachline&&) = delete;
    Beachline& operator=(Beachline&&) = delete;

    Arc* createArc(VoronoiDiagram::Site* site);

    bool isEmpty() const;
    bool isNil(const Arc* x) const;
    void setRoot(Arc* x);
    Arc* getLeftmostArc() const;

    Arc* locateArcAbove(const Vector& point, double l) const;
    void insertBefore(Arc* x, Arc* y);
    void insertAfter(Arc* x, Arc* y);
    void replace(Arc* x, Arc* y);
    void remove(Arc* z);

    std::ostream& print(std::ostream& os) const;

private:
    Arc* mNil;
    Arc* mRoot;

    Arc* minimum(Arc* x) const;
    void transplant(Arc* u, Arc* v);

    void insertFixup(Arc* z);
    void removeFixup(Arc* x);

    void leftRotate(Arc* x);
    void rightRotate(Arc* y);

    double computeBreakpoint(const Vector& point1, const Vector& point2, double l) const;

    void free(Arc* x);

    std::ostream& printArc(std::ostream& os, const Arc* arc, std::string tabs = "") const;
};

std::ostream& operator<<(std::ostream& os, const Beachline& beachline);