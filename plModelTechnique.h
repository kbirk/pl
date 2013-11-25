#ifndef PL_MODEL_TECHNIQUE_H
#define PL_MODEL_TECHNIQUE_H

#include "plCommon.h"
#include "plRenderTechnique.h"
#include "plRenderResources.h"


class plModelTechnique : public plRenderTechnique
{

    public:

        plModelTechnique();       

        void render( const std::set< plRenderComponent >& componentSet ) const;

    private:

        void _initState() const;
       
};


#endif 
