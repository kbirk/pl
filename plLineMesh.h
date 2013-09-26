#ifndef _PL_LINE_MESH_H_
#define _PL_LINE_MESH_H_ 

#include "plCommon.h"
#include "plMesh.h"

class plLineMesh : public plMesh
{
    public:
		
		plLineMesh();		     
        plLineMesh(const plSeq<plVector3> &vertices, const plSeq<PLuint> &indices); 
        plLineMesh( const plLineMesh &mesh );
        
        plLineMesh& operator = ( const plLineMesh &mesh ); 
        
        void setBuffers( const plSeq<plVector3> &vertices, const plSeq<PLuint> &indices);

        void draw() const;
};

#endif
