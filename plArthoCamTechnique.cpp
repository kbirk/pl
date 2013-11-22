#include "plArthroCamTechnique.h"

plArthroCamTechnique::plArthroCamTechnique()
{
}      


void plArthroCamTechnique::render( const std::set< plRenderComponent >& componentSet ) const
{
    const std::shared_ptr< plShader >& shader = plRenderResources::shaders( PL_TEXTURE_SHADER );

    // set initial rendering state
    _initState();
    
    // bind shader
    shader->bind();

    // clear back buffer
    glClearColor( PL_CLEAR_COLOUR );          
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );         
    glViewport( plWindow::viewportX(), plWindow::viewportY(), plWindow::viewportWidth(), plWindow::viewportHeight() );   
    
    plRenderComponent component = _generateComponent();

    glDisable( GL_DEPTH_TEST );

    component.draw( *shader );   
    
    glEnable( GL_DEPTH_TEST );
}


void plArthroCamTechnique::_initState() const
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
