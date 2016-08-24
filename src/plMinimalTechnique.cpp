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

    // bind shader
    shader->bind();

    glDepthFunc(GL_ALWAYS);
    LOG_OPENGL("glDepthFunc");

    // draw main render components
    for (auto component : components)
    {
        component->draw(*shader);
    }

    glDepthFunc(GL_LEQUAL);
    LOG_OPENGL("glDepthFunc");

    // unbind shader
    shader->unbind();

    // unbind fbo
    fbo->unbind();
}
