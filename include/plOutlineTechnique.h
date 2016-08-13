#pragma once

#include "plCommon.h"
#include "plRenderTechnique.h"
#include "plRenderResources.h"


class plOutlineTechnique : public plRenderTechnique
{

    public:

        plOutlineTechnique();

        void render(const std::set<plRenderComponent >& componentSet) const;
};
