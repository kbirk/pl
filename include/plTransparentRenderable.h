#pragma once

#include "plCommon.h"
#include "plRenderable.h"

class plTransparentRenderable : public plRenderable
{

    public:

        plTransparentRenderable();

        virtual void toggleVisibility();
        virtual void setVisible();
        virtual void setTransparent();
        virtual void setInvisible();

        bool isTransparent() const;

        virtual void extractRenderComponents(plRenderMap& renderMap) const = 0;

    protected:

        bool _isTransparent;

};
