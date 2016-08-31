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

    // draw main render components
    for (auto component : components)
    {
        component->draw(*shader);
    }

    // re-enable depth testing
    glEnable(GL_DEPTH_TEST);
    LOG_OPENGL("glEnable");

    // unbind shader
    shader->unbind();

    // unbind fbo
    fbo->unbind();
}
