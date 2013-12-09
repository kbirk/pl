#ifndef PL_MATH_H
#define PL_MATH_H

#include "plCommon.h"

class plVector3;
class plVector4;
class plMatrix44;
class plTriangle;
class plPolygon;
class plIntersection;

namespace plMath
{

    plVector3 projectVectorOnPlane         ( const plVector3 &vector, const plVector3 &plane_normal);
    plVector3 closestPointOnSegment        ( const plVector3 &point, const plVector3 &a, const plVector3 &b);
    plVector3 closestPointOnLine           ( const plVector3 &point, const plVector3 &lineOrigin, const plVector3& lineDirection );
    PLbool    closestPointsBetweenSegments ( const plVector3 &e1Pt1, const plVector3 &e1Pt2, const plVector3 &e2Pt1, const plVector3 &e2Pt2, plVector3& closestPointEdge1, plVector3& closestPointEdge2, PLfloat& distanceBetweenLines);
    plVector3 closestPointOnPlane          ( const plVector3 &linDir, const plVector3 &linPt, const plVector3 &plnN, const plVector3 &plnPt);
    PLbool    intersectTwoLines            ( const plVector3 &edge1Pt1, const plVector3 &edge1Pt2, const plVector3 &edge2Pt1, const plVector3 &edge2Pt2, PLfloat &edge1Param, PLfloat &edge2Param);
    PLfloat   fsqrt                        ( PLfloat x );
    PLfloat   clamp                        ( PLfloat val, PLfloat min = -1, PLfloat max = 1);    

    PLbool solveMatrix22Equation( PLfloat a11, PLfloat a12, PLfloat a21, PLfloat a22, PLfloat b1, PLfloat b2, PLfloat &x, PLfloat &y );

	void convexPolysToTris         ( std::vector<plTriangle> &tris, const std::vector<plPolygon> &polys );
	void concavePolysToTris        ( std::vector<plTriangle> &tris, const std::vector<plPolygon> &polys );

    plIntersection rayIntersect( const std::vector<plTriangle>& triangles, 
                                 const plVector3 &rayOrigin, 
                                 const plVector3 &rayDirection,
                                 PLbool smoothNormal = false, 
                                 PLbool ignoreBehindRay = false, 
                                 PLbool backFaceCull = false );  

    plIntersection rayIntersect( const plVector3 &rayOrigin, 
                                 const plVector3 &rayDirection, 
                                 const plVector3 &planePoint, 
                                 const plVector3 &planeNormal );

    plVector3 getAverageNormal( const std::vector<plTriangle>& triangles, PLfloat radius, const plVector3 &origin, const plVector3 &normal );

    plIntersection getClosestPointToRay( const std::vector<plTriangle>& triangles, const plVector3 &rayOrigin, const plVector3 &rayDirection );
    

};

#endif

