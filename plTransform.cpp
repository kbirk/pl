#include "plTransform.h"

plTransform::plTransform() 
{
}

void plTransform::compute() 
{
    z = (x ^ y).normalize();

    if (fabs(x*y) > 0.001) 
    {
        std::cerr << "x and y are not perpendicular (dot product = " << x*y << std::endl;
        exit(1);
    }

    transform[ 0] = x.x; transform[ 4] = y.x; transform[ 8] = z.x; transform[12] = origin.x;
    transform[ 1] = x.y; transform[ 5] = y.y; transform[ 9] = z.y; transform[13] = origin.y;
    transform[ 2] = x.z; transform[ 6] = y.z; transform[10] = z.z; transform[14] = origin.z;
    transform[ 3] = 0.0; transform[ 7] = 0.0; transform[11] = 0.0; transform[15] = 1.0;
}

void plTransform::apply() const
{
    glMultMatrixf( transform );
}

plVector3 plTransform::applyInverse( const plVector3 &v ) const
{
    plVector3 p = v-origin;
    return plVector3( p*x, p*y, p*z );
} 

plVector3 plTransform::applyNormalInverse( const plVector3 &v ) const
{
    return plVector3( v*x, v*y, v*z );
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
