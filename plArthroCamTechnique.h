#ifndef PL_ARTHRO_CAM_TECHNIQUE_H
#define PL_ARTHRO_CAM_TECHNIQUE_H

#include "plCommon.h"
#include "plRenderTechnique.h"


class plArthroCamTechnique : public plRenderTechnique
{

    public:

        plArthroCamTechnique();       

        void render( const std::set< plRenderComponent >& componentSet ) const;

};


#endif 
