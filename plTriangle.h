#ifndef __PL_TRIANGLE_H__
#define	__PL_TRIANGLE_H__

// PLANNER
#include "pl.h"
#include "plString.h"
#include "plVector3.h"
#include "plSeq.h"

class plOrderPair
{
    public:
    
        int   index;
        float distance;

        plOrderPair (int i, float d)
        {
            index = i;
            distance = d;
        }
    
};

class plTriangle 
{ 
    public:

        plVector3 normal;
        plVector3 point1;
        plVector3 point2;
        plVector3 point3;
        plVector3 centroid;

        plTriangle()
            :   normal(0,0,0),
                point1(0,0,0),
                point2(0,0,0),
                point3(0,0,0),
                centroid(0,0,0)
        {
        }

        plTriangle(const plVector3 &n1, const plVector3 &p1, const plVector3 &p2, const plVector3 &p3 ) 
            :   normal(n1),
                point1(p1),
                point2(p2),
                point3(p3),
                centroid(0.3333f * (p1 + p2 + p3))
        {
        }

        plTriangle(PLfloat n1, PLfloat n2, PLfloat n3,
                   PLfloat p1, PLfloat p2, PLfloat p3,
                   PLfloat p4, PLfloat p5, PLfloat p6,
                   PLfloat p7, PLfloat p8, PLfloat p9) 
            :   normal(n1,n2,n3),
                point1(p1,p2,p3),
                point2(p4,p5,p6),
                point3(p7,p8,p9),
                centroid(0.3333f * (point1 + point2 + point3))           
        {
        }

		PLbool rayIntersect( plVector3 &intPoint, 
		                     plVector3 &intNorm, 
		                     PLfloat &t, 
		                     const plVector3 &rayStart, 
		                     const plVector3 &rayDir, 
		                     PLbool ignoreBehindRay = false, 
		                     PLbool backFaceCull = false ) const;
        
};

std::ostream& operator << ( std::ostream &stream, const plTriangle &p );

void plReadSTLFile(plSeq<plTriangle> &triangles, plString filename);

#endif

