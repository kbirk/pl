#ifndef PL_TEXTURE_2D_H
#define PL_TEXTURE_2D_H

#include "plCommon.h"
#include "plVector3.h"

template< typename T >
class plPixel
{
    public:

	    T r, g, b, a;
	    
	    plPixel()
	        : r( T() ), g( T() ), b( T() ), a( T() )
	    {
	    }
	    
	    plPixel( T red, T green, T blue, T alpha ) 
	        : r( red ), g( green ), b( blue ), a( alpha )
	    {
	    }
};


class plTexture2D 
{
   
    public:

        plTexture2D( PLuint width, PLuint height, PLuint internalFormat, PLuint format, PLuint type, GLvoid* image = NULL );
        plTexture2D( const plTexture2D& texture );
        plTexture2D( plTexture2D&& texture );
        
        ~plTexture2D();
        
        plTexture2D& operator = ( const plTexture2D& texture );
        plTexture2D& operator = ( plTexture2D&& texture );
        
        void bind  ( PLuint unit ) const;   // bind textures AFTER binding shader AND BEFORE drawing arrays 
        void unbind( PLuint unit ) const;

        PLuint width()  const { return _width;  }
        PLuint height() const { return _height; }

        void set( PLuint width, PLuint height, GLvoid* image = NULL );
        void set( PLuint width, PLuint height, PLuint internalFormat, PLuint format, PLuint type, GLvoid* image = NULL );        
        void setParameter( PLuint pname, PLuint param ); 

        friend class plFBO;
        
    private:
    
        GLuint _id;
        GLuint _width;
        GLuint _height;
        GLuint _internalFormat;
        GLuint _format;
        GLuint _type;

        PLuint _getFormatSize() const;

        void _destroy();        
        void _copy( const plTexture2D& texture );
        void _move( plTexture2D&& texture );
};


#endif
