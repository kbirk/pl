#include "plTransform.h"

plTransform::plTransform() 
{
}

void plTransform::compute() 
{
    _z = (_x ^ _y).normalize(); // re-compute z to ensure it is orthogonal to x and y

    if (fabs(_x*_y) > 0.001) 
    {
        std::cerr << "x and y are not perpendicular (dot product = " << _x*_y << std::endl;
    }

    _transform[ 0] = _x.x; _transform[ 4] = _y.x; _transform[ 8] = _z.x; _transform[12] = _origin.x;
    _transform[ 1] = _x.y; _transform[ 5] = _y.y; _transform[ 9] = _z.y; _transform[13] = _origin.y;
    _transform[ 2] = _x.z; _transform[ 6] = _y.z; _transform[10] = _z.z; _transform[14] = _origin.z;
    _transform[ 3] = 0.0;  _transform[ 7] = 0.0;  _transform[11] = 0.0;  _transform[15] = 1.0;
}


void plTransform::set( const plVector3 &x, const plVector3 &y)
{
    _x = x;
    _y = y;
    compute();
}

void plTransform::set( const plVector3 &x, const plVector3 &y, const plVector3 &z)
{
    _x = x;
    _y = y;
    _z = z;
    compute();
}

void plTransform::set( const plVector3 &x, const plVector3 &y, const plVector3 &z, const plVector3 &origin )
{
    _x = x;
    _y = y;
    _z = z;
    _origin = origin;
    compute();
}

void plTransform::apply() const
{
    glMultMatrixf( _transform );
}

plVector3 plTransform::applyInverse( const plVector3 &v ) const
{
    plVector3 p = v-_origin;
    return plVector3( p*_x, p*_y, p*_z );
} 

plVector3 plTransform::applyNormalInverse( const plVector3 &v ) const
{
    return plVector3( v*_x, v*_y, v*_z );
}

PLfloat plTransform::squaredDistToAxis( const plVector3 &v ) const    
{
    // v is already in the *local* coordinate system of the graft        
    static plVector3 axis(0,1,0);
    return (v - (v*axis)*axis).squaredLength();
}

PLfloat plTransform::projectedDistOnAxis( const plVector3 &v ) const  
{
    // v is already in the *local* coordinate system of the graft 
    static plVector3 axis(0,1,0);
    return v*axis;
}
