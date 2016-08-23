#pragma once

#include "plCommon.h"
#include "plRenderTechnique.h"
#include "plRenderResources.h"


class plPlanTechnique : public plRenderTechnique
{
    public:

        plPlanTechnique();

        void render(const plRenderList& components) const;

    private:

        void _initState() const;

};
