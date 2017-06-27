#include "plRenderer.h"

namespace plRenderer {

plRenderMap _renderMap;
plTechniqueMap _techniques;

void init()
{
    // set initial opengl state

    // enable back face culling
    glEnable(GL_CULL_FACE);
    LOG_OPENGL("glEnable");
    glCullFace(GL_BACK);
    LOG_OPENGL("glCullFace");
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    LOG_OPENGL("glPolygonMode");
    // set depth testing
    glEnable(GL_DEPTH_TEST);
    LOG_OPENGL("glEnable");
    glDepthFunc(GL_LEQUAL);
    LOG_OPENGL("glDepthFunc");
    // enable blending
    glEnable(GL_BLEND);
    LOG_OPENGL("glEnable");
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    LOG_OPENGL("glBlendFunc");

    // create techniques
    _techniques[PL_PLAN_TECHNIQUE] = std::make_shared<plPlanTechnique>();
    _techniques[PL_OUTLINE_TECHNIQUE] = std::make_shared<plOutlineTechnique>();
    _techniques[PL_SCREEN_QUAD_TECHNIQUE] = std::make_shared<plScreenQuadTechnique>();
    _techniques[PL_TRANSPARENCY_TECHNIQUE] = std::make_shared<plTransparencyTechnique>();
    _techniques[PL_MINIMAL_TECHNIQUE] = std::make_shared<plMinimalTechnique>();
}

void queue(const plRenderable& renderable)
{
    // it would be nice to have each technique contain its own set of rc's
    renderable.extractRenderComponents(_renderMap);
}

void draw()
{
    for (auto pair : _techniques) {
        // get technique ptr and enum
        uint32_t techniqueEnum = pair.first;
        auto technique = pair.second;
        technique->render(_renderMap[techniqueEnum]);
    }

    // clear map for this frame
    _renderMap.clear();
}

void queueSphere(uint32_t technique, const plVector3& position, float32_t radius)
{
    static std::shared_ptr<plVAO> vao = plRenderShapes::sphereVAO(1.0f, 20, 20);

    plModelStack::push();
    plModelStack::translate(position); // transform
    plModelStack::scale(radius); // scale

    auto component = std::make_shared<plRenderComponent>(vao);

    component->attach(PL_MODEL_MATRIX_UNIFORM, std::make_shared<plUniform>(plModelStack::top()));
    component->attach(PL_VIEW_MATRIX_UNIFORM, std::make_shared<plUniform>(plCameraStack::top()));
    component->attach(PL_PROJECTION_MATRIX_UNIFORM, std::make_shared<plUniform>(plProjectionStack::top()));
    component->attach(PL_COLOR_UNIFORM, std::make_shared<plUniform>(plColorStack::top()));
    component->attach(PL_PICKING_UNIFORM, std::make_shared<plUniform>(plPickingStack::top()));
    component->attach(PL_LIGHT_POSITION_UNIFORM, std::make_shared<plUniform>(plVector3(PL_LIGHT_POSITION)));

    _renderMap[technique].push_back(component);

    plModelStack::pop();
}

void queueCylinder(uint32_t technique, const plVector3& position, const plVector3& direction, float32_t radius, float32_t length)
{
    static std::shared_ptr<plVAO> vao = plRenderShapes::cylinderVAO(1.0f, 1.0f, 1.0f, 30, 1);

    plMatrix44 rot;
    rot.setRotation(plVector3(0, 0, 1), direction.normalize());

    plModelStack::push();
    plModelStack::translate(position);
    plModelStack::mult(rot);
    plModelStack::scale(radius, radius, length);

    auto component = std::make_shared<plRenderComponent>(vao);

    component->attach(PL_MODEL_MATRIX_UNIFORM, std::make_shared<plUniform>(plModelStack::top()));
    component->attach(PL_VIEW_MATRIX_UNIFORM, std::make_shared<plUniform>(plCameraStack::top()));
    component->attach(PL_PROJECTION_MATRIX_UNIFORM, std::make_shared<plUniform>(plProjectionStack::top()));
    component->attach(PL_COLOR_UNIFORM, std::make_shared<plUniform>(plColorStack::top()));
    component->attach(PL_PICKING_UNIFORM, std::make_shared<plUniform>(plPickingStack::top()));
    component->attach(PL_LIGHT_POSITION_UNIFORM, std::make_shared<plUniform>(plVector3(PL_LIGHT_POSITION)));

    _renderMap[technique].push_back(component);

    plModelStack::pop();
}

void queueDisk(uint32_t technique, const plVector3& position, const plVector3& direction, float32_t radius, bool flip)
{
    static std::shared_ptr<plVAO> vao = plRenderShapes::diskVAO(0.0f, 1.0f, 30, 30);

    plMatrix44 rot;
    rot.setRotation(plVector3(0, 0, 1), direction.normalize());

    plModelStack::push();
    plModelStack::translate(position);
    plModelStack::mult(rot);

    if (flip)
        plModelStack::rotate(180.0f, plVector3(1, 0, 0));

    plModelStack::scale(radius);

    // create render component
    auto component = std::make_shared<plRenderComponent>(vao);
    // attached uniforms
    component->attach(PL_MODEL_MATRIX_UNIFORM, std::make_shared<plUniform>(plModelStack::top()));
    component->attach(PL_VIEW_MATRIX_UNIFORM, std::make_shared<plUniform>(plCameraStack::top()));
    component->attach(PL_PROJECTION_MATRIX_UNIFORM, std::make_shared<plUniform>(plProjectionStack::top()));
    component->attach(PL_COLOR_UNIFORM, std::make_shared<plUniform>(plColorStack::top()));
    component->attach(PL_PICKING_UNIFORM, std::make_shared<plUniform>(plPickingStack::top()));
    component->attach(PL_LIGHT_POSITION_UNIFORM, std::make_shared<plUniform>(plVector3(PL_LIGHT_POSITION)));
    // insert into render map
    _renderMap[technique].push_back(component);

    plModelStack::pop();
}

void queueCone(uint32_t technique, const plVector3& position, const plVector3& direction, float32_t topRadius, float32_t bottomRadius, float32_t length)
{
    // can't use static for cones as normals scale inversely
    std::shared_ptr<plVAO> vao = plRenderShapes::coneVAO(bottomRadius, topRadius, length, 30, 1);

    plMatrix44 rot;
    rot.setRotation(plVector3(0, 0, 1), direction.normalize());

    plModelStack::push();
    plModelStack::translate(position);
    plModelStack::mult(rot);

    // create render component
    auto component = std::make_shared<plRenderComponent>(vao);
    // attached uniforms
    component->attach(PL_MODEL_MATRIX_UNIFORM, std::make_shared<plUniform>(plModelStack::top()));
    component->attach(PL_VIEW_MATRIX_UNIFORM, std::make_shared<plUniform>(plCameraStack::top()));
    component->attach(PL_PROJECTION_MATRIX_UNIFORM, std::make_shared<plUniform>(plProjectionStack::top()));
    component->attach(PL_COLOR_UNIFORM, std::make_shared<plUniform>(plColorStack::top()));
    component->attach(PL_PICKING_UNIFORM, std::make_shared<plUniform>(plPickingStack::top()));
    component->attach(PL_LIGHT_POSITION_UNIFORM, std::make_shared<plUniform>(plVector3(PL_LIGHT_POSITION)));
    // insert into render map
    _renderMap[technique].push_back(component);

    plModelStack::pop();
}

void queueArrow(uint32_t technique, const plVector3& position, const plVector3& direction, float32_t length, float32_t scale)
{
    // can't use static for cones as normals scale inversely,
    std::shared_ptr<plVAO> vao = plRenderShapes::coneVAO(PL_HEAD_RADIUS, 0.0f, PL_ARROW_LENGTH, 30, 1);

    plMatrix44 rot;
    rot.setRotation(plVector3(0, 0, 1), direction.normalize());

    plModelStack::push();

    plModelStack::translate(position);
    plModelStack::mult(rot);
    plModelStack::scale(scale);
    queueCylinder(technique, plVector3(0, 0, 0), plVector3(0, 0, 1), PL_HANDLE_RADIUS, length / scale);
    queueDisk(technique, plVector3(0, 0, 0), plVector3(0, 0, 1), PL_HANDLE_RADIUS, true);

    plModelStack::translate(0, 0, length / scale);

    // create render component
    auto component = std::make_shared<plRenderComponent>(vao);
    // attached uniforms
    component->attach(PL_MODEL_MATRIX_UNIFORM, std::make_shared<plUniform>(plModelStack::top()));
    component->attach(PL_VIEW_MATRIX_UNIFORM, std::make_shared<plUniform>(plCameraStack::top()));
    component->attach(PL_PROJECTION_MATRIX_UNIFORM, std::make_shared<plUniform>(plProjectionStack::top()));
    component->attach(PL_COLOR_UNIFORM, std::make_shared<plUniform>(plColorStack::top()));
    component->attach(PL_PICKING_UNIFORM, std::make_shared<plUniform>(plPickingStack::top()));
    component->attach(PL_LIGHT_POSITION_UNIFORM, std::make_shared<plUniform>(plVector3(PL_LIGHT_POSITION)));
    // insert into render map
    _renderMap[technique].push_back(component);

    queueDisk(technique, plVector3(0, 0, 0), plVector3(0, 0, 1), PL_HEAD_RADIUS, true);

    plModelStack::pop();
}

void queueAxis(uint32_t technique, const plVector3& position, const plVector3& x, const plVector3& y, const float32_t scale)
{
    plModelStack::push();

    // draw x
    plColorStack::load(PL_X_AXIS_COLOR);
    queueArrow(technique, position, x, PL_HANDLE_LENGTH * scale, scale);

    // draw y
    plColorStack::load(PL_Y_AXIS_COLOR);
    queueArrow(technique, position, y, PL_HANDLE_LENGTH * scale, scale);

    // draw z
    plColorStack::load(PL_Z_AXIS_COLOR);
    queueArrow(technique, position, x ^ y, PL_HANDLE_LENGTH * scale, scale);

    plModelStack::pop();
}

void queuePlane(uint32_t technique, const plVector3& position, const plVector3& normal, float32_t scale)
{
    static std::shared_ptr<plVAO> vao = plRenderShapes::quadVAO();

    plMatrix44 rot;
    rot.setRotation(plVector3(0, 0, 1), normal.normalize());

    plModelStack::push();
    plModelStack::translate(position);
    plModelStack::mult(rot);
    plModelStack::scale(scale);

    // create render component
    auto component = std::make_shared<plRenderComponent>(vao);
    // attached uniforms
    component->attach(PL_MODEL_MATRIX_UNIFORM, std::make_shared<plUniform>(plModelStack::top()));
    component->attach(PL_VIEW_MATRIX_UNIFORM, std::make_shared<plUniform>(plCameraStack::top()));
    component->attach(PL_PROJECTION_MATRIX_UNIFORM, std::make_shared<plUniform>(plProjectionStack::top()));
    component->attach(PL_COLOR_UNIFORM, std::make_shared<plUniform>(plColorStack::top()));
    component->attach(PL_PICKING_UNIFORM, std::make_shared<plUniform>(plPickingStack::top()));
    component->attach(PL_LIGHT_POSITION_UNIFORM, std::make_shared<plUniform>(plVector3(PL_LIGHT_POSITION)));
    // insert into render map
    _renderMap[technique].push_back(component);

    plModelStack::pop();
}

void queueLine(uint32_t technique, const plVector3& p0, const plVector3& p1)
{
    std::shared_ptr<plVAO> vao = plRenderShapes::lineVAO(p0, p1);

    // create render component
    auto component = std::make_shared<plRenderComponent>(vao);
    // attached uniforms
    component->attach(PL_MODEL_MATRIX_UNIFORM, std::make_shared<plUniform>(plModelStack::top()));
    component->attach(PL_VIEW_MATRIX_UNIFORM, std::make_shared<plUniform>(plCameraStack::top()));
    component->attach(PL_PROJECTION_MATRIX_UNIFORM, std::make_shared<plUniform>(plProjectionStack::top()));
    component->attach(PL_COLOR_UNIFORM, std::make_shared<plUniform>(plColorStack::top()));
    component->attach(PL_PICKING_UNIFORM, std::make_shared<plUniform>(plPickingStack::top()));
    component->attach(PL_LIGHT_POSITION_UNIFORM, std::make_shared<plUniform>(plVector3(PL_LIGHT_POSITION)));
    // insert into render map
    _renderMap[technique].push_back(component);
}
}
