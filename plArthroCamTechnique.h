#ifndef PL_ARTHRO_CAM_TECHNIQUE_H
#define PL_ARTHRO_CAM_TECHNIQUE_H

#include "plCommon.h"
#include "plScreenQuadTechnique.h"


class plArthroCamTechnique : public plScreenQuadTechnique
{

    public:

        plArthroCamTechnique();       

        void render( const std::set< plRenderComponent >& componentSet ) const;

    private:

        void _initState() const;
};


#endif 
