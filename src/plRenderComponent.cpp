#include "plRenderComponent.h"

plRenderComponent::plRenderComponent(std::shared_ptr<plVAO> vao)
    :   _vao (vao)
{
}


void plRenderComponent::attach(uint32_t type, std::shared_ptr<plUniform> uniform)
{
    _uniforms[type] = uniform;
}


void plRenderComponent::attach(uint32_t type, std::shared_ptr<plTexture2D> texture)
{
    _textures[type] = texture;
}


void plRenderComponent::draw(const plShader& shader) const
{
    if (_vao != nullptr)
    {
        _bindUniforms(shader);
        _bindTextures(shader);
        _vao->draw();
    }
    else
    {
        LOG_WARN("Attempting to draw null _vao");
    }
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
    for (auto iter : _uniforms)
    {
        shader.setUniform(iter.first, iter.second);
    }
}
