#ifndef PL_RENDER_TECHNIQUE_H
#define PL_RENDER_TECHNIQUE_H

#include "plCommon.h"
#include "plRenderable.h"
#include "plRenderComponent.h"
#include "plWindow.h"

class plRenderTechnique
{

    public:

        plRenderTechnique();       

        virtual ~plRenderTechnique();

        virtual void render( const std::set< plRenderComponent >& componentSet ) const = 0;


};


#endif 
