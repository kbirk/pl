#include "plVector4.h"

plVector4::plVector4()
{
    x = y = z = 0;
}

plVector4::plVector4(const plVector3& v, float32_t ww)
{
    x = v.x;    y = v.y;    z = v.z;    w = ww;
}

plVector4::plVector4(float32_t xx, float32_t yy, float32_t zz, float32_t ww)
{
    x = xx;     y = yy;     z = zz;     w = ww;
}

plVector4::plVector4(char *str)
{
    int filled = sscanf(str, "%f %f %f %f", &x, &y, &z, &w);
    if (filled != 4) {
        LOG_WARN("Error occured while loading plVector3 from: `" << str << "`.");
        exit(1);
    }
}

plVector4::plVector4(const plString& str)
{
    int filled = sscanf(str.c_str(), "%f %f %f %f", &x, &y, &z, &w);
    if (filled != 4) {
        LOG_WARN("Error occured while loading plVector3 from: `" << str << "`.");
        exit(1);
    }
}

bool plVector4::operator == (const plVector4& p) const
{
    return x == p.x && y == p.y && z == p.z && w == p.w;
}

bool plVector4::operator != (const plVector4& p) const
{
    return x != p.x || y != p.y || z != p.z || w != p.w;
}

plVector4 plVector4::operator + (const plVector4& p) const
{
    return plVector4(x+p.x, y+p.y, z+p.z, w+p.w);
}

plVector4 plVector4::operator - (const plVector4& p) const
{
    return plVector4(x-p.x, y-p.y, z-p.z, w-p.w);
}

float32_t plVector4::operator * (const plVector4& p) const     /* dot product */
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
    float32_t len = sqrt(x*x + y*y + z*z + w*w);
    if (len == 0)
    {
        LOG_WARN("Length is 0, returning zero vector");
        return plVector4(0, 0, 0,0);
    }
    else
    {
        return plVector4(x/len, y/len, z/len, w/len);
    }
}

float32_t plVector4::length() const
{
    return sqrt(x*x + y*y + z*z + w*w);
}

float32_t plVector4::squaredLength() const
{
    return x*x + y*y + z*z + w*w;
}

plVector4 operator * (float32_t k, const plVector4& p)
{
    plVector4 q;

    q.x = p.x * k;
    q.y = p.y * k;
    q.z = p.z * k;
    q.w = p.w * k;

    return q;
}


// I/O operators
std::ostream& operator << (std::ostream& stream, const plVector4& p)
{
  stream << p.x << " " << p.y << " " << p.z << " " << p.w;
  return stream;
}

std::istream& operator >> (std::istream& stream, plVector4& p)
{
  stream >> p.x >> p.y >> p.z >> p.w;
  return stream;
}
