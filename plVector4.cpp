#include "plVector4.h"

plVector4::plVector4() 
{
	x = y = z = 0;
}

plVector4::plVector4( const plVector3 &v, PLfloat ww )
{
    x = v.x;    y = v.y;    z = v.z;    w = ww;
}

plVector4::plVector4( PLfloat xx, PLfloat yy, PLfloat zz, PLfloat ww )
{
	x = xx;     y = yy;     z = zz;     w = ww; 
}

plVector4::plVector4( PLchar *string ) 
{
	sscanf( string, "%f %f %f %f", &x, &y, &z, &w );
}

plVector4::plVector4( plString str ) 
{
	sscanf( str.c_str(), "%f %f %f %f", &x, &y, &z, &w );
}

PLbool plVector4::operator == (const plVector4 &p) const
{
	return x == p.x && y == p.y && z == p.z && w == p.w;
}

PLbool plVector4::operator != (const plVector4 &p) const
{
	return x != p.x || y != p.y || z != p.z || w != p.w; 
}

plVector4 plVector4::operator + (const plVector4 &p) const
{ 
	return plVector4( x+p.x, y+p.y, z+p.z, w+p.w ); 
}

plVector4 plVector4::operator - (const plVector4 &p) const
{ 
	return plVector4( x-p.x, y-p.y, z-p.z, w-p.w ); 
}

PLfloat plVector4::operator * (const plVector4 &p) const     /* dot product */
{ 
	return x * p.x + y * p.y + z * p.z + w * p.w; 
}


plVector4 plVector4::operator-() const
{	
    // return negated vector
	return plVector4(-x, -y, -z, -w);
}

plVector4 plVector4::normalize() const 
{
	PLfloat len;
	len = sqrt( x*x + y*y + z*z + w*w );
	if (len == 0)
	{
	    std::cerr << "normalize error: length is 0\n";
		return plVector4(0,0,0,0);
	}
	else	
		return plVector4( x/len, y/len, z/len, w/len );
}

PLfloat plVector4::length() const
{ 
	return sqrt( x*x + y*y + z*z + w*w ); 
}

PLfloat plVector4::squaredLength() const
{ 
	return x*x + y*y + z*z + w*w; 
}

plVector4 operator * ( PLfloat k, const plVector4 &p )
{
	plVector4 q;

	q.x = p.x * k;
	q.y = p.y * k;
	q.z = p.z * k;
    q.w = p.w * k;
    
	return q;
}


// I/O operators
std::ostream& operator << ( std::ostream& stream, const plVector4 &p )
{
  stream << p.x << " " << p.y << " " << p.z << " " << p.w;
  return stream;
}

std::istream& operator >> ( std::istream& stream, plVector4 & p )
{
  stream >> p.x >> p.y >> p.z >> p.w;
  return stream;
}


