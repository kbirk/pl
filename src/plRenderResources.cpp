#include "plRenderResources.h"

namespace plRenderResources
{

    std::map<uint32_t, std::shared_ptr<plFBO>> _fbos;
    std::map<uint32_t, std::shared_ptr<plShader>> _shaders;

    // private function forward declarations
    void _initFBOs(uint32_t width, uint32_t height);
    void _initShaders();


    const std::shared_ptr<plFBO>& fbos(uint32_t id)
    {
        return _fbos[id];
    }


    const std::shared_ptr<plShader>& shaders(uint32_t id)
    {
        return _shaders[id];
    }


    void init()
    {
        _initShaders();
    }


    void reshape(uint32_t width, uint32_t height)
    {
        // just recreate the FBO / textures
        _initFBOs(width, height);
    }


    void _initFBOs(uint32_t width, uint32_t height)
    {
        _fbos.clear();

        // create main fbo
        _fbos[PL_MAIN_FBO] = std::make_shared<plFBO>();

        // color texture
        _fbos[PL_MAIN_FBO]->attach(
            GL_COLOR_ATTACHMENT0,
            std::make_shared<plTexture2D>(width, height, GL_RGBA8,  GL_RGBA, GL_UNSIGNED_BYTE, nullptr));

        // outline texture
        _fbos[PL_MAIN_FBO]->attach(
            GL_COLOR_ATTACHMENT1,
            std::make_shared<plTexture2D>(width, height, GL_RGBA8I,  GL_RGBA_INTEGER, GL_INT, nullptr));

        // **CURRENTLY UN-USED**
        _fbos[PL_MAIN_FBO]->attach(
            GL_COLOR_ATTACHMENT2,
            std::make_shared<plTexture2D>(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));

        // **CURRENTLY UN-USED**
        _fbos[PL_MAIN_FBO]->attach(
            GL_COLOR_ATTACHMENT3,
            std::make_shared<plTexture2D>(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));

        // picking texture
        _fbos[PL_MAIN_FBO]->attach(
            GL_COLOR_ATTACHMENT4,
            std::make_shared<plTexture2D>(width, height, GL_RGBA8I, GL_RGB_INTEGER, GL_INT, nullptr));

        // depth-stencil buffer texture
        _fbos[PL_MAIN_FBO]->attach(
            GL_DEPTH_ATTACHMENT,
            GL_STENCIL_ATTACHMENT,
            std::make_shared<plTexture2D>(width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr));
    }


    void _initShaders()
    {
        // create shader objects
        _shaders[PL_MINIMAL_SHADER] = std::make_shared<plVertexFragmentShader>("./resources/shaders/minimal.vert", "./resources/shaders/minimal.frag");
        _shaders[PL_PHONG_SHADER] = std::make_shared<plVertexFragmentShader>("./resources/shaders/phong.vert", "./resources/shaders/phong.frag");
        _shaders[PL_OUTLINE_SHADER] = std::make_shared<plVertexFragmentShader>("./resources/shaders/outline.vert", "./resources/shaders/outline.frag");
        _shaders[PL_FBO_SHADER] = std::make_shared<plVertexFragmentShader>("./resources/shaders/fbo.vert", "./resources/shaders/fbo.frag");
    }

}
