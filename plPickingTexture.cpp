#include "plPickingTexture.h"

plPickingTexture::plPickingTexture(GLuint width, GLuint height)
{
    init(width, height);
}

plPickingTexture::~plPickingTexture()
{
    if (_fbo != 0) {
        glDeleteFramebuffers(1, &_fbo);
    }

    if (_pickingTexture != 0) {
        glDeleteTextures(1, &_pickingTexture);
    }
    
    if (_depthStencilTexture != 0) {
        glDeleteTextures(1, &_depthStencilTexture);
    }
}


void plPickingTexture::init(PLuint width, PLuint height)
{
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
}


void plPickingTexture::bind()
{    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
}


void plPickingTexture::unbind()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);   
}


plPickingInfo plPickingTexture::readPixel(PLuint x, PLuint y)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    plPickingInfo pi;
    glReadPixels(x, y, 1, 1, GL_RGB_INTEGER, GL_INT, &pi);

    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    
    std::cout << "picking: " << pi.type << " " << pi.id << " " << pi.index << "\n"; 
    
    return pi;
}


PLfloat plPickingTexture::readDepth(PLuint x, PLuint y)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo);
    glReadBuffer(GL_DEPTH_ATTACHMENT);

    PLfloat depth;
    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    
    return depth;
}

///////////////////////////////////

PLfloat plPickingGetDepth(PLuint x, PLuint y)
{
    return _plPickingTexture->readDepth(x,y);
}


PLint plPickingGetType(PLuint x, PLuint y)
{
   return _plPickingTexture->readPixel(x,y).type;
}

PLint plPickingGetID(PLuint x, PLuint y)
{
   return _plPickingTexture->readPixel(x,y).id;
}

PLint plPickingGetIndex(PLuint x, PLuint y)
{
   return _plPickingTexture->readPixel(x,y).index;
}

PLint plPickingSelect(PLuint x, PLuint y)
{
    plPickingInfo pi = _plPickingTexture->readPixel(x,y);

    switch (pi.type) 
    {   
        case PL_PICKING_TYPE_BONE:   
        case PL_PICKING_TYPE_CARTILAGE:
            
            _plState->selectModel(pi.id);             
            break;  
            
        case PL_PICKING_TYPE_GRAFT:
        
            _plState->selectGraft(pi.id, pi.index);
            break;

        case PL_PICKING_TYPE_NONE:

            _plState->selectNothing();
            break;
            
        case PL_PICKING_TYPE_DEFECT_CORNERS:
        case PL_PICKING_TYPE_DEFECT_BOUNDARY:
        case PL_PICKING_TYPE_DONOR_BOUNDARY:
        case PL_PICKING_TYPE_IGUIDE_BOUNDARY:
        
            _plState->selectBoundaryPoint(pi.type, pi.id, pi.index);
            break;
        
        case PL_PICKING_TYPE_DEFECT_SPLINE:
        
            _plState->selectSpline(pi.id);
            break;
        
        case PL_PICKING_TYPE_DEFECT_HANDLE_0:
        case PL_PICKING_TYPE_DEFECT_HANDLE_1:
        case PL_PICKING_TYPE_DEFECT_HANDLE_2:
        case PL_PICKING_TYPE_DEFECT_HANDLE_3:
        case PL_PICKING_TYPE_DEFECT_HANDLE_C:
        {           
            break;    
        }    
    }
    
    return pi.type; 
}









