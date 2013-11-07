#include "plDebugSphere.h"

plDebugSphere::plDebugSphere() { }

plDebugSphere::plDebugSphere( const plVector3 &origin, PLfloat radius, const plVector4 &colour )
    : _origin( origin ), _radius( radius ), _colour( colour )
{
}


void plDebugSphere::draw() const
{
    plColourStack::push( _colour );
    plDraw::sphere( _origin, _radius );
    plColourStack::pop();
}

void plDebugSphere::colour(const plVector4& c)
{
    _colour = c;
}

void plDebugSphere::radius(const PLfloat &r)
{
    _radius = r;
}

void plDebugSphere::origin(const plVector3& o)
{
    _origin = o;
}
