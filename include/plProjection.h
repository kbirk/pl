#pragma once

#include "plCommon.h"
#include "plMatrix44.h"

#define PL_ASPECT_RATIO                          (1280.0f / 720.0f)
#define PL_NEAR_PLANE                            10.0f
#define PL_FAR_PLANE                             15000.0f
#define PL_FIELD_OF_VIEW                         7.0f

class plProjection
{
    public:

        plProjection(float32_t aspectRatio);
        plProjection(float32_t fov, float32_t aspectRatio, float32_t nearPlane, float32_t farPlane);

        plMatrix44 matrix() const { return plMatrix44(_fov, _aspect, _nearPlane, _farPlane); }

        void fov(float32_t fov) { _fov = fov; }
        void nearPlane(float32_t nearPlane) { _nearPlane = nearPlane; }
        void farPlane(float32_t farPlane) { _farPlane = farPlane; }
        void aspectRatio(float32_t aspectRatio) { _aspect = aspectRatio; }

    private:

        float32_t _fov;
        float32_t _aspect;
        float32_t _nearPlane;
        float32_t _farPlane;

};
