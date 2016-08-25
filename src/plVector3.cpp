#include "plVector3.h"
#include "plVector4.h"

plVector3::plVector3()
{
    x = y = z = 0;
}

plVector3::plVector3(float32_t xx, float32_t yy, float32_t zz)
{
    x = xx; y = yy; z = zz;
}

plVector3::plVector3(const plVector4& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

plVector3::plVector3(char *str)
{
    int filled = sscanf(str, "%f %f %f", &x, &y, &z);
    if (filled != 3) {
        LOG_WARN("Error occured while loading plVector3 from: `" << str << "`.");
        exit(1);
    }
}

plVector3::plVector3(const plString& str)
{
    int filled = sscanf(str.c_str(), "%f %f %f", &x, &y, &z);
    if (filled != 3) {
        LOG_WARN("Error occured while loading plVector3 from: `" << str << "`.");
        exit(1);
    }
}

bool plVector3::operator == (const plVector3& p) const
{
    return x == p.x && y == p.y && z == p.z;
}

bool plVector3::operator != (const plVector3& p) const
{
    return x != p.x || y != p.y || z != p.z;
}

plVector3 plVector3::operator + (const plVector3& p) const
{
    return plVector3(x+p.x, y+p.y, z+p.z);
}

plVector3 plVector3::operator - (const plVector3& p) const
{
    return plVector3(x-p.x, y-p.y, z-p.z);
}

float32_t plVector3::operator * (const plVector3& p) const     /* dot product */
{
    return x * p.x + y * p.y + z * p.z;
}

plVector3 plVector3::operator ^ (const plVector3& p) const    /* cross product */
{
    return plVector3(y*p.z-p.y*z, -(x*p.z-p.x*z), x*p.y-p.x*y);
}

plVector3 plVector3::operator-() const
{
    // return negated vector
    return plVector3(-x, -y, -z);
}

plVector3& plVector3::operator=(const plVector3& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

plVector3 plVector3::normalize() const
{
    float32_t len = sqrt(x*x + y*y + z*z);
    if (len == 0.0)
    {
        LOG_WARN("Length is 0, return zero vector");
        return plVector3(0, 0, 0);
    }
    else
    {
        return plVector3(x/len, y/len, z/len);
    }
}

float32_t plVector3::length() const
{
    return sqrt(x*x + y*y + z*z);
}

float32_t plVector3::squaredLength() const
{
    return x*x + y*y + z*z;
}


float32_t plVector3::signedAngle(const plVector3 v, const plVector3& planeNormal) const
{
    float32_t angle = acos(*this * v);

    plVector3 cross = *this ^  v;

    if ((planeNormal * cross) < 0)
    {
        // Or > 0
        angle = -angle;
    }
    return angle;
}


plVector3 operator * (float32_t k, const plVector3& p)
{
    plVector3 q;

    q.x = p.x * k;
    q.y = p.y * k;
    q.z = p.z * k;

    return q;
}


std::ostream& operator << (std::ostream& stream, const plVector3& p)
{
  stream << p.x << " " << p.y << " " << p.z;
  return stream;
}


std::istream& operator >> (std::istream& stream, plVector3& p)
{
  stream >> p.x >> p.y >> p.z;
  return stream;
}
