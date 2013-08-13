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
        void setAnnealingUniforms( PLuint size, PLfloat area, PLfloat temp, PLfloat energy, const plSeq<PLuint> &grafts );
    private:
    
        PLuint _siteMeshSizeID;                  
        PLuint _siteAreaID;
               
        PLuint _temperatureID;        
        PLuint _stateEnergyID;
        PLuint _stateGraftsID;
     

};



#endif
