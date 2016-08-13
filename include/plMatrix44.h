#pragma once

#include "plCommon.h"
#include "plVector3.h"
#include "plVector4.h"

class plTriangle;
class plMesh;

class plMatrix44
{
    public:

        // 4x4 matrix - column major
        //    0    4    8    12
        //    1    5    9    13
        //    2    6    10    14
        //    3    7    11    15

        plMatrix44();                                     // identity
        plMatrix44(float32_t scale);                      // scale
        plMatrix44(float32_t x, float32_t y, float32_t z);    // translation
        plMatrix44(const plVector3 &translation);       // translation
        plMatrix44(float32_t angle, const plVector3 &axis);                                               // rotation
        plMatrix44(const plVector3 &x, const plVector3 &y, const plVector3 &z, const plVector3 &t);     // affine transformation
        plMatrix44(float32_t xMin, float32_t xMax, float32_t yMin, float32_t yMax, float32_t zMin, float32_t zMax); // orthographic projection matrix
        plMatrix44(float32_t fov, float32_t aspectRatio, float32_t zMin, float32_t zMax);                       // perspective projection matrix
        plMatrix44(float32_t c1r1, float32_t c2r1, float32_t c3r1, float32_t c4r1,
                    float32_t c1r2, float32_t c2r2, float32_t c3r2, float32_t c4r2,
                    float32_t c1r3, float32_t c2r3, float32_t c3r3, float32_t c4r3,
                    float32_t c1r4, float32_t c2r4, float32_t c3r4, float32_t c4r4);

        plMatrix44(const plMatrix44 &m);            // copy constructor

        bool importFile(const plString &file);  // openCV xml input file

        void setRow(int32_t row, float32_t x, float32_t y, float32_t z, float32_t w);
        void setColumn(int32_t col, float32_t x, float32_t y, float32_t z, float32_t w);
        void setRow(int32_t row, const plVector4& vector);
        void setColumn(int32_t col, const plVector4& vector);

        plVector3 getRow(int32_t row);
        plVector3 getColumn(int32_t col);

        void setZero        ();
        void setIdentity    ();
        void setTranslation (float32_t x, float32_t y, float32_t z);
        void setTranslation (const plVector3 &t);
        void setScale       (float32_t scale);
        void setScale       (float32_t xScale, float32_t yScale, float32_t zScale);
        void setScale       (const plVector3 s);

        void setRotation (float32_t angle_radian, const plVector3 &axis);
        void setRotation (const plVector4 &quat);
        void setRotationD(float32_t angle_degree, const plVector3 &axis);
        void setRotation (const plVector3 &from, const plVector3 &to);

        void setOrthographic(float32_t xMin, float32_t xMax, float32_t yMin, float32_t yMax, float32_t zMin, float32_t zMax);
        void setPerspective (float32_t fov, float32_t aspect, float32_t nearPlane, float32_t farPlane);

        plMatrix44 inverse() const;

        plMatrix44 &operator= (const plMatrix44 &m);
        float32_t&    operator[](int32_t index)               { return _data[index]; }
        float32_t     operator[](int32_t index) const           { return _data[index]; }
        float32_t&    operator()(int32_t row, int32_t col)       { return _data[(col*4) + row]; }
        float32_t     operator()(int32_t row, int32_t col) const { return _data[(col*4) + row]; }

        plMatrix44  operator- () const;        // negation
        plMatrix44  operator+ (const plMatrix44 &m) const;
        plMatrix44  operator- (const plMatrix44 &m) const;
        plMatrix44  operator* (float32_t s) const;
        plMatrix44 &operator*=(float32_t s);
        plVector3   operator* (const plVector3 &v) const;
        plVector4   operator* (const plVector4 &v) const;
        plTriangle  operator* (const plTriangle &tri) const;
        plMesh      operator* (const plMesh& mesh) const;
        plMatrix44  operator* (const plMatrix44 &m) const;
        std::vector<plTriangle> operator*(const std::vector<plTriangle> &tris) const;

    protected:

        float32_t _data[16];

};

std::ostream& operator << (std::ostream& stream, const plMatrix44 &m);

plVector4 operator * (const plVector4 &v, const plMatrix44 &m);
plVector3 operator * (const plVector3 &v, const plMatrix44 &m);
