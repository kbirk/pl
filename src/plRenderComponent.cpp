#include "plRenderComponent.h"

plRenderComponent::plRenderComponent(std::shared_ptr<plVAO> vao)
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


void plRenderComponent::attach(uint32_t type, std::shared_ptr<plTexture2D> texture)
{
    _textures[type] = texture;
}


void plRenderComponent::draw(const plShader& shader) const
{
    _bindUniforms(shader);
    _bindTextures(shader);
    _vao->draw();
}

void plRenderComponent::_bindTextures(const plShader& shader) const
{
    // set all textures
    for (auto iter : _textures)
    {
        shader.setTexture(iter.first, iter.second);
    }
}

void plRenderComponent::_bindUniforms(const plShader& shader) const
{
    // set all uniforms
    for (auto uniform : _uniforms)
    {
        shader.setUniform(uniform);
    }
}
