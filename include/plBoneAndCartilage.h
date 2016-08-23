#pragma once

#include "plCommon.h"
#include "plModel.h"

class plBoneAndCartilage : public plRenderable
{
    public:

        std::shared_ptr<plModel> bone;
        std::shared_ptr<plModel> cartilage;
        std::shared_ptr<plModel> combined;

        plBoneAndCartilage(
            const plString& boneFile,
            const plString& cartilageFile,
            const plString& combinedFile);

        void extractRenderComponents(plRenderMap& renderMap) const;
        void extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const;

        plVector3 getCentroid() const;

};
