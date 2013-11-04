#ifndef PL_COLOUR_MESH_H
#define PL_COLOUR_MESH_H

#include "plCommon.h"
#include "plVector3.h"
#include "plMesh.h"

class plColourMesh : public plMesh
{
    public:
		
		plColourMesh();		      
        plColourMesh(const std::vector<plVector3> &vertices, const std::vector<PLuint> &indices);   
        plColourMesh( const plColourMesh &mesh );
        
        plColourMesh& operator = ( const plColourMesh &mesh ); 
    
        void setBuffers( const std::vector<plVector3> &vertices, const std::vector<PLuint> &indices);

};

#endif
