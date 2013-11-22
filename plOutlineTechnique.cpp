#include "plOutlineTechnique.h"

plOutlineTechnique::plOutlineTechnique()
{
}      


void plOutlineTechnique::render( const std::set< plRenderComponent >& componentSet ) const
{

    const std::shared_ptr< plFBO >&    fbo    = plRenderResources::fbos( PL_MAIN_FBO );
    const std::shared_ptr< plShader >& shader = plRenderResources::shaders( PL_OUTLINE_SHADER );

    // set initial rendering state
    _initState();
    
    // bind fbo
    fbo->bind(); 
    // bind shader
    shader->bind();

    // cset viewport
    glViewport( 0, 0, plWindow::viewportWidth(), plWindow::viewportHeight() );      
    
    // draw main render components
    for ( const plRenderComponent& component : componentSet )
    { 
        component.draw( *shader );   
    }

    // unbind shader
    shader->unbind();
    // unbind fbo
    fbo->unbind();      
}


void plOutlineTechnique::_initState() const
{
    // enable back face culling
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    
    glDepthFunc( GL_LEQUAL );
    // enable blending
    glEnable( GL_BLEND ); 
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );      
}
