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
    
    // set draw buffers
    std::vector<GLenum> drawBuffers;
    drawBuffers.push_back( GL_COLOR_ATTACHMENT0 );
    drawBuffers.push_back( GL_NONE );
    drawBuffers.push_back( GL_NONE );
    drawBuffers.push_back( GL_NONE );
    drawBuffers.push_back( GL_COLOR_ATTACHMENT4 );   // no picking output
    fbo->setDrawBuffers( drawBuffers );

    // bind shader
    shader->bind();

    // set viewport        
    glViewport( 0, 0, plWindow::viewportWidth(), plWindow::viewportHeight() );      

    glDepthMask( false );
    //glDisable( GL_CULL_FACE );

    // draw main render components
    for ( const plRenderComponent& component : componentSet )
    { 
        component.draw( *shader );   
    }

    glDepthMask( true );
    //glEnable( GL_CULL_FACE );

    // unbind shader
    shader->unbind();
    // unbind fbo
    fbo->unbind();      

}
