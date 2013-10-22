#ifndef PL_TEXTURE_3D_MESH_H
#define PL_TEXTURE_3D_MESH_H

#include "plCommon.h"
#include "plMesh.h"
#include "plTexture3D.h"

class plTexture3DMesh : public plMesh
{
    public:
		
		plTexture3D texture;
		
		plTexture3DMesh();		
		plTexture3DMesh( PLuint width, PLuint height, PLuint depth );
        plTexture3DMesh( PLuint width, PLuint height, PLuint depth , const plVector3 &bottemLeft, const plVector3 &bottemRight, const plVector3 &topRight, const plVector3 &topLeft);              
        plTexture3DMesh( const plTexture3DMesh &mesh );
        
        plTexture3DMesh& operator = ( const plTexture3DMesh &mesh ); 

        void setBuffers( const plVector3 &bottemLeft, const plVector3 &bottemRight, const plVector3 &topRight, const plVector3 &topLeft );
        void draw() const;

};

#endif
