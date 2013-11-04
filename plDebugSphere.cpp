#include "plDebugSphere.h"

plDebugSphere::plDebugSphere( const plVector3 &origin,
                              PLfloat radius,
                              const plVector4 &colour )
{
    this->radius = radius;
    this->origin = origin;
    this->colour = colour;
}

