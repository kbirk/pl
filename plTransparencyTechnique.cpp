#include "plTransparencyTechnique.h"

plTransparencyTechnique::plTransparencyTechnique()
{
}      


void plTransparencyTechnique::render( const std::set< plRenderComponent >& componentSet ) const
{
    const std::shared_ptr< plFBO >&    fbo    = plRenderResources::fbos( PL_MAIN_FBO );
    const std::shared_ptr< plShader >& shader = plRenderResources::shaders( PL_PHONG_SHADER );

    // bind fbo
    fbo->bind(); 
    
    // set draw buffers to only write to colour output
    std::vector<GLenum> drawBuffers;
    drawBuffers.push_back( GL_COLOR_ATTACHMENT0 );
    drawBuffers.push_back( GL_NONE );
    drawBuffers.push_back( GL_NONE );
    drawBuffers.push_back( GL_NONE );
    drawBuffers.push_back( GL_NONE );   // no picking output
    fbo->setDrawBuffers( drawBuffers );

    // bind shader
    shader->bind();

    // set viewport        
    glViewport( 0, 0, plWindow::viewportWidth(), plWindow::viewportHeight() );      

    glDepthMask( false );

    // draw render components to colour buffer
    for ( const plRenderComponent& component : componentSet )
    { 
        component.draw( *shader );   
    }

    // set stencil testing for picking buffer
    glEnable( GL_STENCIL_TEST) ;
    glStencilFunc( GL_NOTEQUAL, 1, 0xFF);
    glStencilMask( 0xFF );
    glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
    
    // set draw buffers to only write to picking buffer
    drawBuffers.clear();
    drawBuffers.push_back( GL_NONE ); // no colour output
    drawBuffers.push_back( GL_NONE );
    drawBuffers.push_back( GL_NONE );
    drawBuffers.push_back( GL_NONE );
    drawBuffers.push_back( GL_COLOR_ATTACHMENT4 );   
    fbo->setDrawBuffers( drawBuffers );
    
    // draw render components to picking buffer
    for ( const plRenderComponent& component : componentSet )
    { 
        component.draw( *shader );   
    }

    glDisable( GL_STENCIL_TEST );

    glDepthMask( true );

    // unbind shader
    shader->unbind();
    // unbind fbo
    fbo->unbind();      

}
