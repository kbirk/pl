#ifndef PL_TEXTURE_2D_H
#define PL_TEXTURE_2D_H

#include "plCommon.h"
#include "plVector3.h"


class plTexture2D 
{
   
    public:

        plTexture2D( PLuint width, PLuint height );
        plTexture2D( const plTexture2D &texture );
        
        ~plTexture2D();
        
        plTexture2D& operator = (const plTexture2D &texture );
        
        void bind() const;  // bind textures AFTER binding shader AND BEFORE drawing arrays 
        void unbind() const;

        void setTexture( const PLchar *image, PLint width, int height );

    private:
    
        GLuint _textureID;
        PLint  _width, _height;	

        void _destroy();
        void _copyTexture( const plTexture2D &texture );
};


#endif
