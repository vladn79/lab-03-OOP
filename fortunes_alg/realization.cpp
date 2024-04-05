#include "realization.h"
#include "math.h"
#include "data_structures.h"

#include <queue>

#define BREAKPOINTS_EPSILON 1.0e-5
#define _DEBUG_


namespace bl = beachline;


struct Event {

    enum { SITE = 0, CIRCLE = 1, SKIP = 2, };


    int type;
    Point_Fortunes point;


    int index;


    Point_Fortunes center;
    bl::BLNodePtr arc;


    Event(int _index = -1, int _type = Event::SKIP, const Point_Fortunes &_point = Point_Fortunes(0.0, 0.0)) :
            index(_index), type(_type), point(_point), arc(nullptr) {}

};


typedef std::shared_ptr<Event> EventPtr;


struct Point2DComparator {
    bool operator()(const Point_Fortunes &p1, const Point_Fortunes &p2) {
        return (p1.y == p2.y && p1.x > p2.x) || p1.y > p2.y;
    }
};


struct Point2DComparator2 {
    bool operator()(const Point_Fortunes &p1, const Point_Fortunes &p2) {
        return (p1.y == p2.y && p1.x < p2.x) || p1.y < p2.y;
    }
};


struct EventPtrComparator {
    Point2DComparator point_cmp;
    bool operator()(const EventPtr &e1, const EventPtr &e2) {
        return point_cmp(e1->point, e2->point);
    }
};


EventPtr checkCircleEvent(bl::BLNodePtr n1, bl::BLNodePtr n2, bl::BLNodePtr n3,
                          const std::vector<Point_Fortunes> &points, double sweepline) {

    if (n1 == nullptr || n2 == nullptr || n3 == nullptr)
        return nullptr;

    Point_Fortunes p1 = points[n1->get_id()];
    Point_Fortunes p2 = points[n2->get_id()];
    Point_Fortunes p3 = points[n3->get_id()];
    Point_Fortunes center, bottom;

    if (p2.y > p1.y && p2.y > p3.y)
        return nullptr;

    if (!findCircleCenter(p1, p2, p3, center))
        return nullptr;

    bottom = center;
    bottom.y += (center - p2).norm();


    if (fabs(bottom.y - sweepline) < POINT_EPSILON || sweepline < bottom.y) {

        EventPtr e = std::make_shared<Event>(-1, Event::CIRCLE, bottom);

        e->center = center;
        e->arc = n2;

        n2->circle_event = e;
        return e;
    }

    return nullptr;
}


void build_voronoi(const std::vector<Point_Fortunes> &points,
                   std::vector<bl::HalfEdgePtr> &halfedges,
                   std::vector<bl::VertexPtr> &vertices,
                   std::vector<bl::HalfEdgePtr> &faces) {

    std::priority_queue<EventPtr, std::vector<EventPtr>, EventPtrComparator> pq;


    for (size_t i = 0; i < points.size(); ++i) {
        pq.push(std::make_shared<Event>(static_cast<int>(i), Event::SITE, points[i]));
    }
    faces.resize(points.size(), nullptr);

    bl::BLNodePtr root;
    double sweepline = 0L;

    while (!pq.empty()) {

        EventPtr e = pq.top(); pq.pop();

        sweepline = e->point.y;

        if (e->type == Event::SITE) {

            int point_i = e->index;
            if (root == nullptr) {
                root = std::make_shared<bl::BLNode>(std::make_pair(point_i, point_i), &sweepline, &points);
            } else {

                bl::BLNodePtr arc = bl::find(root, e->point.x);
                bl::BLNodePtr subtree, left_leaf, right_leaf;

                if (arc->circle_event != nullptr) {
                    EventPtr circle_e = arc->circle_event;
                    circle_e->type = Event::SKIP;
                }

                int isp_num = intersectionPointsNum(points[arc->get_id()], e->point, sweepline);

                if (isp_num == 1) {
                    subtree = bl::make_simple_subtree(point_i, arc->get_id(), &sweepline, &points, halfedges);
                    left_leaf = subtree->left;
                    right_leaf = subtree->right;
                } else if (isp_num == 2) {
                    subtree = bl::make_subtree(point_i, arc->get_id(), &sweepline, &points, halfedges);
                    left_leaf = subtree->left;
                    right_leaf = subtree->right->right;
                } else {
                    continue;
                }

                if (arc->prev != nullptr)
                    bl::connect(arc->prev, left_leaf);

                if (arc->next != nullptr)
                    bl::connect(right_leaf, arc->next);

                root = bl::replace(arc, subtree);

                EventPtr circle_event = checkCircleEvent(left_leaf->prev, left_leaf, left_leaf->next, points, sweepline);
                if (circle_event != nullptr) {
                    pq.push(circle_event);
                }
                circle_event = checkCircleEvent(right_leaf->prev, right_leaf, right_leaf->next, points, sweepline);
                if (circle_event != nullptr) {
                    pq.push(circle_event);
                }
            }

        } else if (e->type == Event::CIRCLE) {

            bl::BLNodePtr arc = e->arc, prev_leaf, next_leaf;
            std::pair<bl::BLNodePtr, bl::BLNodePtr> breakpoints = bl::breakpoints(arc);
            if (breakpoints.first == nullptr || breakpoints.second == nullptr) {
                continue;
            }

            double v1 = breakpoints.first->value(), v2 = breakpoints.second->value();

            if (fabs(v1 - v2) > BREAKPOINTS_EPSILON) {
                continue;
            }

            bl::VertexPtr vertex = std::make_shared<bl::Vertex>(e->center);
            bl::HalfEdgePtr h_first = breakpoints.first->edge;
            bl::HalfEdgePtr h_second = breakpoints.second->edge;

            vertices.push_back(vertex);
            if (arc->prev != nullptr && arc->prev->circle_event != nullptr) {
                EventPtr circle_e = arc->prev->circle_event;
                circle_e->type = Event::SKIP; // ignore corresponding event
            }
            if (arc->next != nullptr && arc->next->circle_event != nullptr) {
                EventPtr circle_e = arc->next->circle_event;
                circle_e->type = Event::SKIP;
            }
            prev_leaf = arc->prev;
            next_leaf = arc->next;

            assert(prev_leaf != nullptr);
            assert(next_leaf != nullptr);
            bl::BLNodePtr new_edge_node;
            if (arc->parent == breakpoints.first)
                new_edge_node = breakpoints.second;
            else
                new_edge_node = breakpoints.first;
            root = bl::remove(arc);
            std::pair<bl::HalfEdgePtr, bl::HalfEdgePtr> twin_nodes = bl::make_twins(prev_leaf->get_id(), next_leaf->get_id());
            new_edge_node->edge = twin_nodes.first;

            bl::connect_halfedges(h_second, h_first->twin);
            bl::connect_halfedges(h_first, twin_nodes.first);
            bl::connect_halfedges(twin_nodes.second, h_second->twin);

            h_first->vertex = vertex;
            h_second->vertex = vertex;
            twin_nodes.second->vertex = vertex;
            vertex->edge = h_second;

            halfedges.push_back(twin_nodes.first);
            halfedges.push_back(twin_nodes.second);

            if (prev_leaf != nullptr && next_leaf != nullptr) {
                EventPtr circle_event = checkCircleEvent(prev_leaf->prev, prev_leaf, next_leaf, points, sweepline);
                if (circle_event != nullptr) {
                    pq.push(circle_event);
                }
                circle_event = checkCircleEvent(prev_leaf, next_leaf, next_leaf->next, points, sweepline);
                if (circle_event != nullptr) {
                    pq.push(circle_event);
                }
            }
        }
    }

    for (size_t i = 0; i < halfedges.size(); ++i) {
        bl::HalfEdgePtr he = halfedges[i];
        if (he->prev == nullptr || faces[he->l_index] == nullptr) {
            faces[he->l_index] = he;
        }
    }

}