#ifndef PL_RENDER_RESOURCES_H
#define PL_RENDER_RESOURCES_H

#include "plCommon.h"
#include "plShader.h"
#include "plVertexFragmentShader.h"
#include "plFBO.h"


enum plFBOEnums
{
    PL_MAIN_FBO = 1,
};


enum plShaderEnums
{
    PL_MINIMAL_SHADER = 1,
    PL_PHONG_SHADER,
    PL_OUTLINE_SHADER,
    PL_TEXTURE_2D_SHADER,
    PL_FBO_SHADER,
    PL_PICKING_SHADER
};


namespace plRenderResources
{  
    void init();
    void reshape( PLuint width, PLuint height ); 
     
    const std::shared_ptr< plFBO >&    fbos   ( PLuint id );
    const std::shared_ptr< plShader >& shaders( PLuint id ); 
}







#endif

