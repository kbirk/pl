#pragma once

#include "plCommon.h"
#include "plRenderTechnique.h"
#include "plRenderResources.h"

class plMinimalTechnique : public plRenderTechnique
{
    public:

        plMinimalTechnique();

        void render(const std::set<plRenderComponent>& componentSet) const;

    private:

        void _initState() const;

};
