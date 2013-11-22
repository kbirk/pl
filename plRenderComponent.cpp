#include "plRenderComponent.h"

       
plRenderComponent::plRenderComponent( const plVAO* vao )
    :   _vao ( vao )
{ 
}



PLbool plRenderComponent::operator< ( const plRenderComponent& rc ) const
{   
    // then who cares    
    return this < &rc;    
}


void plRenderComponent::attach( const plUniform& uniform )
{
    _uniforms.push_back( uniform );
}



void plRenderComponent::draw( const plShader& shader ) const
{
    //_bindFBO();
    //_bindShader();
    _bindUniforms( shader );
    _vao->draw();
}

/*
void plRenderComponent::_bindShader() const
{
    static const plShader* previousShader = NULL;

    // bind shader if needed
    if ( _shader != previousShader )
        _shader->bind();
          
    previousShader = _shader;
}


void plRenderComponent::_bindFBO() const
{
    static const plFBO* previousFBO = NULL;

    // unbind fbo if need
    if ( !_fbo && previousFBO )
    {
        previousFBO->unbind();
    }

    // bind fbo if needed    
    if ( _fbo && _fbo != previousFBO )
    {   
        _fbo->bind();
    }
  
    previousFBO = _fbo;

}
*/

void plRenderComponent::_bindUniforms( const plShader& shader ) const
{
    // set all uniforms
    for ( const plUniform& uniform : _uniforms )
    {
        shader.setUniform( uniform );
    }
}


