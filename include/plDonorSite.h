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

        plDonorSite(std::shared_ptr<plMesh> mesh);
        plDonorSite(std::shared_ptr<plBoundary> boundary);

        virtual ~plDonorSite();

        void extractRenderComponents(plRenderMap& renderMap) const;
        void extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const;

        std::shared_ptr<plBoundary> boundary;
};
