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

        plTriangle();
        plTriangle(const plVector3 &n, const plVector3 &p0, const plVector3 &p1, const plVector3 &p2 );
        plTriangle(const plVector3 &p0, const plVector3 &p1, const plVector3 &p2 );

        const plVector3 &point0()   const { return _point0;   }                      
		const plVector3 &point1()   const { return _point1;   } 
		const plVector3 &point2()   const { return _point2;   } 
		const plVector3 &normal()   const { return _normal;   }
		const plVector3 &centroid() const { return _centroid; }  
		                    
        void  point0( const plVector3 &point );                     
		void  point1( const plVector3 &point );
		void  point2( const plVector3 &point );

		PLbool rayIntersect( plVector3 &intPoint, 
		                     plVector3 &intNorm, 
		                     PLfloat &t, 
		                     const plVector3 &rayStart, 
		                     const plVector3 &rayDir, 
		                     PLbool ignoreBehindRay = false, 
		                     PLbool backFaceCull = false ) const;

    private:
    
        plVector3 _normal;
        plVector3 _point0;
        plVector3 _point1;
        plVector3 _point2;
        plVector3 _centroid;
        
        void _recalculate();
};

std::ostream& operator << ( std::ostream &stream, const plTriangle &p );

void plSTLImportFile      ( plSeq<plTriangle> &triangles, plString filename );
void plSTLExportFileBinary( const plSeq<plTriangle> &triangles , plString filename );
void plSTLExportFileASCII ( const plSeq<plTriangle> &triangles , plString filename );
void _plCheckTypeSizes    ();


#endif

