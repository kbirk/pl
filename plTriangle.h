#ifndef PL_TRIANGLE_H
#define	PL_TRIANGLE_H

#include "plCommon.h"
#include "plString.h"
#include "plVector3.h"
#include "plMath.h"

// a struct for storing information about ray intersection with a triangle
class plIntersection
{
    public:
        
        PLbool    exists;
        plVector3 point;
        plVector3 normal;
        PLfloat   t;
        
        plIntersection( PLbool e )
            : exists(e)
        {            
        }
        
        plIntersection( const plVector3 &p, const plVector3 &n, PLfloat tt )
            : exists(true), point(p), normal(n), t(tt)
        {            
        }
        
};


class plTriangle 
{ 
    public:

        plTriangle();
        plTriangle(const plVector3 &n, const plVector3 &p0, const plVector3 &p1, const plVector3 &p2 );
        plTriangle(const plVector3 &p0, const plVector3 &p1, const plVector3 &p2 );

        const plVector3 &point0()   const { return _points[0]; }                      
        const plVector3 &point1()   const { return _points[1]; } 
        const plVector3 &point2()   const { return _points[2]; } 
		const plVector3 &normal()   const { return _normal;    }
        const plVector3 &centroid() const { return _centroid;  }  
        PLfloat          radius()   const { return _radius;    }
		                    
        void  point0( const plVector3 &point  );
        void  point1( const plVector3 &point  );
        void  point2( const plVector3 &point  );

        void  swapVertices0And1();
         
        PLbool operator== ( const plTriangle& ) const; 
		const plVector3& operator[](int index) const { return _points[index]; }
          
        bool isInside( const plVector3 &point ) const;   
                 
        plIntersection rayIntersect( const plVector3 &rayStart, 
		                             const plVector3 &rayDir, 
		                             PLbool ignoreBehindRay = false, 
		                             PLbool backFaceCull = false ) const; 

        plVector3 barycentricCoords( const plVector3 &point ) const;

        PLbool contains( const plVector3&, const PLfloat& epsilon=PL_EPSILON ) const;
        
        PLfloat getArea() const;

        plVector3 closestPointTo( const plVector3& point ) const; 

        plVector3 closestPointOnEdge( PLuint edgeNum, const plVector3& point ) const;

    private:
    
        std::vector<plVector3> _points;
        plVector3        _normal;
        plVector3        _centroid;
        PLfloat          _radius;
        
        void _recalculate();
        void _calcRadius();
};


std::ostream& operator << ( std::ostream &stream, const plTriangle &p );


namespace plSTL
{
    PLbool importFile      ( std::vector<plTriangle> &triangles, const plString &filename, PLbool verbose = false );
    PLbool exportFileBinary( const std::vector<plTriangle> &triangles , const plString &filename );
    PLbool exportFileASCII ( const std::vector<plTriangle> &triangles , const plString &filename );
}

#endif

