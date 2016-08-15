#pragma once

#include "plCommon.h"
#include "plVector3.h"
#include "plRenderable.h"
#include "plVAO.h"

#define PL_HANDLE_SPHERE_RADIUS                  0.75f
#define PL_HANDLE_RADIUS                         0.3f
#define PL_HANDLE_LENGTH                         2.0f
#define PL_ARROW_LENGTH                          1.0f
#define PL_CIRCLE_LENGTH                         1.0f
#define PL_HEAD_RADIUS                           0.7f
#define PL_SLICE_NUM                             30
#define PL_STACK_NUM                             1

namespace plRenderShapes
{
    plVAO sphereVAO(float32_t radius, int32_t slices, int32_t stacks);
    plVAO cylinderVAO(float32_t baseRadius, float32_t topRadius, float32_t height, int32_t slices, int32_t stacks);
    plVAO diskVAO(float32_t innerRadius, float32_t outerRadius, int32_t slices, int32_t loops, bool up = true);
    plVAO coneVAO(float32_t baseRadius, float32_t topRadius, float32_t height, int32_t slices, int32_t stacks);
    plVAO quadVAO();
    plVAO lineVAO(const plVector3& p0, const plVector3& p1);
}
