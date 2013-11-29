#include "plMinimalTechnique.h"

plMinimalTechnique::plMinimalTechnique()
{
}      


void plMinimalTechnique::render( const std::set< plRenderComponent >& componentSet ) const
{
    const std::shared_ptr< plFBO >&    fbo    = plRenderResources::fbos( PL_MAIN_FBO );
    const std::shared_ptr< plShader >& shader = plRenderResources::shaders( PL_MINIMAL_SHADER );

    // set initial rendering state
    _initState(); 

    // bind fbo
    fbo->bind(); 

    // set viewport
    glViewport( 0, 0, plWindow::viewportWidth(), plWindow::viewportHeight() ); 

    // set draw buffers
    std::vector<GLenum> drawBuffers;
    drawBuffers.push_back( GL_COLOR_ATTACHMENT0 );
    drawBuffers.push_back( GL_NONE );
    drawBuffers.push_back( GL_NONE );
    drawBuffers.push_back( GL_NONE );
    drawBuffers.push_back( GL_COLOR_ATTACHMENT4 );
    fbo->setDrawBuffers( drawBuffers );

    // bind shader
    shader->bind();

    glDisable( GL_DEPTH_TEST );

    // draw main render components
    for ( const plRenderComponent& component : componentSet )
    { 
        component.draw( *shader );   
    }

    glEnable( GL_DEPTH_TEST );

    // unbind shader
    shader->unbind();

    // unbind fbo
    fbo->unbind(); 

}


void plMinimalTechnique::_initState() const
{
    // enable back face culling
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    // set depth testing
    glEnable( GL_DEPTH_TEST ); 
    glDepthFunc( GL_LEQUAL );
    // enable blending
    glEnable( GL_BLEND ); 
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );      
}
