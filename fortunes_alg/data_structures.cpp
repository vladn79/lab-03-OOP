#include "data_structures.h"

#include <unordered_set>

VoronoiDiagram::VoronoiDiagram(const std::vector<Vector>& points)
{
    mSites.reserve(points.size());
    mFaces.reserve(points.size());
    for(std::size_t i = 0; i < points.size(); ++i)
    {
        mSites.push_back(VoronoiDiagram::Site{i, points[i], nullptr});
        mFaces.push_back(VoronoiDiagram::Face{&mSites.back(), nullptr});
        mSites.back().face = &mFaces.back();
    }
}

VoronoiDiagram::Site* VoronoiDiagram::getSite(std::size_t i)
{
    return &mSites[i];
}

std::size_t VoronoiDiagram::getNbSites() const
{
    return mSites.size();
}

VoronoiDiagram::Face* VoronoiDiagram::getFace(std::size_t i)
{
    return &mFaces[i];
}

const std::list<VoronoiDiagram::Vertex>& VoronoiDiagram::getVertices() const
{
    return mVertices;
}

const std::list<VoronoiDiagram::HalfEdge>& VoronoiDiagram::getHalfEdges() const
{
    return mHalfEdges;
}

bool VoronoiDiagram::intersect(Field box)
{
    bool error = false;
    std::unordered_set<HalfEdge*> processedHalfEdges;
    std::unordered_set<Vertex*> verticesToRemove;
    for (const Site& site : mSites)
    {
        HalfEdge* halfEdge = site.face->outerComponent;
        bool inside = box.contains(halfEdge->origin->point);
        bool outerComponentDirty = !inside;
        HalfEdge* incomingHalfEdge = nullptr;
        HalfEdge* outgoingHalfEdge = nullptr;
        Field::Side incomingSide, outgoingSide;
        do
        {
            std::array<Field::Intersection, 2> intersections;
            int nbIntersections = box.getIntersections(halfEdge->origin->point, halfEdge->destination->point, intersections);
            bool nextInside = box.contains(halfEdge->destination->point);
            HalfEdge* nextHalfEdge = halfEdge->next;

            if (!inside && !nextInside)
            {

                if (nbIntersections == 0)
                {
                    verticesToRemove.emplace(halfEdge->origin);
                    removeHalfEdge(halfEdge);
                }

                else if (nbIntersections == 2)
                {
                    verticesToRemove.emplace(halfEdge->origin);
                    if (processedHalfEdges.find(halfEdge->twin) != processedHalfEdges.end())
                    {
                        halfEdge->origin = halfEdge->twin->destination;
                        halfEdge->destination = halfEdge->twin->origin;
                    }
                    else
                    {
                        halfEdge->origin = createVertex(intersections[0].point);
                        halfEdge->destination = createVertex(intersections[1].point);
                    }
                    if (outgoingHalfEdge != nullptr)
                        link(box, outgoingHalfEdge, outgoingSide, halfEdge, intersections[0].side);
                    if (incomingHalfEdge == nullptr)
                    {
                        incomingHalfEdge = halfEdge;
                        incomingSide = intersections[0].side;
                    }
                    outgoingHalfEdge = halfEdge;
                    outgoingSide = intersections[1].side;
                    processedHalfEdges.emplace(halfEdge);
                }
                else
                    error = true;
            }
            else if (inside && !nextInside)
            {
                if (nbIntersections == 1)
                {
                    if (processedHalfEdges.find(halfEdge->twin) != processedHalfEdges.end())
                        halfEdge->destination = halfEdge->twin->origin;
                    else
                        halfEdge->destination = createVertex(intersections[0].point);
                    outgoingHalfEdge = halfEdge;
                    outgoingSide = intersections[0].side;
                    processedHalfEdges.emplace(halfEdge);
                }
                else
                    error = true;
            }
            else if (!inside && nextInside)
            {
                if (nbIntersections == 1)
                {
                    verticesToRemove.emplace(halfEdge->origin);
                    if (processedHalfEdges.find(halfEdge->twin) != processedHalfEdges.end())
                        halfEdge->origin = halfEdge->twin->destination;
                    else
                        halfEdge->origin = createVertex(intersections[0].point);
                    if (outgoingHalfEdge != nullptr)
                        link(box, outgoingHalfEdge, outgoingSide, halfEdge, intersections[0].side);
                    if (incomingHalfEdge == nullptr)
                    {
                        incomingHalfEdge = halfEdge;
                        incomingSide = intersections[0].side;
                    }
                    processedHalfEdges.emplace(halfEdge);
                }
                else
                    error = true;
            }
            halfEdge = nextHalfEdge;
            inside = nextInside;
        } while (halfEdge != site.face->outerComponent);
        if (outerComponentDirty && incomingHalfEdge != nullptr)
            link(box, outgoingHalfEdge, outgoingSide, incomingHalfEdge, incomingSide);
        if (outerComponentDirty)
            site.face->outerComponent = incomingHalfEdge;
    }

    for (auto& vertex : verticesToRemove)
        removeVertex(vertex);

    return !error;
}

VoronoiDiagram::Vertex* VoronoiDiagram::createVertex(Vector point)
{
    mVertices.emplace_back();
    mVertices.back().point = point;
    mVertices.back().it = std::prev(mVertices.end());
    return &mVertices.back();
}

VoronoiDiagram::Vertex* VoronoiDiagram::createCorner(Field box, Field::Side side)
{
    switch (side)
    {
        case Field::Side::LEFT:
            return createVertex(Vector(box.left, box.top));
        case Field::Side::BOTTOM:
            return createVertex(Vector(box.left, box.bottom));
        case Field::Side::RIGHT:
            return createVertex(Vector(box.right, box.bottom));
        case Field::Side::TOP:
            return createVertex(Vector(box.right, box.top));
        default:
            return nullptr;
    }
}

VoronoiDiagram::HalfEdge* VoronoiDiagram::createHalfEdge(Face* face)
{
    mHalfEdges.emplace_back();
    mHalfEdges.back().incidentFace = face;
    mHalfEdges.back().it = std::prev(mHalfEdges.end());
    if(face->outerComponent == nullptr)
        face->outerComponent = &mHalfEdges.back();
    return &mHalfEdges.back();
}

void VoronoiDiagram::link(Field box, HalfEdge* start, Field::Side startSide, HalfEdge* end, Field::Side endSide)
{
    HalfEdge* halfEdge = start;
    int side = static_cast<int>(startSide);
    while (side != static_cast<int>(endSide))
    {
        side = (side + 1) % 4;
        halfEdge->next = createHalfEdge(start->incidentFace);
        halfEdge->next->prev = halfEdge;
        halfEdge->next->origin = halfEdge->destination;
        halfEdge->next->destination = createCorner(box, static_cast<Field::Side>(side));
        halfEdge = halfEdge->next;
    }
    halfEdge->next = createHalfEdge(start->incidentFace);
    halfEdge->next->prev = halfEdge;
    end->prev = halfEdge->next;
    halfEdge->next->next = end;
    halfEdge->next->origin = halfEdge->destination;
    halfEdge->next->destination = end->origin;
}

void VoronoiDiagram::removeVertex(Vertex* vertex)
{
    mVertices.erase(vertex->it);
}

void VoronoiDiagram::removeHalfEdge(HalfEdge* halfEdge)
{
    mHalfEdges.erase(halfEdge->it);
}