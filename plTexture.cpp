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
    // Register with OpenGL
    glGenTextures( 1, &_textureID );
    glBindTexture( GL_TEXTURE_2D, _textureID );

    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT ); 
       
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D   ( GL_TEXTURE_2D, 0, GL_RGB, _xdim, _ydim, 0, GL_BGR, GL_UNSIGNED_BYTE, 0 );
}


void plTexture::updateFromArthroImage( PLchar *image, PLint dimx, PLint dimy )  
{
    if (image == NULL) 
        return;
    
    _width = _xdim = dimx;
    _height = _ydim = dimy;
    
    glBindTexture( GL_TEXTURE_2D, _textureID );

    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, dimx, dimy, 0, GL_BGR, GL_UNSIGNED_BYTE, image );
}

