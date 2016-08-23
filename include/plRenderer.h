#pragma once

#include "plCommon.h"
#include "plVector3.h"
#include "plRenderingPipeline.h"
#include "plRenderable.h"
#include "plRenderComponent.h"
#include "plFBO.h"
#include "plPlanTechnique.h"
#include "plScreenQuadTechnique.h"
#include "plOutlineTechnique.h"
#include "plTransparencyTechnique.h"
#include "plMinimalTechnique.h"
#include "plRenderShapes.h"

enum plTechniqueEnums       // ORDER MATTERS! std::map iterators are sorted by key value
{
    PL_PLAN_TECHNIQUE = 1,
    PL_MINIMAL_TECHNIQUE,
    PL_TRANSPARENCY_TECHNIQUE,
    PL_OUTLINE_TECHNIQUE,
    PL_SCREEN_QUAD_TECHNIQUE
};

typedef std::map<uint32_t, std::shared_ptr<plRenderTechnique>> plTechniqueMap;

namespace plRenderer
{
    void init();
    void queue(const plRenderable& renderable);
    void draw();

    void queueSphere(uint32_t technique, const plVector3& position, float32_t radius);
    void queueCylinder(uint32_t technique, const plVector3& position, const plVector3& direction, float32_t radius, float32_t length);
    void queueDisk(uint32_t technique, const plVector3& position, const plVector3& direction, float32_t radius, bool flip = false);
    void queueCone(uint32_t technique, const plVector3& position, const plVector3& direction, float32_t topRadius, float32_t bottomRadius, float32_t length);
    void queueArrow(uint32_t technique, const plVector3& position, const plVector3 &direction, float32_t length = 2.0f, float32_t scale = 1.0f);
    void queueAxis(uint32_t technique, const plVector3& position, const plVector3& x, const plVector3& y, const float32_t scale = 1.0f);
    void queuePlane(uint32_t technique, const plVector3& position, const plVector3& normal, float32_t scale = 1.0f);
    void queueLine(uint32_t technique, const plVector3& p0, const plVector3& p1);
}
