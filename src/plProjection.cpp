#include "plProjection.h"

plProjection::plProjection(float32_t aspectRatio)
    :   _fov(7.0f), _aspect(aspectRatio), _nearPlane(100.0f), _farPlane(15000.0f)
{
}

plProjection::plProjection(float32_t fov, float32_t aspectRatio, float32_t nearPlane, float32_t farPlane)
    :   _fov(fov), _aspect(aspectRatio), _nearPlane(nearPlane), _farPlane(farPlane)
{
}
