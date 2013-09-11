#include "plVector3.h"



plVector3::plVector3() 
{
    x = y = z = 0;
}

plVector3::plVector3( PLfloat xx, PLfloat yy, PLfloat zz )
{
    x = xx; y = yy; z = zz;
}

plVector3::plVector3( PLchar *string ) 
{
    sscanf( string, "%f %f %f", &x, &y, &z );
}

plVector3::plVector3( const plString &str ) 
{
    sscanf( str.c_str(), "%f %f %f", &x, &y, &z );
}

PLbool plVector3::operator == (const plVector3 &p) const
{
    return x == p.x && y == p.y && z == p.z;
}

PLbool plVector3::operator != (const plVector3 &p) const
{
    return x != p.x || y != p.y || z != p.z;
}

plVector3 plVector3::operator + (const plVector3 &p) const
{ 
    return plVector3( x+p.x, y+p.y, z+p.z );
}

plVector3 plVector3::operator - (const plVector3 &p) const
{ 
    return plVector3( x-p.x, y-p.y, z-p.z );
}

PLfloat plVector3::operator * (const plVector3 &p) const     /* dot product */
{ 
    return x * p.x + y * p.y + z * p.z;
}

plVector3 plVector3::operator ^ (const plVector3 &p) const    /* cross product */
{ 
    return plVector3( y*p.z-p.y*z, -(x*p.z-p.x*z), x*p.y-p.x*y );
}

plVector3 plVector3::operator-() const
{
    // return negated vector
    return plVector3(-x, -y, -z);
}

plVector3 plVector3::normalize() const 
{
    PLfloat len;
    len = sqrt( x*x + y*y + z*z );
    if (len == 0)
    {
        //std::cerr << "normalize error: length is 0\n";
        return plVector3(0,0,0);
    }
    else
        return plVector3( x/len, y/len, z/len );
}

PLfloat plVector3::length() const
{ 
    return sqrt( x*x + y*y + z*z );
}

PLfloat plVector3::squaredLength() const
{ 
    return x*x + y*y + z*z;
}


plVector3 operator * ( PLfloat k, const plVector3 &p )
{
    plVector3 q;

    q.x = p.x * k;
    q.y = p.y * k;
    q.z = p.z * k;

    return q;
}


plVector3 plVector3::perp1() const
{
    plVector3 result(0,0,0);

    if (x == 0)
    {
        if (y == 0 || z == 0)
            result.x = 1;     /* v = (0 0 z) or (0 y 0) */
        else
        {
            result.y = -z; /* v = (0 y z) */
            result.z = y;
        }
    }
    else if (y == 0)
    {
        if (z == 0)
            result.z = 1;     /* v = (x 0 0) */
        else
        {
            result.x = -z; /* v = (x 0 z) */
            result.z = x;
        }
    }
    else
    {
        result.x = -y;   /* v = (x y z) or (x y 0) */
        result.y = x;
    }

    PLfloat lenRecip = 1.0f / sqrt( result.x*result.x + result.y*result.y + result.z*result.z );
    result.x *= lenRecip;
    result.y *= lenRecip;
    result.z *= lenRecip;

    return result;
}


plVector3 plVector3::perp2() const
{
    plVector3 result(0,0,0);

    if (x == 0)
    {
        if (y == 0)
            result.y = 1;
        else if (z == 0)
            result.z = 1;
        else
        result.x = 1;
    }
    else
    {
        if (y == 0)
            result.y = 1;
        else if (z == 0)
            result.z = 1;
        else
        {
            result.x = x * z;
            result.y = y * z;
            result.z = - x*x - y*y;

            PLfloat lenRecip = 1.0f / sqrt( result.x*result.x + result.y*result.y + result.z*result.z );
            result.x *= lenRecip;
            result.y *= lenRecip;
            result.z *= lenRecip;
        }
    }

  return result;
}



PLfloat distToEdge( const plVector3 &v, const plVector3 &head, const plVector3 &tail )
{
    plVector3 toV = v - tail;
    plVector3 me = head - tail;
    PLfloat  dot = (toV * me) / (me * me);
    plVector3 perp = toV - dot * me;

    if (dot < 0.15f || dot > 0.85f)
        return FLT_MAX;
    else
        return perp * perp;
}


// I/O operators
std::ostream& operator << ( std::ostream& stream, const plVector3 &p )
{
  stream << p.x << " " << p.y << " " << p.z;
  return stream;
}

std::istream& operator >> ( std::istream& stream, plVector3 & p )
{
  stream >> p.x >> p.y >> p.z;
  return stream;
}


