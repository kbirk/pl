#include "plRenderComponent.h"

plRenderComponent::plRenderComponent(const std::shared_ptr<plVAO>& vao)
    :   _vao (vao)
{
}


bool plRenderComponent::operator< (const plRenderComponent& rc) const
{
    // currently sorts by address
    return this < &rc;
}


void plRenderComponent::attach(const plUniform& uniform)
{
    _uniforms.push_back(uniform);
}


void plRenderComponent::draw(const plShader& shader) const
{

    _bindUniforms(shader);
    _vao->draw();
}


void plRenderComponent::_bindUniforms(const plShader& shader) const
{
    // set all uniforms
    for (const plUniform& uniform : _uniforms)
    {
        shader.setUniform(uniform);
    }
}
