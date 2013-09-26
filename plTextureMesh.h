#ifndef _PL_TEXTURE_MESH_H_
#define _PL_TEXTURE_MESH_H_ 

#include "plCommon.h"
#include "plMesh.h"
#include "plTexture.h"

class plTextureMesh : public plMesh
{
    public:
		
		plTexture texture;
		
		plTextureMesh();		
		plTextureMesh( PLuint width, PLuint height );
        plTextureMesh( PLuint width, PLuint height, const plVector3 &bottemLeft, const plVector3 &bottemRight, const plVector3 &topRight, const plVector3 &topLeft);  
             
        plTextureMesh( const plTextureMesh &mesh );
        
        plTextureMesh& operator = ( const plTextureMesh &mesh ); 

        void setBuffers( const plVector3 &bottemLeft, const plVector3 &bottemRight, const plVector3 &topRight, const plVector3 &topLeft );
        void draw() const;

};

#endif
