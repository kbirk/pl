#pragma once

#include "plCommon.h"
#include "plRenderResources.h"
#include "plRenderTechnique.h"

class plMinimalTechnique : public plRenderTechnique {
public:
    plMinimalTechnique();

    void render(const plRenderList& components) const;

private:
    void _initState() const;
};
