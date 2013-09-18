#ifndef __PL_STAGE_0_SHADER_H__
#define __PL_STAGE_0_SHADER_H__

#include "plCommon.h"
#include "plShader.h"
#include "plSiteGrid.h"
#include "plSpline.h"
#include "plMatrix44.h"
#include "plVector4.h"

class plPlannerStage0Shader : public plShader
{

    public:                    
          
        plPlannerStage0Shader ( const char *computeFile );

        void getUniformLocations();
        
        void setSiteUniforms     ( PLuint  meshSize, 
                                   PLfloat meshArea, 
                                   PLuint  gridSize,
                                   PLuint  perimSize ) const;
                                   
        void setAnnealingUniforms( PLfloat temp, 
                                   PLfloat energy, 
                                   PLfloat count,
                                   const plSeq<plVector4> &positions,
                                   const plSeq<plVector4> &normals,
                                   const plSeq<PLfloat>   &radii ) const;
         
        void setLocalLoadUniform( PLuint load ) const;
               
    private:
    
        PLuint _siteMeshSizeID;                  
        PLuint _siteMeshAreaID;
        PLuint _siteGridSizeID;
        PLuint _sitePerimSizeID;
         
        PLuint _stateTemperatureID;                
        PLuint _stateEnergyID;
        PLuint _stateGraftPositionsID;
        PLuint _stateGraftNormalsID;
        PLuint _stateGraftRadiiID;
        PLuint _stateGraftCountID;

        PLuint _loadLocalID;

        PLuint _seedID;
                   
};



#endif
