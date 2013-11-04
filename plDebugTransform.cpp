#include "plDebugTransform.h"


plDebugTransform::plDebugTransform( const plVector3 &x, const plVector3 &y, const plVector3 &origin )
    : _origin( origin ), _x( x ), _y( y ), _z( (x ^ y).normalize() )
{
}


void plDebugTransform::draw() const
{
    const float LENGTH = 3.0f;

    // draw x
    plColourStack::load( PL_X_AXIS_COLOUR );
    plDraw::arrow( _origin, _x, LENGTH );
    
    // draw y
    plColourStack::load( PL_Y_AXIS_COLOUR);
    plDraw::arrow( _origin, _y, LENGTH );
    
    // draw z
    plColourStack::load( PL_Z_AXIS_COLOUR );
    plDraw::arrow( _origin, _z, LENGTH );  
}

