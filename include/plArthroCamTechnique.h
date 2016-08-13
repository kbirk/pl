#pragma once

#include "plCommon.h"
#include "plRenderTechnique.h"

class plArthroCamTechnique : public plRenderTechnique
{

    public:

        plArthroCamTechnique();

        void render(const std::set<plRenderComponent >& componentSet) const;

};
