#include "plProjection.h"

plProjection::plProjection(PLfloat aspectRatio) 
    :   _fov(7.0f), _aspect(aspectRatio), _nearPlane(100.0f), _farPlane(15000.0f)
{
}

plProjection::plProjection(PLfloat fov, PLfloat aspectRatio, PLfloat nearPlane, PLfloat farPlane)
    :   _fov(fov), _aspect(aspectRatio), _nearPlane(nearPlane), _farPlane(farPlane)
{
}

