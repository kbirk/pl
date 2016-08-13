#include "plRenderResources.h"

namespace plRenderResources
{

    std::map<uint32_t, std::shared_ptr<plFBO> >    _fbos;
    std::map<uint32_t, std::shared_ptr<plShader>> _shaders;

    // private function forward declarations
    void _initFBOs(uint32_t width, uint32_t height);
    void _initShaders();


    const std::shared_ptr<plFBO >& fbos(uint32_t id)
    {
        return _fbos[id];
    }


    const std::shared_ptr<plShader >& shaders(uint32_t id)
    {
        return _shaders[id];
    }


    void init()
    {
        _initFBOs(1, 1);
        _initShaders();
    }


    void reshape(uint32_t width, uint32_t height)
    {
        _initFBOs(width, height);
    }


    void _initFBOs(uint32_t width, uint32_t height)
    {
        _fbos.clear();

        // create main fbo
        _fbos[PL_MAIN_FBO] = std::shared_ptr<plFBO>(new plFBO());

        // main render buffer
        _fbos[PL_MAIN_FBO]->attach(
            GL_COLOR_ATTACHMENT0,
            std::shared_ptr<plTexture2D>(new plTexture2D(width, height, GL_RGBA8,  GL_RGBA, GL_UNSIGNED_BYTE, nullptr)));

        // texture for outlining
        _fbos[PL_MAIN_FBO]->attach(
            GL_COLOR_ATTACHMENT1,
            std::shared_ptr<plTexture2D>(new plTexture2D(width, height, GL_RGBA32I,  GL_RGBA_INTEGER, GL_INT, nullptr)));

        // **CURRENTLY UNUSED**
        _fbos[PL_MAIN_FBO]->attach(
            GL_COLOR_ATTACHMENT2,
            std::shared_ptr<plTexture2D>(new plTexture2D(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, nullptr)));

        // **CURRENTLY UNUSED**
        _fbos[PL_MAIN_FBO]->attach(
            GL_COLOR_ATTACHMENT3,
            std::shared_ptr<plTexture2D>(new plTexture2D(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, nullptr)));

        // colour picking texture
        _fbos[PL_MAIN_FBO]->attach(
            GL_COLOR_ATTACHMENT4,
            std::shared_ptr<plTexture2D>(new plTexture2D(width, height, GL_RGB32I, GL_RGB_INTEGER, GL_INT, nullptr)));

        // depth-stencil buffer texture
        _fbos[PL_MAIN_FBO]->attach(
            GL_DEPTH_ATTACHMENT,
            GL_STENCIL_ATTACHMENT,
            std::shared_ptr<plTexture2D>(new plTexture2D(width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr)));
    }


    void _initShaders()
    {
        // create shader objects
        _shaders[PL_MINIMAL_SHADER] = std::shared_ptr<plVertexFragmentShader>(new plVertexFragmentShader(PL_FILE_PREPATH"shaders/minimal.vert", PL_FILE_PREPATH"shaders/minimal.frag"));
        _shaders[PL_PHONG_SHADER] = std::shared_ptr<plVertexFragmentShader>(new plVertexFragmentShader(PL_FILE_PREPATH"shaders/phong.vert",   PL_FILE_PREPATH"shaders/phong.frag"));
        _shaders[PL_OUTLINE_SHADER] = std::shared_ptr<plVertexFragmentShader>(new plVertexFragmentShader(PL_FILE_PREPATH"shaders/outline.vert", PL_FILE_PREPATH"shaders/outline.frag"));
        _shaders[PL_ARTHRO_CAM_SHADER] = std::shared_ptr<plVertexFragmentShader>(new plVertexFragmentShader(PL_FILE_PREPATH"shaders/arthro.vert",  PL_FILE_PREPATH"shaders/arthro.frag"));
        _shaders[PL_FBO_SHADER] = std::shared_ptr<plVertexFragmentShader>(new plVertexFragmentShader(PL_FILE_PREPATH"shaders/fbo.vert",     PL_FILE_PREPATH"shaders/fbo.frag"));
    }

}
