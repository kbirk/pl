#ifndef PL_RENDER_RESOURCES_H
#define PL_RENDER_RESOURCES_H

#include "plCommon.h"
#include "plShader.h"
#include "plVertexFragmentShader.h"
#include "plFBO.h"


enum plFBOEnums
{
    PL_MAIN_FBO = 1,            // the main fbo that holds all the render buffers
};


enum plShaderEnums
{
    PL_MINIMAL_SHADER = 1,      // no lighting calculations
    PL_PHONG_SHADER,            // phong lighting
    PL_OUTLINE_SHADER,          // draws to outline shader
    PL_ARTHRO_CAM_SHADER,       // texturing shader for arthro cam feed
    PL_FBO_SHADER,              // shader for final rendering pass, draws colour buffer to screen and applies outline effect
};


namespace plRenderResources
{  
    void init();
    void reshape( PLuint width, PLuint height ); 
     
    const std::shared_ptr< plFBO >&    fbos   ( PLuint id );
    const std::shared_ptr< plShader >& shaders( PLuint id ); 
}







#endif

