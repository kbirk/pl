#pragma once

#include "plCommon.h"

#include "plVector3.h"
#include "plTriangle.h"

class plEdge
{

  public:

    plVector3 pt1;
    plVector3 pt2;

    plEdge ();
    plEdge (const plVector3 &v1, const plVector3 &v2);

    bool    contains  (const plVector3& point, const float32_t &epsilon = PL_EPSILON) const;
    plVector3 otherPt   (const plVector3 &point, const float32_t &epsilon = PL_EPSILON) const;
    bool    equals    (const plEdge& e2,       const float32_t &epsilon = PL_EPSILON) const; // flexible epsilon
    bool    operator==(const plEdge& e2) const; // epsilon is absolute zero
    plEdge&   operator= (const plEdge& other);

};


class plPolygon
{

    public:

        plPolygon() {}
        plPolygon(const plTriangle& tri)
        {
            points.push_back(tri.point0());
            points.push_back(tri.point1());
            points.push_back(tri.point2());
            normal = tri.normal();
        }

        std::vector<plVector3> points;
        plVector3              normal;

};

std::ostream& operator << (std::ostream &stream, const plEdge &e);
std::ostream& operator << (std::ostream &stream, const plPolygon &p);
