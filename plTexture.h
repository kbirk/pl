#ifndef _PL_TEXTURE_H_
#define _PL_TEXTURE_H_

#include "plCommon.h"
#include "plVector3.h"
#include "plSeq.h"

class plTexture 
{
   
    public:

        plTexture( PLuint width, PLuint height );
        plTexture( const plTexture &texture );
        
        ~plTexture();
        
        plTexture& operator = (const plTexture &texture );
        
        void bind() const;  // bind textures AFTER binding shader AND BEFORE drawing arrays 
        void unbind() const;

        void setTexture( const PLchar *image, PLint width, int height );

    private:
    
        GLuint _textureID;
        PLint  _width, _height;	

        void _destroy();
        void _copyTexture( const plTexture &texture );
};


#endif
