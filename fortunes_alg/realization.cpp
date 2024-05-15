#include "realization.h"
#include "math.h"
#include "data_structures.h"
#include <list>
#include <unordered_map>

FortuneAlgorithm::FortuneAlgorithm(std::vector<Vector> points) : mDiagram(std::move(points))
{

}

FortuneAlgorithm::~FortuneAlgorithm() = default;

void FortuneAlgorithm::construct()
{
    for (std::size_t i = 0; i < mDiagram.getNbSites(); ++i)
        mEvents.push(std::make_unique<Event>(mDiagram.getSite(i)));

    while (!mEvents.isEmpty())
    {
        std::unique_ptr<Event> event = mEvents.pop();
        mBeachlineY = event->y;
        if(event->type == Event::Type::SITE)
            handleSiteEvent(event.get());
        else
            handleCircleEvent(event.get());
    }
}

VoronoiDiagram FortuneAlgorithm::getDiagram()
{
    return std::move(mDiagram);
}

void FortuneAlgorithm::handleSiteEvent(Event* event)
{
    VoronoiDiagram::Site* site = event->site;
    // 1. Check if the bachline is empty
    if (mBeachline.isEmpty())
    {
        mBeachline.setRoot(mBeachline.createArc(site));
        return;
    }
    // 2. Look for the arc above the site
    Arc* arcToBreak = mBeachline.locateArcAbove(site->point, mBeachlineY);
    deleteEvent(arcToBreak);
    // 3. Replace this arc by the new arcs
    Arc* middleArc = breakArc(arcToBreak, site);
    Arc* leftArc = middleArc->prev;
    Arc* rightArc = middleArc->next;
    // 4. Add an edge in the diagram
    addEdge(leftArc, middleArc);
    middleArc->rightHalfEdge = middleArc->leftHalfEdge;
    rightArc->leftHalfEdge = leftArc->rightHalfEdge;
    // 5. Check circle events
    // Left triplet
    if (!mBeachline.isNil(leftArc->prev))
        addEvent(leftArc->prev, leftArc, middleArc);
    // Right triplet
    if (!mBeachline.isNil(rightArc->next))
        addEvent(middleArc, rightArc, rightArc->next);
}

void FortuneAlgorithm::handleCircleEvent(Event* event)
{
    Vector point = event->point;
    Arc* arc = event->arc;
    // 1. Add vertex
    VoronoiDiagram::Vertex* vertex = mDiagram.createVertex(point);
    // 2. Delete all the events with this arc
    Arc* leftArc = arc->prev;
    Arc* rightArc = arc->next;
    deleteEvent(leftArc);
    deleteEvent(rightArc);
    // 3. Update the beachline and the diagram
    removeArc(arc, vertex);
    // 4. Add new circle events
    // Left triplet
    if (!mBeachline.isNil(leftArc->prev))
        addEvent(leftArc->prev, leftArc, rightArc);
    // Right triplet
    if (!mBeachline.isNil(rightArc->next))
        addEvent(leftArc, rightArc, rightArc->next);
}

Arc* FortuneAlgorithm::breakArc(Arc* arc, VoronoiDiagram::Site* site)
{

    Arc* middleArc = mBeachline.createArc(site);
    Arc* leftArc = mBeachline.createArc(arc->site);
    leftArc->leftHalfEdge = arc->leftHalfEdge;
    Arc* rightArc = mBeachline.createArc(arc->site);
    rightArc->rightHalfEdge = arc->rightHalfEdge;

    mBeachline.replace(arc, middleArc);
    mBeachline.insertBefore(middleArc, leftArc);
    mBeachline.insertAfter(middleArc, rightArc);

    delete arc;

    return middleArc;
}

void FortuneAlgorithm::removeArc(Arc* arc, VoronoiDiagram::Vertex* vertex)
{

    setDestination(arc->prev, arc, vertex);
    setDestination(arc, arc->next, vertex);

    arc->leftHalfEdge->next = arc->rightHalfEdge;
    arc->rightHalfEdge->prev = arc->leftHalfEdge;

    mBeachline.remove(arc);

    VoronoiDiagram::HalfEdge* prevHalfEdge = arc->prev->rightHalfEdge;
    VoronoiDiagram::HalfEdge* nextHalfEdge = arc->next->leftHalfEdge;
    addEdge(arc->prev, arc->next);
    setOrigin(arc->prev, arc->next, vertex);
    setPrevHalfEdge(arc->prev->rightHalfEdge, prevHalfEdge);
    setPrevHalfEdge(nextHalfEdge, arc->next->leftHalfEdge);

    delete arc;
}

bool FortuneAlgorithm::isMovingRight(const Arc* left, const Arc* right) const
{
    return left->site->point.y < right->site->point.y;
}

double FortuneAlgorithm::getInitialX(const Arc* left, const Arc* right, bool movingRight) const
{
    return movingRight ? left->site->point.x : right->site->point.x;
}

void FortuneAlgorithm::addEdge(Arc* left, Arc* right)
{

    left->rightHalfEdge = mDiagram.createHalfEdge(left->site->face);
    right->leftHalfEdge = mDiagram.createHalfEdge(right->site->face);

    left->rightHalfEdge->twin = right->leftHalfEdge;
    right->leftHalfEdge->twin = left->rightHalfEdge;
}

void FortuneAlgorithm::setOrigin(Arc* left, Arc* right, VoronoiDiagram::Vertex* vertex)
{
    left->rightHalfEdge->destination = vertex;
    right->leftHalfEdge->origin = vertex;
}

void FortuneAlgorithm::setDestination(Arc* left, Arc* right, VoronoiDiagram::Vertex* vertex)
{
    left->rightHalfEdge->origin = vertex;
    right->leftHalfEdge->destination = vertex;
}

void FortuneAlgorithm::setPrevHalfEdge(VoronoiDiagram::HalfEdge* prev, VoronoiDiagram::HalfEdge* next)
{
    prev->next = next;
    next->prev = prev;
}

void FortuneAlgorithm::addEvent(Arc* left, Arc* middle, Arc* right)
{
    double y;
    Vector convergencePoint = computeConvergencePoint(left->site->point, middle->site->point, right->site->point, y);
    bool isBelow = y <= mBeachlineY;
    bool leftBreakpointMovingRight = isMovingRight(left, middle);
    bool rightBreakpointMovingRight = isMovingRight(middle, right);
    double leftInitialX = getInitialX(left, middle, leftBreakpointMovingRight);
    double rightInitialX = getInitialX(middle, right, rightBreakpointMovingRight);
    bool isValid =
            ((leftBreakpointMovingRight && leftInitialX < convergencePoint.x) ||
             (!leftBreakpointMovingRight && leftInitialX > convergencePoint.x)) &&
            ((rightBreakpointMovingRight && rightInitialX < convergencePoint.x) ||
             (!rightBreakpointMovingRight && rightInitialX > convergencePoint.x));
    if (isValid && isBelow)
    {
        std::unique_ptr<Event> event = std::make_unique<Event>(y, convergencePoint, middle);
        middle->event = event.get();
        mEvents.push(std::move(event));
    }
}

void FortuneAlgorithm::deleteEvent(Arc* arc)
{
    if (arc->event != nullptr)
    {
        mEvents.remove(arc->event->index);
        arc->event = nullptr;
    }
}

Vector FortuneAlgorithm::computeConvergencePoint(const Vector& point1, const Vector& point2, const Vector& point3, double& y) const
{
    Vector v1 = (point1 - point2).GetOrthogonal();
    Vector v2 = (point2 - point3).GetOrthogonal();
    Vector delta = 0.5 * (point3 - point1);
    double t = delta.GetDet(v2) / v1.GetDet(v2);
    Vector center = 0.5 * (point1 + point2) + t * v1;
    double r = center.GetDistance(point1);
    y = center.y - r;
    return center;
}



bool FortuneAlgorithm::bound(Field box)
{

    for (const auto& vertex : mDiagram.getVertices())
    {
        box.left = std::min(vertex.point.x, box.left);
        box.bottom = std::min(vertex.point.y, box.bottom);
        box.right = std::max(vertex.point.x, box.right);
        box.top = std::max(vertex.point.y, box.top);
    }

    std::list<LinkedVertex> linkedVertices;
    std::unordered_map<std::size_t, std::array<LinkedVertex*, 8>> vertices(mDiagram.getNbSites());
    if (!mBeachline.isEmpty())
    {
        Arc* leftArc = mBeachline.getLeftmostArc();
        Arc* rightArc = leftArc->next;
        while (!mBeachline.isNil(rightArc))
        {

            Vector direction = (leftArc->site->point - rightArc->site->point).GetOrthogonal();
            Vector origin = (leftArc->site->point + rightArc->site->point) * 0.5f;
            Field::Intersection intersection = box.getFirstIntersection(origin, direction);

            VoronoiDiagram::Vertex* vertex = mDiagram.createVertex(intersection.point);
            setDestination(leftArc, rightArc, vertex);

            if (vertices.find(leftArc->site->index) == vertices.end())
                vertices[leftArc->site->index].fill(nullptr);
            if (vertices.find(rightArc->site->index) == vertices.end())
                vertices[rightArc->site->index].fill(nullptr);

            linkedVertices.emplace_back(LinkedVertex{nullptr, vertex, leftArc->rightHalfEdge});
            vertices[leftArc->site->index][2 * static_cast<int>(intersection.side) + 1] = &linkedVertices.back();
            linkedVertices.emplace_back(LinkedVertex{rightArc->leftHalfEdge, vertex, nullptr});
            vertices[rightArc->site->index][2 * static_cast<int>(intersection.side)] = &linkedVertices.back();

            leftArc = rightArc;
            rightArc = rightArc->next;
        }
    }

    for (auto& kv : vertices)
    {
        auto& cellVertices = kv.second;

        for (std::size_t i = 0; i < 5; ++i)
        {
            std::size_t side = i % 4;
            std::size_t nextSide = (side + 1) % 4;

            if (cellVertices[2 * side] == nullptr && cellVertices[2 * side + 1] != nullptr)
            {
                std::size_t prevSide = (side + 3) % 4;
                VoronoiDiagram::Vertex* corner = mDiagram.createCorner(box, static_cast<Field::Side>(side));
                linkedVertices.emplace_back(LinkedVertex{nullptr, corner, nullptr});
                cellVertices[2 * prevSide + 1] = &linkedVertices.back();
                cellVertices[2 * side] = &linkedVertices.back();
            }

            else if (cellVertices[2 * side] != nullptr && cellVertices[2 * side + 1] == nullptr)
            {
                VoronoiDiagram::Vertex* corner = mDiagram.createCorner(box, static_cast<Field::Side>(nextSide));
                linkedVertices.emplace_back(LinkedVertex{nullptr, corner, nullptr});
                cellVertices[2 * side + 1] = &linkedVertices.back();
                cellVertices[2 * nextSide] = &linkedVertices.back();
            }
        }
    }

    for (auto& kv : vertices)
    {
        std::size_t i = kv.first;
        auto& cellVertices = kv.second;
        for (std::size_t side = 0; side < 4; ++side)
        {
            if (cellVertices[2 * side] != nullptr)
            {

                VoronoiDiagram::HalfEdge* halfEdge = mDiagram.createHalfEdge(mDiagram.getFace(i));
                halfEdge->origin = cellVertices[2 * side]->vertex;
                halfEdge->destination = cellVertices[2 * side + 1]->vertex;
                cellVertices[2 * side]->nextHalfEdge = halfEdge;
                halfEdge->prev = cellVertices[2 * side]->prevHalfEdge;
                if (cellVertices[2 * side]->prevHalfEdge != nullptr)
                    cellVertices[2 * side]->prevHalfEdge->next = halfEdge;
                cellVertices[2 * side + 1]->prevHalfEdge = halfEdge;
                halfEdge->next = cellVertices[2 * side + 1]->nextHalfEdge;
                if (cellVertices[2 * side + 1]->nextHalfEdge != nullptr)
                    cellVertices[2 * side + 1]->nextHalfEdge->prev = halfEdge;
            }
        }
    }
    return true;
}



