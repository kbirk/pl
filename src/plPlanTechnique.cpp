#include "plPlanTechnique.h"

plPlanTechnique::plPlanTechnique()
{
}

void plPlanTechnique::render(const plRenderList& components) const
{
    auto fbo = plRenderResources::fbos(PL_MAIN_FBO);
    auto shader = plRenderResources::shaders(PL_PHONG_SHADER);

    // bind fbo
    fbo->bind();
    // bind shader
    shader->bind();

    // set all draw buffers
    std::vector<GLenum> drawBuffers;
    drawBuffers.push_back(GL_COLOR_ATTACHMENT0);
    drawBuffers.push_back(GL_COLOR_ATTACHMENT1);
    drawBuffers.push_back(GL_COLOR_ATTACHMENT2);
    drawBuffers.push_back(GL_COLOR_ATTACHMENT3);
    drawBuffers.push_back(GL_COLOR_ATTACHMENT4);
    fbo->setDrawBuffers(drawBuffers);

    glViewport(0, 0, plWindow::viewportWidth(), plWindow::viewportHeight());
    LOG_OPENGL("glViewport");

    // clear fbo before individual draw buffers are set
    glClearColor(0, 0, 0, 0);
    LOG_OPENGL("glClearColor");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    LOG_OPENGL("glClear");

    // disable face culling
    glDisable(GL_CULL_FACE);
    LOG_OPENGL("glDisable");

    // set draw buffers
    drawBuffers.clear();
    drawBuffers.push_back(GL_COLOR_ATTACHMENT0);
    drawBuffers.push_back(GL_NONE);
    drawBuffers.push_back(GL_NONE);
    drawBuffers.push_back(GL_NONE);
    drawBuffers.push_back(GL_COLOR_ATTACHMENT4);
    fbo->setDrawBuffers(drawBuffers);

    // set stencil testing to write 1's wherever is rendered, this is later
    // used in transparency shader to ensure proper picking in transparent areas
    glEnable(GL_STENCIL_TEST);
    LOG_OPENGL("glEnable");
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    LOG_OPENGL("glStencilFunc");
    glStencilMask(0xFF);
    LOG_OPENGL("glStencilMask");
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    LOG_OPENGL("glStencilOp");

    // draw main render components
    for (auto component : components) {
        component->draw(*shader);
    }

    // disable depth testing
    glDisable(GL_STENCIL_TEST);
    LOG_OPENGL("glDisable");

    // re-enable face culling
    glEnable(GL_CULL_FACE);
    LOG_OPENGL("glEnable");

    // unbind shader
    shader->unbind();

    // unbind fbo
    fbo->unbind();
}
