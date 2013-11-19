#ifndef PL_MESH_H
#define PL_MESH_H

#include "plCommon.h"
#include "plVector3.h"
#include "plTriangle.h"

class plMesh
{
    public:
		
		plMesh();		
		plMesh( const std::vector<plTriangle> &triangles ); 		 
		plMesh( std::vector<plTriangle>&& triangles );                      
        plMesh( const plMesh &mesh );
        plMesh( plMesh&& mesh );      
          
        virtual ~plMesh();
        
        plMesh& operator= ( const plMesh& mesh ); 
        plMesh& operator= ( plMesh&& mesh ); 
        
        const std::vector<plTriangle>& triangles() const { return _triangles; }
        void getMinMax( plVector3 &min, plVector3 &max ) const;               
        virtual plVector3 getAverageNormal ( PLfloat radius, const plVector3 &origin, const plVector3 &up ) const;                       
        virtual plIntersection rayIntersect ( const plVector3 &rayOrigin, const plVector3 &rayDirection, 
                                              PLbool smoothNormal    = false, 
                                              PLbool ignoreBehindRay = false, 
                                              PLbool backFaceCull    = false ) const;
                                                  
    protected:

        std::vector< plTriangle > _triangles;

};

#endif
