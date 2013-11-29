#ifndef PL_RENDERER_H
#define PL_RENDERER_H

#include "plCommon.h"
#include "plVector3.h"
#include "plRenderingPipeline.h"
#include "plRenderable.h"
#include "plRenderComponent.h"
#include "plFBO.h"
#include "plPlanTechnique.h"
#include "plScreenQuadTechnique.h"
#include "plOutlineTechnique.h"
#include "plArthroCamTechnique.h"
#include "plTransparencyTechnique.h"
#include "plMinimalTechnique.h"


enum plTechniqueEnums
{   
    PL_PLAN_TECHNIQUE = 1,  
    PL_MINIMAL_TECHNIQUE,
    PL_TRANSPARENCY_TECHNIQUE,
    PL_OUTLINE_TECHNIQUE,
    PL_ARTHRO_CAM_TECHNIQUE,
    PL_DEBUG_TECHNIQUE,
    PL_MENU_TECHNIQUE,
    PL_SCREEN_QUAD_TECHNIQUE
};


namespace plRenderer
{  
    void init(); 
    void queue( const plRenderable& renderable ); 
    void draw();  
}


#endif

