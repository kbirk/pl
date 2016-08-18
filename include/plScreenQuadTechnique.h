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

        virtual plVAO _generateQuad() const;
        virtual plRenderComponent _generateComponent() const;
};
