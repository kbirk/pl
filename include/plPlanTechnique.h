#pragma once

#include "plCommon.h"
#include "plRenderResources.h"
#include "plRenderTechnique.h"

class plPlanTechnique : public plRenderTechnique {
public:
    plPlanTechnique();

    void render(const plRenderList& components) const;

private:
    void _initState() const;
};
