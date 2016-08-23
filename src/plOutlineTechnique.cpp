#include "plOutlineTechnique.h"

plOutlineTechnique::plOutlineTechnique()
{
}


void plOutlineTechnique::render(const plRenderList& components) const
{
    auto fbo = plRenderResources::fbos(PL_MAIN_FBO);
    auto shader = plRenderResources::shaders(PL_OUTLINE_SHADER);

    // bind fbo
    fbo->bind();

    // set viewport
    glViewport(0, 0, plWindow::viewportWidth(), plWindow::viewportHeight());

    std::vector<GLenum> drawBuffers;
    drawBuffers.push_back(GL_NONE);
    drawBuffers.push_back(GL_COLOR_ATTACHMENT1);
    drawBuffers.push_back(GL_NONE);
    drawBuffers.push_back(GL_NONE);
    drawBuffers.push_back(GL_NONE);
    fbo->setDrawBuffers(drawBuffers);

    // bind shader
    shader->bind();

    glDepthFunc(GL_LEQUAL);

    // draw shapes to outline buffer
    for (auto component : components)
    {
        component->draw(*shader);
    }

    // unbind shader
    shader->unbind();
    // unbind fbo
    fbo->unbind();
}
