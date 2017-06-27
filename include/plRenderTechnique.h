#pragma once

#include "plCommon.h"
#include "plRenderComponent.h"
#include "plRenderable.h"
#include "plWindow.h"

class plRenderTechnique {
public:
    plRenderTechnique();

    virtual ~plRenderTechnique();

    virtual void render(const plRenderList& components) const = 0;
};
