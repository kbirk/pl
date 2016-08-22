#pragma once

#include "plCommon.h"
#include "plRenderTechnique.h"
#include "plRenderResources.h"

class plScreenQuadTechnique : public plRenderTechnique
{
    public:

        plScreenQuadTechnique();

        void render(const std::set<plRenderComponent>& componentSet) const;

    private:

        std::shared_ptr<plVAO> _generateQuad() const;
        plRenderComponent _generateComponent() const;
};
