#ifndef PL_RENDERER_H
#define PL_RENDERER_H

#include "plCommon.h"
#include "plVector3.h"
#include "plRenderingPipeline.h"
#include "plRenderable.h"
#include "plRenderComponent.h"
#include "plFBO.h"


namespace plRenderer
{  
    void init(); 
    void resize( PLuint width, PLuint height );  
    void queue( const plRenderable& renderable ); 
    void draw();


    const plPickingInfo& previousPick();
    const plPickingInfo& pickPixel( PLuint x, PLuint y );
   
}







#endif

