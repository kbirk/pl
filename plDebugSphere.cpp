#include "plDebugSphere.h"


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

