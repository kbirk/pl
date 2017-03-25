#pragma once

#include "plCommon.h"
#include "plString.h"
#include "plVector3.h"

class plVector4
{
    public:

        float32_t x, y, z, w;

        plVector4();
        plVector4(const plVector3& v, float32_t ww = 1.0f);
        plVector4(float32_t xx, float32_t yy, float32_t zz, float32_t ww = 1.0f);
        plVector4(char *string);
        plVector4(const plString &str);

        bool operator== (const plVector4 &p) const;
        bool operator!= (const plVector4 &p) const;
        plVector4 operator+ (const plVector4 &p) const;
        plVector4 operator- (const plVector4 &p) const;
        float32_t operator* (const plVector4 &p) const;

        plVector4& operator= (const plVector4& p);

        plVector4 operator-() const;

        plVector4 normalize() const;
        float32_t length() const;
        float32_t squaredLength() const;

};

// Scalar / Vector multiplication

plVector4 operator* (float32_t k, const plVector4 &p);

// I/O operators

std::ostream& operator<< (std::ostream& stream, const plVector4 &p);
std::istream& operator>> (std::istream& stream, plVector4 &p);
