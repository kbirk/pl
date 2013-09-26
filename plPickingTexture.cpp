#include "plPickingTexture.h"

namespace plPicking
{
    plPickingTexture *texture;
    plPickingInfo     value;

    void init()
    {
        delete texture;
        texture = new plPickingTexture(1,1);
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
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);

    // Create the texture object for the primitive information buffer
    glGenTextures(1, &_pickingTexture);
    glBindTexture(GL_TEXTURE_2D, _pickingTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32I, width, height, 0, GL_RGB_INTEGER, GL_INT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _pickingTexture, 0);    

    // create stencil / depth buffer
    glGenRenderbuffers(1, &_depthStencilTexture);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthStencilTexture);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthStencilTexture);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthStencilTexture);

    // Verify that the FBO is correct
    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) 
    {
        printf("FB error, status: 0x%x\n", Status);
        return;
    }
    
    // Restore the default framebuffer
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
    std::cout << "Texture width: " << width << ", Texture height: " << height << std::endl;
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
        return _lastPick; // already read, no need to read buffer again
    }

    _readSinceLastDraw = true;
        
    glBindFramebuffer( GL_READ_FRAMEBUFFER, _fbo );
    glReadBuffer     ( GL_COLOR_ATTACHMENT0 );

    glReadPixels( x, y, 1, 1, GL_RGB_INTEGER, GL_INT, &_lastPick);

    glReadBuffer     ( GL_NONE );
    glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );
        
    std::cout << "picking: " << _lastPick.type << " " << _lastPick.id << " " << _lastPick.index << "\n"; 
    
    return _lastPick;
    
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








