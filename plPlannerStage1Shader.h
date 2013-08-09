#ifndef __PL_STAGE_1_SHADER_H__
#define __PL_STAGE_1_SHADER_H__

#include "plCommon.h"
#include "plShader.h"
#include "plSiteGrid.h"
#include "plSpline.h"

class plPlannerStage1Shader : public plShader
{
    public:                    
          
        plPlannerStage1Shader ( const char *computeFile );
        ~plPlannerStage1Shader();

        void getUniformLocations();
        
    private:
    
        // grid size (number of points/normals)

        PLuint        _gridSizeID;  
        PLuint        _siteMeshSizeID;                  
        PLuint        _siteMeshTrianglesID;
        PLuint        _siteAreaID;

};



#endif
