#pragma once

#include "plCommon.h"
#include "plString.h"

class plVector4; // forward delcaration to allow plVector4 -> plVector3 casting

class plVector3 {
public:
    float32_t x, y, z;

    plVector3();
    plVector3(float32_t xx, float32_t yy, float32_t zz);
    plVector3(const plVector4& v);
    plVector3(char* string);
    plVector3(const plString& str);

    bool operator==(const plVector3& p) const;
    bool operator!=(const plVector3& p) const;
    plVector3 operator+(const plVector3& p) const;
    plVector3 operator-(const plVector3& p) const;
    float32_t operator*(const plVector3& p) const;
    plVector3 operator^(const plVector3& p) const;

    plVector3& operator=(const plVector3& p);

    plVector3 operator-() const;

    plVector3 normalize() const;
    float32_t length() const;
    float32_t squaredLength() const;
    float32_t signedAngle(const plVector3 v, const plVector3& planeNormal) const;
};

// scalar multiplication
plVector3 operator*(float32_t k, const plVector3& p);

// I/O operators
std::ostream& operator<<(std::ostream& stream, const plVector3& p);
std::istream& operator>>(std::istream& stream, plVector3& p);
