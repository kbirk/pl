#ifndef __PL_STAGE_0_SHADER_H__
#define __PL_STAGE_0_SHADER_H__

#include "plCommon.h"
#include "plShader.h"
#include "plSiteGrid.h"
#include "plSpline.h"

class plPlannerStage0Shader : public plShader
{
    public:                    
          
        plPlannerStage0Shader ( const char *computeFile );
        ~plPlannerStage0Shader();

        void getUniformLocations();
        void setMeshSizeUniform ( PLuint size );
               
    private:
    
        // defect site mesh triangle count
        PLuint        _siteMeshSizeID;  
};



#endif
