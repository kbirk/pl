#pragma once

#include "plCommon.h"
#include "plRenderResources.h"
#include "plRenderTechnique.h"

class plOutlineTechnique : public plRenderTechnique {
public:
    plOutlineTechnique();

    void render(const plRenderList& components) const;
};
