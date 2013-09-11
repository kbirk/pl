#include "plTransform.h"

plTransform::plTransform() 
{
}


plTransform::plTransform( const plVector3 &y, const plVector3 &origin )
    : _x( (y ^ plVector3(1,0,0)).normalize() ), _y( y ), _origin( origin )
{
    _compute();
}


plTransform::plTransform( const plVector3 &x, const plVector3 &y, const plVector3 &origin )
    : _x( x ), _y( y ), _origin( origin )
{
    _compute();
}


plTransform::plTransform( const plSeq<plString> &row )
{
    _origin = plVector3( row[1] );
    _x      = plVector3( row[2] );
    _y      = plVector3( row[3] ); 
    _compute();
}


void plTransform::_compute() 
{
    _z = (_x ^ _y).normalize(); // re-compute z to ensure it is orthogonal to x and y

    if (fabs(_x*_y) > 0.001) 
    {
        std::cerr << "x and y are not perpendicular (dot product = " << _x*_y << std::endl;
    }

    _transform.setColumn(0, _x.x,      _x.y,      _x.z,      0.0f);
    _transform.setColumn(1, _y.x,      _y.y,      _y.z,      0.0f);
    _transform.setColumn(2, _z.x,      _z.y,      _z.z,      0.0f);
    _transform.setColumn(3, _origin.x, _origin.y, _origin.z, 1.0f);
}


void plTransform::set( const plVector3 &x, const plVector3 &y)
{
    _x = x.normalize();
    _y = y.normalize();
    _compute();
}


void plTransform::set( const plVector3 &x, const plVector3 &y, const plVector3 &origin)
{
    _x = x.normalize();
    _y = y.normalize();
    _origin = origin;
    _compute();
}


void plTransform::set( const plVector3 &x, const plVector3 &y, const plVector3 &z, const plVector3 &origin )
{
    _x = x.normalize();
    _y = y.normalize();
    _z = z.normalize();
    _origin = origin;
    _compute();
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


std::ostream& operator << ( std::ostream& out, const plTransform &t )
{
    out << t._origin << "," << t._x << "," << t._y;  
    return out;
}
