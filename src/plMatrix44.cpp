#include "plMatrix44.h"
#include "plMesh.h"
#include "plTriangle.h"

// identity
plMatrix44::plMatrix44()
{
    // default to identity
    setIdentity();
}

// scale
plMatrix44::plMatrix44(float32_t scale)
{
    setScale(scale);
}

// translation
plMatrix44::plMatrix44(float32_t x, float32_t y, float32_t z)
{
    setTranslation(x, y, z);
}

plMatrix44::plMatrix44(const plVector3& translation)
{
    setTranslation(translation);
}

// rotation
plMatrix44::plMatrix44(float32_t angle, const plVector3& axis)
{
    setRotation(angle, axis);
}

// affine transformation
plMatrix44::plMatrix44(const plVector3& x, const plVector3& y, const plVector3& z, const plVector3& t)
{
    _data[0] = x.x;
    _data[4] = y.x;
    _data[8] = z.x;
    _data[12] = t.x;
    _data[1] = x.y;
    _data[5] = y.y;
    _data[9] = z.y;
    _data[13] = t.y;
    _data[2] = x.z;
    _data[6] = y.z;
    _data[10] = z.z;
    _data[14] = t.z;
    _data[3] = 0.0f;
    _data[7] = 0.0f;
    _data[11] = 0.0f;
    _data[15] = 1.0f;
}

// orthographic projection matrix
plMatrix44::plMatrix44(float32_t xMin, float32_t xMax, float32_t yMin, float32_t yMax, float32_t zMin, float32_t zMax)
{
    setOrthographic(xMin, xMax, yMin, yMax, zMin, zMax);
}

// perspective projection matrix
plMatrix44::plMatrix44(float32_t fov, float32_t aspectRatio, float32_t zMin, float32_t zMax)
{
    setPerspective(fov, aspectRatio, zMin, zMax);
}

// element wise
plMatrix44::plMatrix44(float32_t c1r1, float32_t c2r1, float32_t c3r1, float32_t c4r1,
    float32_t c1r2, float32_t c2r2, float32_t c3r2, float32_t c4r2,
    float32_t c1r3, float32_t c2r3, float32_t c3r3, float32_t c4r3,
    float32_t c1r4, float32_t c2r4, float32_t c3r4, float32_t c4r4)
{
    _data[0] = c1r1;
    _data[4] = c2r1;
    _data[8] = c3r1;
    _data[12] = c4r1;
    _data[1] = c1r2;
    _data[5] = c2r2;
    _data[9] = c3r2;
    _data[13] = c4r2;
    _data[2] = c1r3;
    _data[6] = c2r3;
    _data[10] = c3r3;
    _data[14] = c4r3;
    _data[3] = c1r4;
    _data[7] = c2r4;
    _data[11] = c3r4;
    _data[15] = c4r4;
}

// copy constructor
plMatrix44::plMatrix44(const plMatrix44& m)
{
    std::memcpy(_data, m._data, sizeof(float32_t) * 16);
}

void plMatrix44::setRow(int32_t row, float32_t x, float32_t y, float32_t z, float32_t w)
{
    // copy plVector3 into specified row, rows indexed 0-3
    _data[row] = x;
    _data[row + 4] = y;
    _data[row + 8] = z;
    _data[row + 12] = w;
}

void plMatrix44::setColumn(int32_t col, float32_t x, float32_t y, float32_t z, float32_t w)
{
    // copy plVector3 into specified column, columns indexed 0-3
    _data[(col * 4)] = x;
    _data[(col * 4) + 1] = y;
    _data[(col * 4) + 2] = z;
    _data[(col * 4) + 3] = w;
}

void plMatrix44::setRow(int32_t row, const plVector4& vector)
{
    setRow(row, vector.x, vector.y, vector.z, vector.w);
}

void plMatrix44::setColumn(int32_t col, const plVector4& vector)
{
    setColumn(col, vector.x, vector.y, vector.z, vector.w);
}

plVector3 plMatrix44::getRow(int32_t row)
{
    return plVector3(_data[row], _data[row + 4], _data[row + 8]);
}

plVector3 plMatrix44::getColumn(int32_t col)
{
    return plVector3(_data[(col * 4)], _data[(col * 4) + 1], _data[(col * 4) + 2]);
}

void plMatrix44::setZero()
{
    memset(_data, 0, sizeof(float32_t) * 16);
}

void plMatrix44::setIdentity()
{
    // set to identity plMatrix44
    setZero();
    _data[0] = 1.0f;
    _data[5] = 1.0f;
    _data[10] = 1.0f;
    _data[15] = 1.0f;
}

void plMatrix44::setTranslation(float32_t x, float32_t y, float32_t z)
{
    setIdentity();
    _data[12] = x;
    _data[13] = y;
    _data[14] = z;
}

void plMatrix44::setTranslation(const plVector3& t)
{
    setTranslation(t.x, t.y, t.z);
}

void plMatrix44::setScale(float32_t scale)
{
    // create scaling plMatrix44
    setIdentity();
    _data[0] = scale;
    _data[5] = scale;
    _data[10] = scale;
}

void plMatrix44::setScale(float32_t xScale, float32_t yScale, float32_t zScale)
{
    // create scaling plMatrix44
    setIdentity();
    _data[0] = xScale;
    _data[5] = yScale;
    _data[10] = zScale;
}

void plMatrix44::setScale(const plVector3 s)
{
    // create scaling plMatrix44
    setScale(s.x, s.y, s.z);
}

// assignment
plMatrix44& plMatrix44::operator=(const plMatrix44& m)
{
    // copy data
    std::memcpy(_data, m._data, sizeof(float32_t) * 16);
    return *this;
}

// negation
plMatrix44 plMatrix44::operator-() const
{
    return plMatrix44(-_data[0], -_data[1], -_data[2], -_data[3],
        -_data[4], -_data[5], -_data[6], -_data[7],
        -_data[8], -_data[9], -_data[10], -_data[11],
        -_data[12], -_data[13], -_data[14], -_data[15]);
}

// addition
plMatrix44 plMatrix44::operator+(const plMatrix44& m) const
{
    return plMatrix44(_data[0] + m._data[0], _data[1] + m._data[1], _data[2] + m._data[2], _data[3] + m._data[3],
        _data[4] + m._data[4], _data[5] + m._data[5], _data[6] + m._data[6], _data[7] + m._data[7],
        _data[8] + m._data[8], _data[9] + m._data[9], _data[10] + m._data[10], _data[11] + m._data[11],
        _data[12] + m._data[12], _data[13] + m._data[13], _data[14] + m._data[14], _data[15] + m._data[15]);
}
// subtraction
plMatrix44 plMatrix44::operator-(const plMatrix44& m) const
{
    return plMatrix44(_data[0] - m._data[0], _data[1] - m._data[1], _data[2] - m._data[2], _data[3] - m._data[3],
        _data[4] - m._data[4], _data[5] - m._data[5], _data[6] - m._data[6], _data[7] - m._data[7],
        _data[8] - m._data[8], _data[9] - m._data[9], _data[10] - m._data[10], _data[11] - m._data[11],
        _data[12] - m._data[12], _data[13] - m._data[13], _data[14] - m._data[14], _data[15] - m._data[15]);
}
// scalar multiplication
plMatrix44 plMatrix44::operator*(float32_t s) const
{
    return plMatrix44(_data[0] * s, _data[1] * s, _data[2] * s, _data[3] * s,
        _data[4] * s, _data[5] * s, _data[6] * s, _data[7] * s,
        _data[8] * s, _data[9] * s, _data[10] * s, _data[11] * s,
        _data[12] * s, _data[13] * s, _data[14] * s, _data[15] * s);
}
plMatrix44& plMatrix44::operator*=(float32_t s)
{
    _data[0] *= s;
    _data[4] *= s;
    _data[8] *= s;
    _data[12] *= s;
    _data[1] *= s;
    _data[5] *= s;
    _data[9] *= s;
    _data[13] *= s;
    _data[2] *= s;
    _data[6] *= s;
    _data[10] *= s;
    _data[14] *= s;
    _data[3] *= s;
    _data[7] *= s;
    _data[11] *= s;
    _data[15] *= s;
    return *this;
}

// vector multiplication
plVector3 plMatrix44::operator*(const plVector3& v) const
{
    return plVector3((_data[0] * v.x) + (_data[4] * v.y) + (_data[8] * v.z) + _data[12],
        (_data[1] * v.x) + (_data[5] * v.y) + (_data[9] * v.z) + _data[13],
        (_data[2] * v.x) + (_data[6] * v.y) + (_data[10] * v.z) + _data[14]);
}

plVector4 plMatrix44::operator*(const plVector4& v) const
{
    return plVector4((_data[0] * v.x) + (_data[4] * v.y) + (_data[8] * v.z) + (_data[12] * v.w),
        (_data[1] * v.x) + (_data[5] * v.y) + (_data[9] * v.z) + (_data[13] * v.w),
        (_data[2] * v.x) + (_data[6] * v.y) + (_data[10] * v.z) + (_data[14] * v.w),
        (_data[3] * v.x) + (_data[7] * v.y) + (_data[11] * v.z) + (_data[15] * v.w));
}

// triangle multiplication
plTriangle plMatrix44::operator*(const plTriangle& tri) const
{
    plVector3 point0 = (*this) * tri.point0();
    plVector3 point1 = (*this) * tri.point1();
    plVector3 point2 = (*this) * tri.point2();

    // normal is special, we must have 0 in the final entry (default is 1 for a plVector3)
    plVector4 normal4 = *this * plVector4(tri.normal(), 0.0f);
    plVector3 normal3 = plVector3(normal4.x, normal4.y, normal4.z);

    return plTriangle(normal3, point0, point1, point2);
}

// mesh multiplication
plMesh plMatrix44::operator*(const plMesh& mesh) const
{
    const std::vector<plTriangle>& triangles = mesh.triangles();

    std::vector<plTriangle> outputTris;
    outputTris.reserve(mesh.triangles().size());

    for (uint32_t i = 0; i < triangles.size(); i++) {
        outputTris.push_back((*this) * triangles[i]);
    }
    return outputTris;
}

std::vector<plTriangle> plMatrix44::operator*(const std::vector<plTriangle>& tris) const
{
    std::vector<plTriangle> output;
    for (uint32_t i = 0; i < tris.size(); i++) {
        output.push_back((*this) * tris[i]);
    }
    return output;
}

// matrix multiplication
plMatrix44 plMatrix44::operator*(const plMatrix44& m) const
{
    plMatrix44 result;
    for (int32_t i = 0; i < 4; i++) {
        result._data[i] = _data[i] * m._data[0] + _data[i + 4] * m._data[1] + _data[i + 8] * m._data[2] + _data[i + 12] * m._data[3];
        result._data[i + 4] = _data[i] * m._data[4] + _data[i + 4] * m._data[5] + _data[i + 8] * m._data[6] + _data[i + 12] * m._data[7];
        result._data[i + 8] = _data[i] * m._data[8] + _data[i + 4] * m._data[9] + _data[i + 8] * m._data[10] + _data[i + 12] * m._data[11];
        result._data[i + 12] = _data[i] * m._data[12] + _data[i + 4] * m._data[13] + _data[i + 8] * m._data[14] + _data[i + 12] * m._data[15];
    }
    return result;
}

void plMatrix44::setRotationD(float32_t angle_degree, const plVector3& axis)
{
    setRotation(PL_DEG_TO_RAD(angle_degree), axis);
}

void plMatrix44::setRotation(float32_t angle, const plVector3& axis)
{
    float32_t x = axis.x;
    float32_t y = axis.y;
    float32_t z = axis.z;

    // all angles in radians
    const float32_t s = sin(angle);
    const float32_t c = cos(angle);

    const float32_t mag = sqrt(x * x + y * y + z * z);

    // zero vector, return identity
    if (mag == 0.0f) {
        setIdentity();
        return;
    }

    // Rotation matrix is normalized
    x /= mag;
    y /= mag;
    z /= mag;

    float32_t xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;
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

void plMatrix44::setRotation(const plVector4& quat)
{
    float32_t w, x, y, z;
    float32_t xs, ys, zs, wx, wy, wz, xx, yy, zz, xy, yz, xz;

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
    wx = w * xs;
    wy = w * ys;
    wz = w * zs;
    xx = x * xs;
    yy = y * ys;
    zz = z * zs;
    xy = x * ys;
    yz = y * zs;
    xz = x * zs;

    _data[0] = 1.0f - (yy + zz);
    _data[4] = xy - wz;
    _data[8] = xz + wy;
    _data[12] = 0.0f;

    _data[1] = xy + wz;
    _data[5] = 1.0f - (xx + zz);
    _data[9] = yz - wx;
    _data[13] = 0.0f;

    _data[2] = xz - wy;
    _data[6] = yz + wx;
    _data[10] = 1.0f - (xx + yy);
    _data[14] = 0.0f;

    _data[3] = 0.0f;
    _data[7] = 0.0f;
    _data[11] = 0.0f;
    _data[15] = 1.0f;
}

void plMatrix44::setRotation(const plVector3& from_vector, const plVector3& to_vector)
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
    plVector3 to = to_vector.normalize();

    float32_t EPSILON = 0.000001f;
    float32_t e = from * to;
    float32_t f = fabs(e);

    if (f > (1.0 - EPSILON)) { // "from" and "to" almost parallel
        // nearly orthogonal
        float32_t fx = fabs(from.x);
        float32_t fy = fabs(from.y);
        float32_t fz = fabs(from.z);

        plVector3 x;

        if (fx < fy) {
            if (fx < fz) {
                x.x = 1.0f;
                x.y = 0.0f;
                x.z = 0.0f;
            } else {
                x.x = 0.0f;
                x.y = 0.0f;
                x.z = 1.0f;
            }
        } else {
            if (fy < fz) {
                x.x = 0.0f;
                x.y = 1.0f;
                x.z = 0.0f;
            } else {
                x.x = 0.0f;
                x.y = 0.0f;
                x.z = 1.0f;
            }
        }
        plVector3 u = x - from;
        plVector3 v = x - to;

        float32_t c1 = 2.0f / (u * u);
        float32_t c2 = 2.0f / (v * v);
        float32_t c3 = c1 * c2 * (u * v);

        // set matrix entries
        _data[0] = -c1 * u.x * u.x - c2 * v.x * v.x + c3 * v.x * u.x;
        _data[4] = -c1 * u.x * u.y - c2 * v.x * v.y + c3 * v.x * u.y;
        _data[8] = -c1 * u.x * u.z - c2 * v.x * v.z + c3 * v.x * u.z;

        _data[1] = -c1 * u.y * u.x - c2 * v.y * v.x + c3 * v.y * u.x;
        _data[5] = -c1 * u.y * u.y - c2 * v.y * v.y + c3 * v.y * u.y;
        _data[9] = -c1 * u.y * u.z - c2 * v.y * v.z + c3 * v.y * u.z;

        _data[2] = -c1 * u.z * u.x - c2 * v.z * v.x + c3 * v.z * u.x;
        _data[6] = -c1 * u.z * u.y - c2 * v.z * v.y + c3 * v.z * u.y;
        _data[10] = -c1 * u.z * u.z - c2 * v.z * v.z + c3 * v.z * u.z;

        _data[0] += 1.0;
        _data[5] += 1.0;
        _data[10] += 1.0;
    } else {
        // the most common case, unless "from"="to", or "to"=-"from"
        plVector3 v = from ^ to;
        float32_t h = 1.0f / (1.0f + e); // optimization by Gottfried Chen
        float32_t hvx = h * v.x;
        float32_t hvz = h * v.z;
        float32_t hvxy = hvx * v.y;
        float32_t hvxz = hvx * v.z;
        float32_t hvyz = hvz * v.y;

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

void plMatrix44::setOrthographic(float32_t xMin, float32_t xMax, float32_t yMin, float32_t yMax, float32_t zMin, float32_t zMax)
{
    setIdentity();
    _data[0] = 2.0f / (xMax - xMin);
    _data[5] = 2.0f / (yMax - yMin);
    _data[10] = -2.0f / (zMax - zMin);
    _data[12] = -((xMax + xMin) / (xMax - xMin));
    _data[13] = -((yMax + yMin) / (yMax - yMin));
    _data[14] = -((zMax + zMin) / (zMax - zMin));
    _data[15] = 1.0f;
}

void plMatrix44::setPerspective(float32_t fov, float32_t aspect, float32_t nearPlane, float32_t farPlane)
{
    setIdentity(); // Fastest way to get most valid values already in place

    const float32_t PI = 3.14159265358979323846f;
    const float32_t yMax = nearPlane * tan(fov * PI / 360.0f);
    const float32_t yMin = -yMax;
    const float32_t xMin = yMin * aspect;
    const float32_t xMax = -xMin;

    _data[0] = (2.0f * nearPlane) / (xMax - xMin);
    _data[5] = (2.0f * nearPlane) / (yMax - yMin);
    _data[8] = (xMax + xMin) / (xMax - xMin);
    _data[9] = (yMax + yMin) / (yMax - yMin);
    _data[10] = -((farPlane + nearPlane) / (farPlane - nearPlane));
    _data[11] = -1.0f;
    _data[14] = -((2.0f * (farPlane * nearPlane)) / (farPlane - nearPlane));
    _data[15] = 0.0f;
}

plMatrix44 plMatrix44::inverse() const
{
    plMatrix44 inv;
    // compute inverse
    // row 1
    inv._data[0] = _data[5] * _data[10] * _data[15] - _data[5] * _data[11] * _data[14] - _data[9] * _data[6] * _data[15] + _data[9] * _data[7] * _data[14] + _data[13] * _data[6] * _data[11] - _data[13] * _data[7] * _data[10];
    inv._data[4] = -_data[4] * _data[10] * _data[15] + _data[4] * _data[11] * _data[14] + _data[8] * _data[6] * _data[15] - _data[8] * _data[7] * _data[14] - _data[12] * _data[6] * _data[11] + _data[12] * _data[7] * _data[10];
    inv._data[8] = _data[4] * _data[9] * _data[15] - _data[4] * _data[11] * _data[13] - _data[8] * _data[5] * _data[15] + _data[8] * _data[7] * _data[13] + _data[12] * _data[5] * _data[11] - _data[12] * _data[7] * _data[9];
    inv._data[12] = -_data[4] * _data[9] * _data[14] + _data[4] * _data[10] * _data[13] + _data[8] * _data[5] * _data[14] - _data[8] * _data[6] * _data[13] - _data[12] * _data[5] * _data[10] + _data[12] * _data[6] * _data[9];
    // row 2
    inv._data[1] = -_data[1] * _data[10] * _data[15] + _data[1] * _data[11] * _data[14] + _data[9] * _data[2] * _data[15] - _data[9] * _data[3] * _data[14] - _data[13] * _data[2] * _data[11] + _data[13] * _data[3] * _data[10];
    inv._data[5] = _data[0] * _data[10] * _data[15] - _data[0] * _data[11] * _data[14] - _data[8] * _data[2] * _data[15] + _data[8] * _data[3] * _data[14] + _data[12] * _data[2] * _data[11] - _data[12] * _data[3] * _data[10];
    inv._data[9] = -_data[0] * _data[9] * _data[15] + _data[0] * _data[11] * _data[13] + _data[8] * _data[1] * _data[15] - _data[8] * _data[3] * _data[13] - _data[12] * _data[1] * _data[11] + _data[12] * _data[3] * _data[9];
    inv._data[13] = _data[0] * _data[9] * _data[14] - _data[0] * _data[10] * _data[13] - _data[8] * _data[1] * _data[14] + _data[8] * _data[2] * _data[13] + _data[12] * _data[1] * _data[10] - _data[12] * _data[2] * _data[9];
    // row 3
    inv._data[2] = _data[1] * _data[6] * _data[15] - _data[1] * _data[7] * _data[14] - _data[5] * _data[2] * _data[15] + _data[5] * _data[3] * _data[14] + _data[13] * _data[2] * _data[7] - _data[13] * _data[3] * _data[6];
    inv._data[6] = -_data[0] * _data[6] * _data[15] + _data[0] * _data[7] * _data[14] + _data[4] * _data[2] * _data[15] - _data[4] * _data[3] * _data[14] - _data[12] * _data[2] * _data[7] + _data[12] * _data[3] * _data[6];
    inv._data[10] = _data[0] * _data[5] * _data[15] - _data[0] * _data[7] * _data[13] - _data[4] * _data[1] * _data[15] + _data[4] * _data[3] * _data[13] + _data[12] * _data[1] * _data[7] - _data[12] * _data[3] * _data[5];
    inv._data[14] = -_data[0] * _data[5] * _data[14] + _data[0] * _data[6] * _data[13] + _data[4] * _data[1] * _data[14] - _data[4] * _data[2] * _data[13] - _data[12] * _data[1] * _data[6] + _data[12] * _data[2] * _data[5];
    // row 4
    inv._data[3] = -_data[1] * _data[6] * _data[11] + _data[1] * _data[7] * _data[10] + _data[5] * _data[2] * _data[11] - _data[5] * _data[3] * _data[10] - _data[9] * _data[2] * _data[7] + _data[9] * _data[3] * _data[6];
    inv._data[7] = _data[0] * _data[6] * _data[11] - _data[0] * _data[7] * _data[10] - _data[4] * _data[2] * _data[11] + _data[4] * _data[3] * _data[10] + _data[8] * _data[2] * _data[7] - _data[8] * _data[3] * _data[6];
    inv._data[11] = -_data[0] * _data[5] * _data[11] + _data[0] * _data[7] * _data[9] + _data[4] * _data[1] * _data[11] - _data[4] * _data[3] * _data[9] - _data[8] * _data[1] * _data[7] + _data[8] * _data[3] * _data[5];
    inv._data[15] = _data[0] * _data[5] * _data[10] - _data[0] * _data[6] * _data[9] - _data[4] * _data[1] * _data[10] + _data[4] * _data[2] * _data[9] + _data[8] * _data[1] * _data[6] - _data[8] * _data[2] * _data[5];
    // calculate determinant
    float32_t det = _data[0] * inv._data[0] + _data[1] * inv._data[4] + _data[2] * inv._data[8] + _data[3] * inv._data[12];
    inv *= 1.0f / det;
    // return
    return inv;
}

// I/O operators
std::ostream& operator<<(std::ostream& stream, const plMatrix44& m)
{
    for (uint32_t i = 0; i < 4; i++) {
        stream << m(i, 0) << " " << m(i, 1) << " " << m(i, 2) << " " << m(i, 3) << std::endl;
    }

    return stream;
}

plVector4 operator*(const plVector4& v, const plMatrix44& m)
{
    float32_t x = v.x * m(0, 0) + v.y * m(1, 0) + v.z * m(2, 0) + v.w * m(3, 0);
    float32_t y = v.x * m(0, 1) + v.y * m(1, 1) + v.z * m(2, 1) + v.w * m(3, 1);
    float32_t z = v.x * m(0, 2) + v.y * m(1, 2) + v.z * m(2, 2) + v.w * m(3, 2);
    float32_t w = v.x * m(0, 3) + v.y * m(1, 3) + v.z * m(2, 3) + v.w * m(3, 3);

    return plVector4(x, y, z, w);
}

plVector3 operator*(const plVector3& v, const plMatrix44& m)
{
    float32_t x = v.x * m(0, 0) + v.y * m(1, 0) + v.z * m(2, 0) + m(3, 0);
    float32_t y = v.x * m(0, 1) + v.y * m(1, 1) + v.z * m(2, 1) + m(3, 1);
    float32_t z = v.x * m(0, 2) + v.y * m(1, 2) + v.z * m(2, 2) + m(3, 2);

    return plVector3(x, y, z);
}
