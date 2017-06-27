#include "plScreenQuadTechnique.h"

plScreenQuadTechnique::plScreenQuadTechnique()
{
}

void plScreenQuadTechnique::render(const plRenderList& components) const
{
    auto shader = plRenderResources::shaders(PL_FBO_SHADER);

    // bind shader
    shader->bind();

    // clear back buffer
    glClearColor(PL_CLEAR_COLOR);
    LOG_OPENGL("glClearColor");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    LOG_OPENGL("glClear");
    glViewport(plWindow::viewportX(), plWindow::viewportY(), plWindow::viewportWidth(), plWindow::viewportHeight());
    LOG_OPENGL("glViewport");

    // get the quad component
    auto component = _generateComponent();

    // draw the quad component
    component->draw(*shader);

    // unbind shader
    shader->unbind();
}

std::shared_ptr<plVAO> plScreenQuadTechnique::_generateQuad() const
{
    std::vector<plVector3> vertices;
    vertices.reserve(8);

    std::vector<uint32_t> indices;
    indices.reserve(6);

    // position                               // texture coord
    vertices.push_back(plVector3(-1, -1, 0));
    vertices.push_back(plVector3(0, 0, 0));
    vertices.push_back(plVector3(1, -1, 0));
    vertices.push_back(plVector3(1, 0, 0));
    vertices.push_back(plVector3(1, 1, 0));
    vertices.push_back(plVector3(1, 1, 0));
    vertices.push_back(plVector3(-1, 1, 0));
    vertices.push_back(plVector3(0, 1, 0));

    // indices
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);

    // set vbo and attach attribute pointers
    auto vbo = std::make_shared<plVBO>();
    vbo->set(vertices);
    vbo->set(plVertexAttributePointer(PL_POSITION_ATTRIBUTE, 32, 0));
    vbo->set(plVertexAttributePointer(PL_TEXCOORD_ATTRIBUTE, 32, 16));
    // set eabo
    auto eabo = std::make_shared<plEABO>();
    eabo->set(indices);
    // create vao
    auto vao = std::make_shared<plVAO>();
    // attach to vao
    vao->attach(vbo);
    vao->attach(eabo);
    // upload to gpu
    vao->upload();
    return vao;
}

std::shared_ptr<plRenderComponent> plScreenQuadTechnique::_generateComponent() const
{
    static plMatrix44 ortho(
        -1, 1,
        -1, 1,
        -1, 1);

    static plMatrix44 model(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);

    static plMatrix44 camera(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, -1, 0,
        0, 0, 0, 1);

    static std::shared_ptr<plVAO> quad = _generateQuad();

    auto fbo = plRenderResources::fbos(PL_MAIN_FBO);

    auto component = std::make_shared<plRenderComponent>(quad);
    // attach transformation uniforms
    component->attach(PL_MODEL_MATRIX_UNIFORM, std::make_shared<plUniform>(model));
    component->attach(PL_VIEW_MATRIX_UNIFORM, std::make_shared<plUniform>(camera));
    component->attach(PL_PROJECTION_MATRIX_UNIFORM, std::make_shared<plUniform>(ortho));
    // attach texture uniforms
    component->attach(PL_TEXTURE_UNIT_0, fbo->texture2DAttachment(GL_COLOR_ATTACHMENT0)); // color buffer
    component->attach(PL_TEXTURE_UNIT_1, fbo->texture2DAttachment(GL_COLOR_ATTACHMENT1)); // outline buffer

    return component;
}
