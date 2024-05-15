#ifndef LAB03_OOP_REALIZATION_H
#define LAB03_OOP_REALIZATION_H

#include "data_structures.h"

class FortuneAlgorithm
{
public:

    FortuneAlgorithm(std::vector<Vector> points);
    ~FortuneAlgorithm();

    void construct();
    bool bound(Field box);

    VoronoiDiagram getDiagram();

private:
    VoronoiDiagram mDiagram;
    Beachline mBeachline;
    PriorityQueue<Event> mEvents;
    double mBeachlineY;


    void handleSiteEvent(Event* event);
    void handleCircleEvent(Event* event);


    Arc* breakArc(Arc* arc, VoronoiDiagram::Site* site);
    void removeArc(Arc* arc, VoronoiDiagram::Vertex* vertex);


    bool isMovingRight(const Arc* left, const Arc* right) const;
    double getInitialX(const Arc* left, const Arc* right, bool movingRight) const;


    void addEdge(Arc* left, Arc* right);
    void setOrigin(Arc* left, Arc* right, VoronoiDiagram::Vertex* vertex);
    void setDestination(Arc* left, Arc* right, VoronoiDiagram::Vertex* vertex);
    void setPrevHalfEdge(VoronoiDiagram::HalfEdge* prev, VoronoiDiagram::HalfEdge* next);


    void addEvent(Arc* left, Arc* middle, Arc* right);
    void deleteEvent(Arc* arc);
    Vector computeConvergencePoint(const Vector& point1, const Vector& point2, const Vector& point3, double& y) const;


    struct LinkedVertex
    {
        VoronoiDiagram::HalfEdge* prevHalfEdge;
        VoronoiDiagram::Vertex* vertex;
        VoronoiDiagram::HalfEdge* nextHalfEdge;
    };
};

#endif //LAB03_OOP_REALIZATION_H
