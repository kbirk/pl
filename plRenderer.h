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
#include "plRenderShapes.h"

enum plTechniqueEnums
{   
    PL_PLAN_TECHNIQUE = 1,  
    PL_MINIMAL_TECHNIQUE,
    PL_TRANSPARENCY_TECHNIQUE,
    PL_OUTLINE_TECHNIQUE,
    PL_ARTHRO_CAM_TECHNIQUE,
    PL_SCREEN_QUAD_TECHNIQUE
};


typedef std::map< PLuint, std::shared_ptr<plRenderTechnique> >   plTechniqueMap;


namespace plRenderer
{  
    void init(); 
    void queue( const plRenderable& renderable ); 
    void draw();  

    void queueSphere   ( PLuint technique, const plVector3& position, PLfloat radius );
    void queueCylinder ( PLuint technique, const plVector3& position, const plVector3& direction, PLfloat radius, PLfloat length );
    void queueDisk     ( PLuint technique, const plVector3& position, const plVector3& direction, PLfloat radius, PLbool flip = false );   
    void queueCone     ( PLuint technique, const plVector3& position, const plVector3& direction, PLfloat topRadius, PLfloat bottomRadius, PLfloat length );    
    void queueArrow    ( PLuint technique, const plVector3& position, const plVector3 &direction, PLfloat length = 2.0f, PLfloat scale = 1.0f );
    void queueAxis     ( PLuint technique, const plVector3& position, const plVector3& x, const plVector3& y, const PLfloat scale = 1.0f );
    void queuePlane    ( PLuint technique, const plVector3& position, const plVector3& normal, PLfloat scale = 1.0f );
    void queueLine     ( PLuint technique, const plVector3& p0, const plVector3& p1 ); 
}


#endif

