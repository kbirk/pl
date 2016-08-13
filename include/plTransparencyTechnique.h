#pragma once

#include "plCommon.h"
#include "plRenderTechnique.h"
#include "plRenderResources.h"


class plTransparencyTechnique : public plRenderTechnique
{

    public:

        plTransparencyTechnique();

        void render(const std::set<plRenderComponent >& componentSet) const;

};
