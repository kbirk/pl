#ifndef _PL_TEXTURE_H_
#define _PL_TEXTURE_H_

#include "plCommon.h"
#include "plVector3.h"

class plTexture 
{
   
    public:

        plTexture();
        plTexture( char *filename );

        ~plTexture();

        void bind() const;  // bind textures AFTER binding shader AND BEFORE drawing arrays 
        void unbind() const;

        void updateFromArthroImage( PLchar *image, PLint dimx, int PLdimy );

    private:
    
        GLuint   _textureID;
        PLint    _width, _height;	

        void _registerWithOpenGL();
        
  
};


#endif
