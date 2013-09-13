#ifndef _PL_COLOUR_MESH_H_
#define _PL_COLOUR_MESH_H_ 

#include "plCommon.h"
#include "plVector3.h"
#include "plMesh.h"

class plColourMesh : public plMesh
{
    public:
		
		plColourMesh();		      
        plColourMesh(const plSeq<plVector3> &vertices, const plSeq<PLuint> &indices);
     
        void setBuffers( const plSeq<plVector3> &vertices, const plSeq<PLuint> &indices);
        
        /*
        void draw() const;	
        void draw( PLfloat transparency ) const;
        */
};

#endif
