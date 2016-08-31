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
    std::shared_ptr<plVAO> sphereVAO(float32_t radius, int32_t slices, int32_t stacks);
    std::shared_ptr<plVAO> cylinderVAO(float32_t baseRadius, float32_t topRadius, float32_t height, int32_t slices, int32_t stacks);
    std::shared_ptr<plVAO> diskVAO(float32_t innerRadius, float32_t outerRadius, int32_t slices, int32_t loops, bool up = true);
    std::shared_ptr<plVAO> coneVAO(float32_t baseRadius, float32_t topRadius, float32_t height, int32_t slices, int32_t stacks);
    std::shared_ptr<plVAO> quadVAO();
    std::shared_ptr<plVAO> lineVAO(const plVector3& p0, const plVector3& p1);
}
