#pragma once

#include "plCommon.h"
#include "plRenderResources.h"
#include "plRenderTechnique.h"

class plTransparencyTechnique : public plRenderTechnique {

public:
    plTransparencyTechnique();

    void render(const plRenderList& components) const;
};
