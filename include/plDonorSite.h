#pragma once

#include "plCommon.h"
#include "plVector3.h"
#include "plMeshSpecific.h"
#include "plBoundary.h"
#include "plRenderable.h"
#include "plPicking.h"

class plDonorSite : public plRenderable
{
    public:

        plDonorSite();
        plDonorSite(const plMesh& mesh);
        plDonorSite(const plBoundary &boundary);

        virtual ~plDonorSite();

        void extractRenderComponents(plRenderMap& renderMap) const;
        void extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const;

        plBoundary boundary;
};
