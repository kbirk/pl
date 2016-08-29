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

    // bind shader
    shader->bind();

    // set viewport
    glViewport(0, 0, plWindow::viewportWidth(), plWindow::viewportHeight());
    LOG_OPENGL("glViewport");

    // set draw buffers
    std::vector<GLenum> drawBuffers;
    drawBuffers.push_back(GL_NONE);
    drawBuffers.push_back(GL_COLOR_ATTACHMENT1);
    drawBuffers.push_back(GL_NONE);
    drawBuffers.push_back(GL_NONE);
    drawBuffers.push_back(GL_NONE);
    fbo->setDrawBuffers(drawBuffers);

    // glEnable(GL_POLYGON_OFFSET_FILL);
    // LOG_OPENGL("glEnable");
    //
    // glPolygonOffset(-1.0, -1.0);
    // LOG_OPENGL("glPolygonOffset");

    // draw main render components
    for (auto component : components)
    {
        component->draw(*shader);
    }

    // glPolygonOffset(0.0, 0.0);
    // LOG_OPENGL("glPolygonOffset");
    //
    // glDisable(GL_POLYGON_OFFSET_FILL);
    // LOG_OPENGL("glDisable");

    // unbind shader
    shader->unbind();

    // unbind fbo
    fbo->unbind();
}
