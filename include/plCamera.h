#pragma once

#include "plCommon.h"
#include "plOpenGLCommon.h"
#include "plVector3.h"
#include "plVector4.h"
#include "plMatrix44.h"

class plCamera
{

    public:

        plVector3 position;
        plVector3 lookat;
        plVector3 up;

        plCamera();
        plCamera(plString filename);

        plVector3  getDirection() const { return (lookat - position).normalize(); }
        plMatrix44 getMatrix() const;

        void importViewParams(const std::string& filename);
        void exportViewParams(const std::string& filename);

        void translate(int32_t x, int32_t y);
        void rotate(int32_t x0, int32_t y0, int32_t x1, int32_t y1);
        void zoom(float32_t z);
        void reset(const plVector3& point);

    private:

        void _defaultInit();

};
