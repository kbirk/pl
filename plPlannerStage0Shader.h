#ifndef __PL_STAGE_0_SHADER_H__
#define __PL_STAGE_0_SHADER_H__

#include "plCommon.h"
#include "plShader.h"
#include "plSiteGrid.h"
#include "plSpline.h"
#include "plMatrix44.h"

class plPlannerStage0Shader : public plShader
{
    public:                    
          
        plPlannerStage0Shader ( const char *computeFile );
        ~plPlannerStage0Shader();

        void getUniformLocations();
        void setAnnealingUniforms( PLuint size, 
                                   PLfloat area, 
                                   PLfloat temp, 
                                   PLfloat energy, 
                                   PLfloat count,
                                   const plSeq<PLuint> &indices,
                                   const plSeq<plMatrix44> &perturbations );
               
    private:
    
        PLuint _siteMeshSizeID;                  
        PLuint _siteAreaID;
               
        PLuint _stateTemperatureID;                
        PLuint _stateEnergyID;
        PLuint _stateIndicesID;
        PLuint _statePerturbationsID;
        PLuint _stateGraftCountsID;
            
};



#endif
