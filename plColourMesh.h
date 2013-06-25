#ifndef _PL_COLOUR_MESH_H_
#define _PL_COLOUR_MESH_H_ 

#include "pl.h"
#include "plVector3.h"
#include "plMesh.h"

class plColourMesh : public plMesh
{
    public:
		
		plColourMesh();		      
        plColourMesh(const plSeq<plVector3> &interleaved_vertices, const plSeq<unsigned int> &indices);
     
        void setBuffers( const plSeq<plVector3> &interleaved_vertices, const plSeq<unsigned int> &indices);
};

#endif
