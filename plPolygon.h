#ifndef __PL_POLYGON_H__
#define __PL_POLYGON_H__

#include "plCommon.h"
#include "plSeq.h"
#include "plVector3.h"


class plEdge
{

  public:

    plVector3 pt1;
    plVector3 pt2;

    plEdge ();
    plEdge (const plVector3 &v1, const plVector3 &v2);

    PLbool    contains  (const plVector3& point, const PLfloat &epsilon = PL_EPSILON);
    plVector3 otherPt   (const plVector3 &point, const PLfloat &epsilon = PL_EPSILON);
    PLbool    equals    (const plEdge& e2,       const PLfloat &epsilon = PL_EPSILON); // flexible epsilon
    PLbool    operator==(const plEdge& e2); // epsilon is absolute zero

};


class plPolygon 
{

    public:
    
        plSeq<plVector3> points;
        plVector3        normal;

};

std::ostream& operator << ( std::ostream &stream, const plEdge &e );
std::ostream& operator << ( std::ostream &stream, const plPolygon &p );

#endif
