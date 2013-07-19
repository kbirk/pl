#ifndef _PL_TEXTURE_H_
#define _PL_TEXTURE_H_

#include "plCommon.h"
#include "plVector3.h"

class plTexture 
{
   
    public:

        plTexture();
        plTexture( char *filename );

        void bind() const;  // bind textures AFTER binding shader AND BEFORE drawing arrays 
        void unbind() const;

        void updateFromArthroImage( PLchar *image, PLint dimx, int PLdimy );

    private:
    
        GLuint   _textureID;
    
        //GLubyte *_texmap;
        PLint    _width, _height;
        PLuint   _xdim,  _ydim;		

        void _registerWithOpenGL();
        
  
};


#endif
