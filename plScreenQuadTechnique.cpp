#include "plScreenQuadTechnique.h"

plScreenQuadTechnique::plScreenQuadTechnique()
{
}      


void plScreenQuadTechnique::render( const std::set< plRenderComponent >& componentSet ) const
{
    const std::shared_ptr< plShader >& shader = plRenderResources::shaders( PL_FBO_SHADER );

    // set initial rendering state
    _initState();
    
    // bind shader
    shader->bind();

    // clear back buffer
    glClearColor( PL_CLEAR_COLOUR );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    glViewport( plWindow::viewportX(), plWindow::viewportY(), plWindow::viewportWidth(), plWindow::viewportHeight() );   

    plRenderComponent component = _generateComponent();

    component.draw( *shader ); 
}


void plScreenQuadTechnique::_initState() const
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


plVAO plScreenQuadTechnique::_generateQuad() const
{
    std::vector<plVector3> vertices;        vertices.reserve( 8 );
    std::vector<PLuint>    indices;         indices.reserve( 6 );

    // position                                     // texture coord       
    vertices.push_back( plVector3( -1, -1, 0 ) );   vertices.push_back( plVector3( 0,0,0) );   
    vertices.push_back( plVector3(  1, -1, 0 ) );   vertices.push_back( plVector3( 1,0,0) );   
    vertices.push_back( plVector3(  1,  1, 0 ) );   vertices.push_back( plVector3( 1,1,0) );    
    vertices.push_back( plVector3( -1,  1, 0 ) );   vertices.push_back( plVector3( 0,1,0) );
    
    indices.push_back( 0 );   indices.push_back( 1 );   indices.push_back( 2 );
    indices.push_back( 0 );   indices.push_back( 2 );   indices.push_back( 3 );

    // set vbo and attach attribute pointers
    std::shared_ptr< plVBO > vbo = std::make_shared< plVBO >();
    vbo->set( vertices );
    vbo->set( plVertexAttributePointer( PL_POSITION_ATTRIBUTE, 0  ) );
    vbo->set( plVertexAttributePointer( PL_TEXCOORD_ATTRIBUTE, 16 ) );
    // set eabo
    std::shared_ptr<plEABO> eabo = std::make_shared< plEABO >();    
    eabo->set( indices );

    plVAO vao;
    // attach to vao
    vao.attach( vbo );
    vao.attach( eabo );
    // upload to gpu
    vao.upload(); 

    return vao;
}


plRenderComponent plScreenQuadTechnique::_generateComponent() const
{
    static plMatrix44 ortho( -1, 1, -1, 1, -1, 1 );

    static plMatrix44 camera( 1, 0,  0, 0,
                              0, 1,  0, 0,
                              0, 0, -1, 0,
                              0, 0,  0, 1 ); 

    static std::shared_ptr< plVAO > screenQuadVAO = std::make_shared<plVAO>( _generateQuad() );

    const std::shared_ptr< plFBO >& fbo = plRenderResources::fbos( PL_MAIN_FBO );

    plRenderComponent component( screenQuadVAO );

    // attach transformation uniforms
    component.attach( plUniform( PL_MODEL_MATRIX_UNIFORM,      plMatrix44() ) );
    component.attach( plUniform( PL_VIEW_MATRIX_UNIFORM,       camera       ) );
    component.attach( plUniform( PL_PROJECTION_MATRIX_UNIFORM, ortho        ) );
    // attach texture uniforms
    component.attach( plUniform( PL_TEXTURE_UNIT_0_UNIFORM, &( *fbo->texture2DAttachment( GL_COLOR_ATTACHMENT0 ) ) ) );     // colour buffer
    component.attach( plUniform( PL_TEXTURE_UNIT_1_UNIFORM, &( *fbo->texture2DAttachment( GL_COLOR_ATTACHMENT1 ) ) ) );     // outline buffer
    component.attach( plUniform( PL_TEXTURE_UNIT_2_UNIFORM, &( *fbo->texture2DAttachment( GL_COLOR_ATTACHMENT3 ) ) ) );     // arthro cam buffer

    return component;
}

