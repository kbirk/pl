#ifndef PL_OCTREE_MESH_H
#define PL_OCTREE_MESH_H

#include "plCommon.h"
#include "plMesh.h"
#include "plOctree.h"

class plOctreeMesh : public plMesh
{
    public:
		
		plOctreeMesh();		
		plOctreeMesh( const std::vector<plTriangle> &triangles, PLuint depth ); 		 
		plOctreeMesh( std::vector<plTriangle>&& triangles, PLuint depth );                      
        
        plOctreeMesh( const plOctreeMesh &mesh );
        plOctreeMesh( plOctreeMesh&& mesh );      

        plOctreeMesh& operator= ( const plOctreeMesh& mesh ); 
        plOctreeMesh& operator= ( plOctreeMesh&& mesh ); 
        
        const plOctree& octree() const { return _octree; }
        
        plVector3      getAverageNormal ( PLfloat radius, const plVector3 &origin, const plVector3 &up ) const;
                        
        plIntersection rayIntersect ( const plVector3 &rayOrigin, const plVector3 &rayDirection, 
                                      PLbool smoothNormal    = false, 
                                      PLbool ignoreBehindRay = false, 
                                      PLbool backFaceCull    = false ) const;
                                
    protected:

        plOctree  _octree;
        
        void _buildOctree( PLuint depth );

};

#endif
