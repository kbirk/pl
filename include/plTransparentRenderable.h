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

        PLbool isTransparent() const;

        virtual void extractRenderComponents(plRenderMap& renderMap) const = 0;

    protected:

        PLbool _isTransparent;

};
