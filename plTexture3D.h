#ifndef PL_TEXTURE_3D_H
#define PL_TEXTURE_3D_H

#include "plCommon.h"
#include "plVector3.h"
#include "plSeq.h"

class plTexture3D 
{
   
    public:

        plTexture3D( PLuint width, PLuint height, PLuint depth );
        plTexture3D( const plTexture3D &texture );
        
        ~plTexture3D();
        
        plTexture3D& operator = (const plTexture3D &texture );
        
        void bind() const;  // bind textures AFTER binding shader AND BEFORE drawing arrays 
        void unbind() const;

        void setTexture( const PLchar *image, PLint width, PLint height, PLint depth );

    private:
    
        GLuint _textureID;
        PLint  _width, _height, _depth;	

        void _destroy();
        void _copyTexture( const plTexture3D &texture );
};


#endif
