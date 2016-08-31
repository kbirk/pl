#include "plTransparencyTechnique.h"

plTransparencyTechnique::plTransparencyTechnique()
{
}


void plTransparencyTechnique::render(const plRenderList& components) const
{
    auto fbo = plRenderResources::fbos(PL_MAIN_FBO);
    auto shader = plRenderResources::shaders(PL_PHONG_SHADER);

    // bind fbo
    fbo->bind();

    // bind shader
    shader->bind();

    // set draw buffers to only write to color output
    std::vector<GLenum> drawBuffers;
    drawBuffers.push_back(GL_COLOR_ATTACHMENT0);
    drawBuffers.push_back(GL_NONE);
    drawBuffers.push_back(GL_NONE);
    drawBuffers.push_back(GL_NONE);
    drawBuffers.push_back(GL_NONE);   // no picking output
    fbo->setDrawBuffers(drawBuffers);

    // set viewport
    glViewport(0, 0, plWindow::viewportWidth(), plWindow::viewportHeight());
    LOG_OPENGL("glViewport");

    // turn off writing to depth buffer
    glDepthMask(false);
    LOG_OPENGL("glDepthMask");

    // draw render components to color buffer
    for (auto component : components)
    {
        component->draw(*shader);
    }

    // set stencil testing for picking buffer
    glEnable(GL_STENCIL_TEST);
    LOG_OPENGL("glEnable");
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    LOG_OPENGL("glStencilFunc");
    glStencilMask(0xFF);
    LOG_OPENGL("glStencilMask");
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    LOG_OPENGL("glStencilOp");

    // set draw buffers to only write to picking buffer
    drawBuffers.clear();
    drawBuffers.push_back(GL_NONE); // no color output
    drawBuffers.push_back(GL_NONE);
    drawBuffers.push_back(GL_NONE);
    drawBuffers.push_back(GL_NONE);
    drawBuffers.push_back(GL_COLOR_ATTACHMENT4);
    fbo->setDrawBuffers(drawBuffers);

    // draw render components to picking buffer
    for (auto component : components)
    {
        component->draw(*shader);
    }

    // disable stencil testing
    glDisable(GL_STENCIL_TEST);
    LOG_OPENGL("glDisable");

    // re-enable writing to depth buffer
    glDepthMask(true);
    LOG_OPENGL("glDepthMask");

    // unbind shader
    shader->unbind();
    // unbind fbo
    fbo->unbind();
}
