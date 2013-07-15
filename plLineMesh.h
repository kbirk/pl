#ifndef _PL_LINE_MESH_H_
#define _PL_LINE_MESH_H_ 

#include "plCommon.h"
#include "plMesh.h"

class plLineMesh : public plMesh
{
    public:
		
		plLineMesh();		     
        plLineMesh(const plSeq<plVector3> &vertices, const plSeq<PLuint> &indices); 
        
        void setBuffers( const plSeq<plVector3> &vertices, const plSeq<PLuint> &indices);

};

#endif
