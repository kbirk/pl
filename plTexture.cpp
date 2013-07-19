#include "plTexture.h"

plTexture::plTexture() 
{
    _width = _height = 0;
    _registerWithOpenGL();
}

void plTexture::bind() const
{
	// bind textures AFTER binding shader AND BEFORE drawing arrays 
	glDisable(GL_BLEND);
    glActiveTexture(GL_TEXTURE0);   glBindTexture(GL_TEXTURE_2D, _textureID);          
}

void plTexture::unbind() const
{
	// unbind textures after drawing
	glEnable(GL_BLEND);
    glActiveTexture(GL_TEXTURE0);   glBindTexture(GL_TEXTURE_2D, 0);           
}


void plTexture::_registerWithOpenGL()
{
    /*
    unsigned int power;
    int exp;
    GLubyte *p, *q;

    // Find texture map dimensions of size 2^k x 2^l which contain this texture

    for (power=0, exp=1; power < sizeof(unsigned int)*8; power++, exp *= 2)
    {
        if (_width <= exp) 
        {
            _xdim = exp;
            break;
        }
    }
    
    for (power=0, exp=1; power < sizeof(unsigned int)*8; power++, exp *= 2)
    {
        if (_height <= exp) 
        {
            _ydim = exp;
            break;
        }
    }

    // Create texture map of size 2^k x 2^l, filling blank space with black

    GLubyte *newTexMap = new GLubyte[ _xdim * _ydim * 4 ];

    p = &newTexMap[0];	// destination
    q = _texmap;			// source

    for (int y=0; y<_ydim; y++)
    {
        for (int x=0; x<_xdim; x++)
        {
            if (y >= _height || x >= _width) 
            {
                // black outside of map
	            *p++ = 0; 
	            *p++ = 0; 
	            *p++ = 0; 
	            *p++ = 0;
            } 
            else 
            {
                // copy from source
	            *p++ = *q++; 
	            *p++ = *q++; 
	            *p++ = *q++; 
	            *p++ = *q++; 	            
            }
        }
    }
    */
    // Register it with OpenGL

    glGenTextures( 1, &_textureID );
    glBindTexture( GL_TEXTURE_2D, _textureID );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    // ordinary texture
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D   ( GL_TEXTURE_2D, 0, GL_RGBA, _xdim, _ydim, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
    //glTexImage2D   ( GL_TEXTURE_2D, 0, GL_RGBA, _xdim, _ydim, 0, GL_RGBA, GL_UNSIGNED_BYTE, newTexMap );
    
    //delete [] newTexMap;
}


void plTexture::updateFromArthroImage( PLchar *image, PLint dimx, PLint dimy )  
{
    _width = _xdim = dimx;
    _height = _ydim = dimy;
    //_texmap = image;

    glBindTexture( GL_TEXTURE_2D, _textureID );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, dimx, dimy, 0, GL_BGRA, GL_UNSIGNED_BYTE, image );
}

