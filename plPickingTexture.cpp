#include "plPickingTexture.h"

namespace plPicking
{
    plPickingTexture *_texture;


    void init()
    {
        delete _texture;
        _texture = new plPickingTexture(1,1);
    }
    

    const plPickingInfo& previousPick()
    {
        return _texture->previousPick();
    }
    
    
    plPickingInfo readPixel( PLuint x, PLuint y )
    {
        return _texture->readPixel( x, y );
    }


    void bind()
    {
        _texture->bind();
    }
    
    
    void unbind()
    {
        _texture->unbind();
    }
    
    
    void resize( PLuint width, PLuint height )
    {
        _texture->setFBO( width, height ); 
    }


}
///////////////////////////////////////////////////////////////////////

plPickingTexture::plPickingTexture(GLuint width, GLuint height)
    : _readSinceLastDraw(false)
{
    setFBO(width, height);
}


void plPickingTexture::destroy()
{
    glDeleteFramebuffers(1, &_fbo);
    glDeleteTextures(1, &_pickingTexture);
    glDeleteTextures(1, &_depthStencilTexture);
}


void plPickingTexture::setFBO( PLuint width, PLuint height )
{
    // destroy previous buffer
    destroy();
    
    // Create the FBO
    glGenFramebuffers(1, &_fbo);    
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    // Create the texture object for the primitive information buffer
    glGenTextures(1, &_pickingTexture);
    glBindTexture(GL_TEXTURE_2D, _pickingTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32I, width, height, 0, GL_RGB_INTEGER, GL_INT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _pickingTexture, 0);    

    // create stencil / depth buffer
    glGenRenderbuffers(1, &_depthStencilTexture);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthStencilTexture);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,   GL_RENDERBUFFER, _depthStencilTexture);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthStencilTexture);

    // Verify that the FBO is correct
    GLenum errorCode = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (errorCode != GL_FRAMEBUFFER_COMPLETE) 
    {
        std::cerr << "plPickingTexture::setFBO() error: " << errorCode << std::endl;
        return;
    }
    
    // Restore the default framebuffer
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void plPickingTexture::bind()
{   
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, _fbo );   
    _readSinceLastDraw = false;
}


void plPickingTexture::unbind()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);   
}


plPickingInfo plPickingTexture::readPixel( PLuint x, PLuint y )
{
    if (_readSinceLastDraw)
    {
        return _previousPick; // already read, no need to read buffer again
    }

    _readSinceLastDraw = true;
        
    glBindFramebuffer( GL_READ_FRAMEBUFFER, _fbo );
    glReadBuffer     ( GL_COLOR_ATTACHMENT0 );

    glReadPixels( x, y, 1, 1, GL_RGB_INTEGER, GL_INT, &_previousPick);

    glReadBuffer     ( GL_NONE );
    glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );
        
    std::cout << "picking: " << _previousPick.r << " " << _previousPick.g << " " << _previousPick.b << "\n"; 
    
    return _previousPick;
    
}


PLfloat plPickingTexture::readDepth( PLuint x, PLuint y )
{
    glBindFramebuffer( GL_READ_FRAMEBUFFER, _fbo );
    glReadBuffer     ( GL_DEPTH_ATTACHMENT );

    PLfloat depth;
    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

    glReadBuffer     ( GL_NONE) ;
    glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );
    
    return depth;
}








