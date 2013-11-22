#ifndef PL_SCREEN_QUAD_TECHNIQUE_H
#define PL_SCREEN_QUAD_TECHNIQUE_H

#include "plCommon.h"
#include "plRenderTechnique.h"
#include "plRenderResources.h"


class plScreenQuadTechnique : public plRenderTechnique
{

    public:

        plScreenQuadTechnique();       

        void render( const std::set< plRenderComponent >& componentSet ) const;

    private:

        void _initState() const;
        virtual plVAO _generateQuad() const;
        virtual plRenderComponent _generateComponent() const;
};


#endif 
