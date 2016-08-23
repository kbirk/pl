#pragma once

#include "plCommon.h"
#include "plRenderTechnique.h"
#include "plRenderResources.h"

class plOutlineTechnique : public plRenderTechnique
{
    public:

        plOutlineTechnique();

        void render(const plRenderList& components) const;
};
