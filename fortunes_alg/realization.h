#ifndef LAB03_OOP_REALIZATION_H
#define LAB03_OOP_REALIZATION_H

#include "math.h"
#include "data_structures.h"
using namespace std;

namespace bl = beachline;


void build_voronoi (const vector<Point_Fortunes> &points,
                   vector<bl::HalfEdgePtr> &halfedges,
                   vector<bl::VertexPtr> &vertices,
                   vector<bl::HalfEdgePtr> &faces);


#endif //LAB03_OOP_REALIZATION_H
