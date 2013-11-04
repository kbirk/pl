#ifndef PL_STAGE_0_SHADER_H
#define PL_STAGE_0_SHADER_H

#include "plCommon.h"
#include "plShader.h"
#include "plSpline.h"
#include "plMatrix44.h"
#include "plVector4.h"
#include "plPlanningSite.h"


class plPlannerStage0Shader : public plShader
{

    public:                    
          
        plPlannerStage0Shader ( const std::vector< plString > &computeFiles );

        void getUniformLocations();
        
        void setDefectSiteUniforms( const plPlanningSite &defectSite ) const;
         
        void setTemperatureUniform ( PLfloat temperature ) const;        
        void setLocalLoadUniform   ( PLuint loadLocal ) const;
               
    private:
    
        PLuint _defectSiteGridPointCountID;
        PLuint _defectSiteTriangleCountID; 
        PLuint _defectSiteBoundaryPointCountID;
        PLuint _defectSiteAreaID;
        PLuint _defectSiteAvgNormalID;
                
        PLuint _temperatureID;

        PLuint _loadLocalID;

        PLuint _seedID;
                   
};



#endif
