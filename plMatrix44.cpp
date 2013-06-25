#include "plMatrix44.h"

void plMatrix44::setRotationD(PLfloat angle_degree, const plVector3 &axis) 
{
    setRotation(angle_degree*0.017453292, axis);
}

void plMatrix44::setRotation(PLfloat angle, const plVector3 &axis) 
{
    PLfloat x = axis.x;
    PLfloat y = axis.y;
    PLfloat z = axis.z;

    // all angles in radians
    const PLfloat s = sin(angle);
    const PLfloat c = cos(angle);

    const PLfloat mag = sqrt(x*x + y*y + z*z);

    // zero vector, return identity
    if (mag == 0.0f) 
    {
        setIdentity();
        return;
    }

    // Rotation matrix is normalized
    x /= mag;
    y /= mag;
    z /= mag;

    PLfloat xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;
    xx = x * x;
    yy = y * y;
    zz = z * z;
    xy = x * y;
    yz = y * z;
    zx = z * x;
    xs = x * s;
    ys = y * s;
    zs = z * s;
    one_c = 1.0f - c;

    _data[0] = (one_c * xx) + c;
    _data[4] = (one_c * xy) - zs;
    _data[8] = (one_c * zx) + ys;
    _data[12] = 0.0f;

    _data[1] = (one_c * xy) + zs;
    _data[5] = (one_c * yy) + c;
    _data[9] = (one_c * yz) - xs;
    _data[13] = 0.0f;

    _data[2] = (one_c * zx) - ys;
    _data[6] = (one_c * yz) + xs;
    _data[10] = (one_c * zz) + c;
    _data[14] = 0.0f;

    _data[3] = 0.0f;
    _data[7] = 0.0f;
    _data[11] = 0.0f;
    _data[15] = 1.0f;

}

void plMatrix44::setRotation( const plVector4 &quat )
{
    PLfloat w,x,y,z;
    PLfloat xs,ys,zs,wx,wy,wz,xx,yy,zz,xy,yz,xz;

    w = quat.w;
    x = quat.x;
    y = quat.y;
    z = quat.z;

    /*
    if (w <= -0.999) 
    {
        setIdentity();
        return;
    }
    */
    
    xs = x + x;
    ys = y + y;
    zs = z + z;
    wx = w*xs;
    wy = w*ys;
    wz = w*zs;
    xx = x*xs;
    yy = y*ys;
    zz = z*zs;
    xy = x*ys;
    yz = y*zs;
    xz = x*zs;
    
    _data[0] = 1.0f - (yy+zz);
    _data[4] = xy-wz;
    _data[8] = xz+wy;
    _data[12] = 0.0f;
    
    _data[1] = xy + wz;
    _data[5] = 1.0f - (xx+zz);
    _data[9] = yz - wx;
    _data[13] = 0.0f;
    
    _data[2] = xz-wy;
    _data[6] = yz + wx;
    _data[10] = 1.0f - (xx+yy);
    _data[14] = 0.0f;
    
    _data[3] = 0.0f;
    _data[7] = 0.0f;
    _data[11] = 0.0f;
    _data[15] = 1.0f;
}
     
void plMatrix44::setRotation(const plVector3 &from_vector, const plVector3 &to_vector) 
{
    /*Builds the rotation matrix that rotates one vector into another.

    The generated rotation matrix will rotate the vector from into
    the plVector3 to. from and to must be unit plVector3s!

    This method is based on the code from:

    Tomas Mller, John Hughes
    Efficiently Building a Matrix to Rotate One Vector to Another
    Journal of Graphics Tools, 4(4):1-4, 1999
    */
    plVector3 from = from_vector.normalize();
    plVector3 to   = to_vector.normalize();

    PLfloat EPSILON = 0.000001f;
    PLfloat e = from * to; 
    PLfloat f = fabs(e);

    if (f > (1.0-EPSILON)) 
    {   // "from" and "to" almost parallel
        // nearly orthogonal
        PLfloat fx = fabs(from.x);
        PLfloat fy = fabs(from.y);
        PLfloat fz = fabs(from.z);

        plVector3 x;

        if (fx < fy)
        {
            if (fx<fz)
            {
                x.x = 1.0f;
                x.y = 0.0f;
                x.z = 0.0f;
            } 
            else 
            {
                x.x = 0.0f;
                x.y = 0.0f;
                x.z = 1.0f;
            }
        } 
        else 
        {
            if (fy < fz) 
            {
                x.x = 0.0f;
                x.y = 1.0f;
                x.z = 0.0f;
            } 
            else
            {
                x.x = 0.0f;
                x.y = 0.0f;
                x.z = 1.0f;
            }
        }
        plVector3 u = x - from;
        plVector3 v = x - to;

        PLfloat c1 = 2.0f / (u * u);
        PLfloat c2 = 2.0f / (v * v);
        PLfloat c3 = c1*c2 * (u * v);
                
        // set matrix entries
        _data[0] =  - c1*u.x*u.x - c2*v.x*v.x + c3*v.x*u.x;
        _data[4] =  - c1*u.x*u.y - c2*v.x*v.y + c3*v.x*u.y;
        _data[8] =  - c1*u.x*u.z - c2*v.x*v.z + c3*v.x*u.z;
                
        _data[1] =  - c1*u.y*u.x - c2*v.y*v.x + c3*v.y*u.x;
        _data[5] =  - c1*u.y*u.y - c2*v.y*v.y + c3*v.y*u.y;
        _data[9] =  - c1*u.y*u.z - c2*v.y*v.z + c3*v.y*u.z;
                
        _data[2]  =  - c1*u.z*u.x - c2*v.z*v.x + c3*v.z*u.x;
        _data[6]  =  - c1*u.z*u.y - c2*v.z*v.y + c3*v.z*u.y;
        _data[10]  =  - c1*u.z*u.z - c2*v.z*v.z + c3*v.z*u.z;
        
        _data[0]  += 1.0;
        _data[5]  += 1.0;
        _data[10] += 1.0;      
    } 
    else 
    {	
        // the most common case, unless "from"="to", or "to"=-"from"
        plVector3 v = from ^ to;
        PLfloat h = 1.0f / (1.0f + e);    // optimization by Gottfried Chen
        PLfloat hvx = h * v.x;
        PLfloat hvz = h * v.z;
        PLfloat hvxy = hvx * v.y;
        PLfloat hvxz = hvx * v.z;
        PLfloat hvyz = hvz * v.y;
                    
        _data[0] = e + hvx * v.x;
        _data[4] = hvxy - v.z;
        _data[8] = hvxz + v.y;

        _data[1] = hvxy + v.z;
        _data[5] = e + h * v.y * v.y;
        _data[9] = hvyz - v.x;

        _data[2] = hvxz - v.y;
        _data[6] = hvyz + v.x;
        _data[10] = e + hvz * v.z;      
    }

    // fill in remaining entries
    _data[12] = 0.0f;
    _data[13] = 0.0f;
    _data[14] = 0.0f;
    _data[15] = 1.0f;
    _data[3] = 0.0f;
    _data[7] = 0.0f;
    _data[11] = 0.0f;

}
        
void plMatrix44::setOrthographic(PLfloat xMin, PLfloat xMax, PLfloat yMin, PLfloat yMax, PLfloat zMin, PLfloat zMax)
{
    setIdentity();
    _data[0] = 2.0f / (xMax - xMin);
    _data[5] = 2.0f / (yMax - yMin);
    _data[10] = -2.0f / (zMax - zMin);
    _data[12] = -((xMax + xMin)/(xMax - xMin));
    _data[13] = -((yMax + yMin)/(yMax - yMin));
    _data[14] = -((zMax + zMin)/(zMax - zMin));
    _data[15] = 1.0f;

}

void plMatrix44::setPerspective(PLfloat fov, PLfloat aspect, PLfloat nearPlane, PLfloat farPlane)
{	
    setIdentity(); // Fastest way to get most valid values already in place

    const PLfloat PI = 3.14159265358979323846f;
    const PLfloat yMax = nearPlane * tan(fov * PI / 360.0f); 
    const PLfloat yMin = -yMax;
    const PLfloat xMin = yMin * aspect;
    const PLfloat xMax = -xMin;    

    _data[0] = (2.0f * nearPlane) / (xMax - xMin);
    _data[5] = (2.0f * nearPlane) / (yMax - yMin);
    _data[8] = (xMax + xMin) / (xMax - xMin);
    _data[9] = (yMax + yMin) / (yMax - yMin);
    _data[10] = -((farPlane + nearPlane) / (farPlane - nearPlane));
    _data[11] = -1.0f;
    _data[14] = -((2.0f * (farPlane*nearPlane))/(farPlane - nearPlane));
    _data[15] = 0.0f;

}


plMatrix44 plMatrix44::inverse() const 
{		
    plMatrix44 inv;
    // compute inverse
    // row 1
    inv._data[0] =  _data[5]*_data[10]*_data[15] - _data[5]*_data[11]*_data[14] - _data[9]*_data[6]*_data[15] + _data[9]*_data[7]*_data[14] + _data[13]*_data[6]*_data[11] - _data[13]*_data[7]*_data[10];
    inv._data[4] = -_data[4]*_data[10]*_data[15] + _data[4]*_data[11]*_data[14] + _data[8]*_data[6]*_data[15] - _data[8]*_data[7]*_data[14] - _data[12]*_data[6]*_data[11] + _data[12]*_data[7]*_data[10];
    inv._data[8] =  _data[4]*_data[9]*_data[15]  - _data[4]*_data[11]*_data[13] - _data[8]*_data[5]*_data[15] + _data[8]*_data[7]*_data[13] + _data[12]*_data[5]*_data[11] - _data[12]*_data[7]*_data[9];
    inv._data[12] = -_data[4]*_data[9]*_data[14]  + _data[4]*_data[10]*_data[13] + _data[8]*_data[5]*_data[14] - _data[8]*_data[6]*_data[13] - _data[12]*_data[5]*_data[10] + _data[12]*_data[6]*_data[9];
    // row 2
    inv._data[1] = -_data[1]*_data[10]*_data[15] + _data[1]*_data[11]*_data[14] + _data[9]*_data[2]*_data[15] - _data[9]*_data[3]*_data[14] - _data[13]*_data[2]*_data[11] + _data[13]*_data[3]*_data[10];
    inv._data[5] =  _data[0]*_data[10]*_data[15] - _data[0]*_data[11]*_data[14] - _data[8]*_data[2]*_data[15] + _data[8]*_data[3]*_data[14] + _data[12]*_data[2]*_data[11] - _data[12]*_data[3]*_data[10];
    inv._data[9] = -_data[0]*_data[9]*_data[15]  + _data[0]*_data[11]*_data[13] + _data[8]*_data[1]*_data[15] - _data[8]*_data[3]*_data[13] - _data[12]*_data[1]*_data[11] + _data[12]*_data[3]*_data[9];
    inv._data[13] =  _data[0]*_data[9]*_data[14]  - _data[0]*_data[10]*_data[13] - _data[8]*_data[1]*_data[14] + _data[8]*_data[2]*_data[13] + _data[12]*_data[1]*_data[10] - _data[12]*_data[2]*_data[9];
    // row 3
    inv._data[2] =  _data[1]*_data[6]*_data[15] - _data[1]*_data[7]*_data[14] - _data[5]*_data[2]*_data[15] + _data[5]*_data[3]*_data[14] + _data[13]*_data[2]*_data[7] - _data[13]*_data[3]*_data[6];
    inv._data[6] = -_data[0]*_data[6]*_data[15] + _data[0]*_data[7]*_data[14] + _data[4]*_data[2]*_data[15] - _data[4]*_data[3]*_data[14] - _data[12]*_data[2]*_data[7] + _data[12]*_data[3]*_data[6];
    inv._data[10] =  _data[0]*_data[5]*_data[15] - _data[0]*_data[7]*_data[13] - _data[4]*_data[1]*_data[15] + _data[4]*_data[3]*_data[13] + _data[12]*_data[1]*_data[7] - _data[12]*_data[3]*_data[5];
    inv._data[14] = -_data[0]*_data[5]*_data[14] + _data[0]*_data[6]*_data[13] + _data[4]*_data[1]*_data[14] - _data[4]*_data[2]*_data[13] - _data[12]*_data[1]*_data[6] + _data[12]*_data[2]*_data[5];
    // row 4
    inv._data[3] = -_data[1]*_data[6]*_data[11] + _data[1]*_data[7]*_data[10] + _data[5]*_data[2]*_data[11] - _data[5]*_data[3]*_data[10] - _data[9]*_data[2]*_data[7] + _data[9]*_data[3]*_data[6];
    inv._data[7] =  _data[0]*_data[6]*_data[11] - _data[0]*_data[7]*_data[10] - _data[4]*_data[2]*_data[11] + _data[4]*_data[3]*_data[10] + _data[8]*_data[2]*_data[7] - _data[8]*_data[3]*_data[6];
    inv._data[11] = -_data[0]*_data[5]*_data[11] + _data[0]*_data[7]*_data[9]  + _data[4]*_data[1]*_data[11] - _data[4]*_data[3]*_data[9]  - _data[8]*_data[1]*_data[7] + _data[8]*_data[3]*_data[5];
    inv._data[15] =  _data[0]*_data[5]*_data[10] - _data[0]*_data[6]*_data[9]  - _data[4]*_data[1]*_data[10] + _data[4]*_data[2]*_data[9]  + _data[8]*_data[1]*_data[6] - _data[8]*_data[2]*_data[5];
    // calculate determinant
    PLfloat det = _data[0]*inv._data[0] + _data[1]*inv._data[4] + _data[2]*inv._data[8] + _data[3]*inv._data[12];
    inv *= 1.0f/det; 
    // return
    return inv;
}

plVector4 operator * ( const plVector4 &v, const plMatrix44 &m )
{
    PLfloat x = v.x * m(0,0) + v.y * m(1,0) + v.z * m(2,0) + v.w * m(3,0);
    PLfloat y = v.x * m(0,1) + v.y * m(1,1) + v.z * m(2,1) + v.w * m(3,1);
    PLfloat z = v.x * m(0,2) + v.y * m(1,2) + v.z * m(2,2) + v.w * m(3,2);
    PLfloat w = v.x * m(0,3) + v.y * m(1,3) + v.z * m(2,3) + v.w * m(3,3);
    
    return plVector4(x,y,z,w);
}

