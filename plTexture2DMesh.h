#ifndef PL_TEXTURE_2D_MESH_H
#define PL_TEXTURE_2D_MESH_H

#include "plCommon.h"
#include "plMesh.h"
#include "plTexture2D.h"

class plTexture2DMesh : public plMesh
{
    public:
		
		plTexture2D texture;
		
		plTexture2DMesh();		
		plTexture2DMesh( PLuint width, PLuint height );
        plTexture2DMesh( PLuint width, PLuint height, const plVector3 &bottemLeft, const plVector3 &bottemRight, const plVector3 &topRight, const plVector3 &topLeft);              
        plTexture2DMesh( const plTexture2DMesh &mesh );
        
        plTexture2DMesh& operator = ( const plTexture2DMesh &mesh ); 

        void setBuffers( const plVector3 &bottemLeft, const plVector3 &bottemRight, const plVector3 &topRight, const plVector3 &topLeft );
        void draw() const;

};

#endif
