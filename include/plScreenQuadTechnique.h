#pragma once

#include "plCommon.h"
#include "plRenderTechnique.h"
#include "plRenderResources.h"

class plScreenQuadTechnique : public plRenderTechnique
{
    public:

        plScreenQuadTechnique();

        void render(const plRenderList& components) const;

    private:

        std::shared_ptr<plVAO> _generateQuad() const;
        std::shared_ptr<plRenderComponent> _generateComponent() const;
};
