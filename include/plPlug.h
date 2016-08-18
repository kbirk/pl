#pragma once

#include "plCommon.h"
#include "plMeshSpecific.h"
#include "plVector3.h"
#include "plBoneAndCartilage.h"
#include "plTransform.h"

#define PL_MAX_GRAFT_ROTATION 25.0f

class plPlug : public plMeshSpecific
{
    public:

        plPlug();
        plPlug(const plMesh& mesh, const plTransform& surfaceTransform, const plTransform& rotationalOffset);

        const plTransform& surfaceTransform() const;
        const plTransform& offsetTransform() const;
        plTransform finalTransform() const;

        void move(const plVector3& origin, const plVector3& y);
        void rotate(const plVector3& y);
        void rotate(float32_t angleDegrees);

    private:

        plTransform _surfaceTransform;
        plTransform _rotationalOffset;

};
