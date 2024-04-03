#ifndef LAB03_OOP_DATA_STRUCTURES_H
#define LAB03_OOP_DATA_STRUCTURES_H


#include <memory>
#include "math.h"
#include <time.h>
#include <iostream>
#include <limits>
#include <iomanip>
#include <vector>
#include <random>
#include <cassert>



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

class Event;


namespace beachline {

    using namespace Doubly_connected_edge_list;

    class BLNode;
    typedef std::shared_ptr<BLNode> BLNodePtr;

    class BLNode {
    public:

        // Height of the tree
        int height;

        // Pointer to a position of a sweepline
        double *sweepline;

        // Pointer to a vector of input points
        const std::vector<Point_Fortunes> *points;

        // Indices of the points
        std::pair<int, int> indices;

        // Pointers to left, right children and parent node
        BLNodePtr left, right, parent;

        // Pointer to a circle event for a leaf node or halfedge for an internal node
        std::shared_ptr<Event> circle_event;
        std::shared_ptr<HalfEdge> edge;

        // Constructor
        BLNode(const std::pair<int,int>& _indices,
               double* _sweepline = nullptr,
               const std::vector<Point_Fortunes>* _points = nullptr,
               BLNodePtr _left = nullptr,
               BLNodePtr _right = nullptr,
               BLNodePtr _parent = nullptr,
               int _height = 1);
        BLNodePtr next, prev;

        inline bool is_leaf() {
            return indices.first == indices.second;
        }

        inline int get_id() {
            return indices.first;
        }

        inline bool has_indices(int a, int b) {
            return indices.first == a && indices.second == b;
        }

        inline bool has_indices(const std::pair<int,int> &p) {
            return indices.first == p.first && indices.second == p.second;
        }

        double value();

    };

    void connect(BLNodePtr prev, BLNodePtr next);
    bool is_root(BLNodePtr node);

    int get_height(BLNodePtr node);
    void update_height(BLNodePtr node);

    int get_balance(BLNodePtr node);
    BLNodePtr rotate_left(BLNodePtr node);
    BLNodePtr rotate_right(BLNodePtr node);
    BLNodePtr find(BLNodePtr root, double x);

    BLNodePtr replace(BLNodePtr node, BLNodePtr new_node);
    BLNodePtr remove(BLNodePtr leaf);;

    std::pair<BLNodePtr, BLNodePtr> breakpoints(BLNodePtr leaf);
    BLNodePtr make_subtree(int index, int index_behind, double *sweepline,
                           const std::vector<Point_Fortunes> *points,
                           std::vector<HalfEdgePtr> &edges);
    BLNodePtr make_simple_subtree(int index, int index_behind, double *sweepline,
                                  const std::vector<Point_Fortunes> *points,
                                  std::vector<HalfEdgePtr> &edges);
    bool _validate(BLNodePtr node);
    bool _check_balance(BLNodePtr node);
    void print_tree(BLNodePtr root, int width = 7);

}



#endif //LAB03_OOP_DATA_STRUCTURES_H
