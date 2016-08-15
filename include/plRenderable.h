#pragma once

#include "plCommon.h"
#include "plVector3.h"
#include "plRenderComponent.h"

class plRenderable
{
    public:

        plRenderable();

        bool isVisible() const;

        virtual void toggleVisibility();
        virtual void setVisible();
        virtual void setInvisible();

        virtual void extractRenderComponents(plRenderMap& renderMap) const = 0;
        virtual void extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const = 0;

    protected:

        bool _isVisible;

};
