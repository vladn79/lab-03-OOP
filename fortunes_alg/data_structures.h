#ifndef LAB03_OOP_DATA_STRUCTURES_H
#define LAB03_OOP_DATA_STRUCTURES_H


#include <memory>
#include "math.h"


namespace Doubly_connected_edge_list {

    class HalfEdge;
    class Vertex;

    typedef std::shared_ptr<HalfEdge> HalfEdgePtr;
    typedef std::shared_ptr<Vertex> VertexPtr;

    class Vertex {
    public:
        Point_Fortunes point;
        HalfEdgePtr edge;

        Vertex(const Point_Fortunes &pos, HalfEdgePtr incident_edge = nullptr);

        inline double x() { return point.x; }
        inline double y() { return point.y; }
    };

    class HalfEdge {
    public:
        int l_index, r_index;

        VertexPtr vertex;
        HalfEdgePtr twin;
        HalfEdgePtr next;
        HalfEdgePtr prev;

        HalfEdge(int _l_index, int _r_index, VertexPtr _vertex = nullptr);

        inline VertexPtr vertex0() { return vertex; }
        inline VertexPtr vertex1() { return twin->vertex; }
        inline bool is_finite() {
            return vertex != nullptr && twin->vertex != nullptr;
        }

        HalfEdgePtr vertexNextCCW();
        HalfEdgePtr vertexNextCW();
    };

    std::pair<HalfEdgePtr, HalfEdgePtr> make_twins(int left_index, int right_index);

    std::pair<HalfEdgePtr, HalfEdgePtr> make_twins(const std::pair<int,int> &indices);

    void connect_halfedges(HalfEdgePtr p1, HalfEdgePtr p2);

}


#endif //LAB03_OOP_DATA_STRUCTURES_H
