#include "plMinimalTechnique.h"

plMinimalTechnique::plMinimalTechnique()
{
}


void plMinimalTechnique::render(const plRenderList& components) const
{
    auto fbo = plRenderResources::fbos(PL_MAIN_FBO);
    auto shader = plRenderResources::shaders(PL_MINIMAL_SHADER);

    // bind fbo
    fbo->bind();

    // bind shader
    shader->bind();

    // set viewport
    glViewport(0, 0, plWindow::viewportWidth(), plWindow::viewportHeight());
    LOG_OPENGL("glViewport");

    // set draw buffers
    std::vector<GLenum> drawBuffers;
    drawBuffers.push_back(GL_COLOR_ATTACHMENT0);
    drawBuffers.push_back(GL_NONE);
    drawBuffers.push_back(GL_NONE);
    drawBuffers.push_back(GL_NONE);
    drawBuffers.push_back(GL_COLOR_ATTACHMENT4);
    fbo->setDrawBuffers(drawBuffers);

    // disable depth testing
    glDisable(GL_DEPTH_TEST);
    LOG_OPENGL("glDisable");

    // turn off writing to depth buffer
    glDepthMask(false);
    LOG_OPENGL("glDepthMask");

    // draw main render components
    for (auto component : components)
    {
        component->draw(*shader);
    }

    // re-enable writing to depth buffer
    glDepthMask(true);
    LOG_OPENGL("glDepthMask");

    // re-enable depth testing
    glEnable(GL_DEPTH_TEST);
    LOG_OPENGL("glEnable");

    // unbind shader
    shader->unbind();

    // unbind fbo
    fbo->unbind();
}
