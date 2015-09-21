#ifndef PL_RENDERABLE_H
#define PL_RENDERABLE_H

#include "plCommon.h"
#include "plVector3.h"
#include "plRenderComponent.h"


class plRenderable
{

    public:

        plRenderable();

        PLbool isVisible() const;

        virtual void toggleVisibility();
        virtual void setVisible();
		virtual void setInvisible();

        virtual void extractRenderComponents( plRenderMap& renderMap ) const = 0;
        virtual void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const = 0;

    protected:

        PLbool _isVisible;

};


#endif
