#include "plTexture2D.h"

plTexture2D::plTexture2D( PLuint width, PLuint height ) 
    : _textureID( 0 ), _width( width ), _height( height )
{
    setTexture( NULL, _width, _height );
}


plTexture2D::~plTexture2D()
{
    _destroy();
}


plTexture2D::plTexture2D( const plTexture2D &texture )
    : _textureID( 0 )
{
    _copyTexture( texture );
}


plTexture2D& plTexture2D::operator = ( const plTexture2D &texture )
{
    _copyTexture( texture );
    return *this;
}


void plTexture2D::_destroy()
{
    glDeleteTextures(1, &_textureID);
    _textureID = 0;
}


void plTexture2D::_copyTexture( const plTexture2D &texture )
{    
    // destroy previous buffers
    _destroy();

    plSeq<PLchar> texData( texture._width * texture._height * 3, '0' );

    // copy vertex data
    glBindTexture( GL_TEXTURE_2D, texture._textureID );    
    glGetTexImage( GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, &texData[0] );   
    glBindTexture( GL_TEXTURE_2D, 0 );
    
    // set texture
    setTexture( &texData[0], texture._width, texture._height );
}


void plTexture2D::bind() const
{
	// bind textures AFTER binding shader AND BEFORE drawing arrays 
	glDisable(GL_BLEND);
    glActiveTexture(GL_TEXTURE0);   
    glBindTexture(GL_TEXTURE_2D, _textureID);          
}


void plTexture2D::unbind() const
{
	// unbind textures after drawing
	glEnable(GL_BLEND);
    glActiveTexture(GL_TEXTURE0);   
    glBindTexture(GL_TEXTURE_2D, 0);           
}


void plTexture2D::setTexture( const PLchar *image, PLint width, PLint height )  
{
    _width  = width;
    _height = height;
    
    _destroy();
    
    glGenTextures( 1, &_textureID );          
    glBindTexture( GL_TEXTURE_2D, _textureID );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_BGR, GL_UNSIGNED_BYTE, image );
    
    glBindTexture( GL_TEXTURE_2D, 0 );  
}

