#include "plTexture.h"

plTexture::plTexture() 
    : _width( 0 ), _height( 0 )
{
    _registerWithOpenGL();
}

plTexture::~plTexture()
{
    glDeleteTextures(1, &_textureID);
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
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP  ); 
       
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        
    glTexImage2D  ( GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 0, GL_BGR, GL_UNSIGNED_BYTE, NULL );
}


void plTexture::updateFromArthroImage( PLchar *image, PLint dimx, PLint dimy )  
{
    if (image == NULL) 
        return;
    
    _width  = dimx;
    _height = dimy;
    
    glBindTexture( GL_TEXTURE_2D, _textureID );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_BGR, GL_UNSIGNED_BYTE, image );
    glBindTexture( GL_TEXTURE_2D, 0 );  
}

