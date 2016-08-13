#include "plRenderer.h"

namespace plRenderer
{

    plRenderMap    _renderMap;
    plTechniqueMap _techniques;


    void init()
    {
        // set initial opengl state

        // enable back face culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // set depth testing
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        // enable blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // create techniques
        _techniques[ PL_PLAN_TECHNIQUE ]         = std::make_shared<plPlanTechnique>();
        _techniques[ PL_OUTLINE_TECHNIQUE ]      = std::make_shared<plOutlineTechnique>();
        _techniques[ PL_SCREEN_QUAD_TECHNIQUE ]  = std::make_shared<plScreenQuadTechnique>();
        _techniques[ PL_ARTHRO_CAM_TECHNIQUE ]   = std::make_shared<plArthroCamTechnique>();
        _techniques[ PL_TRANSPARENCY_TECHNIQUE ] = std::make_shared<plTransparencyTechnique>();
        _techniques[ PL_MINIMAL_TECHNIQUE ]      = std::make_shared<plMinimalTechnique>();
    }


    void queue(const plRenderable& renderable)
    {
        // it would be nice to have each technique contain its own set of rc's
        renderable.extractRenderComponents(_renderMap);
    }


    void draw()
    {
        for (auto& pair : _techniques)
        {
            // get technique ptr and enum
            PLuint techniqueEnum = pair.first;
            auto&  technique = pair.second;
            technique->render(_renderMap[ techniqueEnum ]);
        }

        // clear map for this frame
        _renderMap.clear();
    }


    void queueSphere(PLuint technique, const plVector3& position, PLfloat radius)
    {
        static std::shared_ptr<plVAO> vao =  std::make_shared<plVAO>(plRenderShapes::sphereVAO(1.0f, 20, 20));

        plModelStack::push();
        plModelStack::translate(position);   // transform
        plModelStack::scale(radius);         // scale

        plRenderComponent component(vao);

        component.attach(plUniform(PL_MODEL_MATRIX_UNIFORM,      plModelStack::top()));
        component.attach(plUniform(PL_VIEW_MATRIX_UNIFORM,       plCameraStack::top()));
        component.attach(plUniform(PL_PROJECTION_MATRIX_UNIFORM, plProjectionStack::top()));
        component.attach(plUniform(PL_COLOUR_UNIFORM,            plColourStack::top()));
        component.attach(plUniform(PL_PICKING_UNIFORM,           plPickingStack::top()));
        component.attach(plUniform(PL_LIGHT_POSITION_UNIFORM,    plVector3(PL_LIGHT_POSITION)));

        _renderMap[ technique ].insert(component);

        plModelStack::pop();
    }


    void queueCylinder(PLuint technique, const plVector3& position, const plVector3& direction, PLfloat radius, PLfloat length)
    {
        static std::shared_ptr<plVAO> vao = std::make_shared<plVAO>(plRenderShapes::cylinderVAO(1.0f, 1.0f, 1.0f, 30, 1));

        plMatrix44 rot; rot.setRotation(plVector3(0, 0, 1), direction.normalize());

        plModelStack::push();
        plModelStack::translate(position);
        plModelStack::mult(rot);
        plModelStack::scale(radius, radius, length);

        plRenderComponent component(vao);

        component.attach(plUniform(PL_MODEL_MATRIX_UNIFORM,      plModelStack::top()));
        component.attach(plUniform(PL_VIEW_MATRIX_UNIFORM,       plCameraStack::top()));
        component.attach(plUniform(PL_PROJECTION_MATRIX_UNIFORM, plProjectionStack::top()));
        component.attach(plUniform(PL_COLOUR_UNIFORM,            plColourStack::top()));
        component.attach(plUniform(PL_PICKING_UNIFORM,           plPickingStack::top()));
        component.attach(plUniform(PL_LIGHT_POSITION_UNIFORM,    plVector3(PL_LIGHT_POSITION)));

        _renderMap[ technique ].insert(component);

        plModelStack::pop();
    }


    void queueDisk(PLuint technique, const plVector3& position, const plVector3& direction, PLfloat radius, PLbool flip)
    {
        static std::shared_ptr<plVAO> vao = std::make_shared<plVAO>(plRenderShapes::diskVAO(0.0f, 1.0f, 30, 30));

        plMatrix44 rot; rot.setRotation(plVector3(0, 0, 1), direction.normalize());

        plModelStack::push();
        plModelStack::translate(position);
        plModelStack::mult(rot);

        if (flip)
            plModelStack::rotate(180.0f, plVector3(1,0,0));

        plModelStack::scale(radius);

        // create render component
        plRenderComponent component(vao);
        // attached uniforms
        component.attach(plUniform(PL_MODEL_MATRIX_UNIFORM,      plModelStack::top()));
        component.attach(plUniform(PL_VIEW_MATRIX_UNIFORM,       plCameraStack::top()));
        component.attach(plUniform(PL_PROJECTION_MATRIX_UNIFORM, plProjectionStack::top()));
        component.attach(plUniform(PL_COLOUR_UNIFORM,            plColourStack::top()));
        component.attach(plUniform(PL_PICKING_UNIFORM,           plPickingStack::top()));
        component.attach(plUniform(PL_LIGHT_POSITION_UNIFORM,    plVector3(PL_LIGHT_POSITION)));
        // insert into render map
        _renderMap[ technique ].insert(component);

        plModelStack::pop();
    }


    void queueCone(PLuint technique, const plVector3& position, const plVector3& direction, PLfloat topRadius, PLfloat bottomRadius, PLfloat length)
    {
        // can't use static for cones as normals scale inversely,
        std::shared_ptr<plVAO > vao = std::make_shared<plVAO>(plRenderShapes::coneVAO(bottomRadius, topRadius, length, 30, 1));

        plMatrix44 rot; rot.setRotation(plVector3(0, 0, 1), direction.normalize());

        plModelStack::push();
        plModelStack::translate(position);
        plModelStack::mult(rot);

        // create render component
        plRenderComponent component(vao);
        // attached uniforms
        component.attach(plUniform(PL_MODEL_MATRIX_UNIFORM,      plModelStack::top()));
        component.attach(plUniform(PL_VIEW_MATRIX_UNIFORM,       plCameraStack::top()));
        component.attach(plUniform(PL_PROJECTION_MATRIX_UNIFORM, plProjectionStack::top()));
        component.attach(plUniform(PL_COLOUR_UNIFORM,            plColourStack::top()));
        component.attach(plUniform(PL_PICKING_UNIFORM,           plPickingStack::top()));
        component.attach(plUniform(PL_LIGHT_POSITION_UNIFORM,    plVector3(PL_LIGHT_POSITION)));
        // insert into render map
        _renderMap[ technique ].insert(component);

        plModelStack::pop();
    }


    void queueArrow(PLuint technique, const plVector3& position, const plVector3 &direction, PLfloat length, PLfloat scale)
    {
        // can't use static for cones as normals scale inversely,
        std::shared_ptr<plVAO > vao = std::make_shared<plVAO>(plRenderShapes::coneVAO(PL_HEAD_RADIUS, 0.0f, PL_ARROW_LENGTH, 30, 1));

        plMatrix44 rot; rot.setRotation(plVector3(0,0,1), direction.normalize());

        plModelStack::push();

        plModelStack::translate(position);
        plModelStack::mult(rot);
        plModelStack::scale(scale);
        queueCylinder(technique, plVector3(0, 0, 0), plVector3(0, 0, 1), PL_HANDLE_RADIUS, length/scale);
        queueDisk(technique, plVector3(0, 0, 0), plVector3(0, 0, 1), PL_HANDLE_RADIUS, true);

        plModelStack::translate(0, 0, length/scale);

         // create render component
        plRenderComponent component(vao);
        // attached uniforms
        component.attach(plUniform(PL_MODEL_MATRIX_UNIFORM,      plModelStack::top()));
        component.attach(plUniform(PL_VIEW_MATRIX_UNIFORM,       plCameraStack::top()));
        component.attach(plUniform(PL_PROJECTION_MATRIX_UNIFORM, plProjectionStack::top()));
        component.attach(plUniform(PL_COLOUR_UNIFORM,            plColourStack::top()));
        component.attach(plUniform(PL_PICKING_UNIFORM,           plPickingStack::top()));
        component.attach(plUniform(PL_LIGHT_POSITION_UNIFORM,    plVector3(PL_LIGHT_POSITION)));
        // insert into render map
        _renderMap[ technique ].insert(component);

        queueDisk(technique, plVector3(0, 0, 0), plVector3(0, 0, 1), PL_HEAD_RADIUS, true);

        plModelStack::pop();
    }


    void queueAxis(PLuint technique, const plVector3& position, const plVector3& x, const plVector3& y, const PLfloat scale)
    {
        plModelStack::push();

        // draw x
        plColourStack::load(PL_X_AXIS_COLOUR);
        queueArrow(technique, position, x, PL_HANDLE_LENGTH*scale, scale);

        // draw y
        plColourStack::load(PL_Y_AXIS_COLOUR);
        queueArrow(technique, position, y, PL_HANDLE_LENGTH*scale, scale);

        // draw z
        plColourStack::load(PL_Z_AXIS_COLOUR);
        queueArrow(technique, position, x ^ y,  PL_HANDLE_LENGTH*scale, scale);

        plModelStack::pop();
    }


    void queuePlane(PLuint technique, const plVector3& position, const plVector3& normal, PLfloat scale)
    {
        static std::shared_ptr<plVAO > vao = std::make_shared<plVAO>(plRenderShapes::quadVAO());

        plMatrix44 rot; rot.setRotation(plVector3(0, 0, 1), normal.normalize());

        plModelStack::push();
        plModelStack::translate(position);
        plModelStack::mult(rot);
        plModelStack::scale(scale);

        // create render component
        plRenderComponent component(vao);
        // attached uniforms
        component.attach(plUniform(PL_MODEL_MATRIX_UNIFORM,      plModelStack::top()));
        component.attach(plUniform(PL_VIEW_MATRIX_UNIFORM,       plCameraStack::top()));
        component.attach(plUniform(PL_PROJECTION_MATRIX_UNIFORM, plProjectionStack::top()));
        component.attach(plUniform(PL_COLOUR_UNIFORM,            plColourStack::top()));
        component.attach(plUniform(PL_PICKING_UNIFORM,           plPickingStack::top()));
        component.attach(plUniform(PL_LIGHT_POSITION_UNIFORM,    plVector3(PL_LIGHT_POSITION)));
        // insert into render map
        _renderMap[ technique ].insert(component);

        plModelStack::pop();
    }


    void queueLine(PLuint technique, const plVector3& p0, const plVector3& p1)
    {
        std::shared_ptr<plVAO > vao = std::make_shared<plVAO>(plRenderShapes::lineVAO(p0, p1));

        // create render component
        plRenderComponent component(vao);
        // attached uniforms
        component.attach(plUniform(PL_MODEL_MATRIX_UNIFORM,      plModelStack::top()));
        component.attach(plUniform(PL_VIEW_MATRIX_UNIFORM,       plCameraStack::top()));
        component.attach(plUniform(PL_PROJECTION_MATRIX_UNIFORM, plProjectionStack::top()));
        component.attach(plUniform(PL_COLOUR_UNIFORM,            plColourStack::top()));
        component.attach(plUniform(PL_PICKING_UNIFORM,           plPickingStack::top()));
        component.attach(plUniform(PL_LIGHT_POSITION_UNIFORM,    plVector3(PL_LIGHT_POSITION)));
        // insert into render map
        _renderMap[ technique ].insert(component);

    }
}
