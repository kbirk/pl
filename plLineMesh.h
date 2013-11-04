#ifndef PL_LINE_MESH_H
#define PL_LINE_MESH_H

#include "plCommon.h"
#include "plMesh.h"

class plLineMesh : public plMesh
{
    public:
		
		plLineMesh();		     
        plLineMesh(const std::vector<plVector3> &vertices, const std::vector<PLuint> &indices); 
        plLineMesh( const plLineMesh &mesh );
        
        plLineMesh& operator = ( const plLineMesh &mesh ); 
        
        void setBuffers( const std::vector<plVector3> &vertices, const std::vector<PLuint> &indices);

        void draw() const;
};

#endif
