#ifndef __PL_STAGE_2_SHADER_H__
#define __PL_STAGE_2_SHADER_H__

#include "plCommon.h"
#include "plShader.h"
#include "plSiteGrid.h"
#include "plSpline.h"
#include "plMatrix44.h"
#include "plVector4.h"

class plPlannerStage2Shader : public plShader
{

    public:                    
          
        plPlannerStage2Shader ( const char *computeFile );

        void getUniformLocations();
        
        void setGraftUniforms ( PLuint count, 
                                PLuint numDirections,
                                const plSeq<PLfloat> &radii ) const;
                               
        void setSiteUniforms  (  PLuint donorCount,
                                 PLuint totalGridPoints,
                                 const plSeq<PLuint> &donorGridSize,
                                 const plSeq<PLuint> &donorDataOffset) const;
        
        void setSeedUniform ( PLfloat seed ) const;
        
    private:
    
        PLuint _graftCountID;
        PLuint _graftRadiiID;

        PLuint _totalGridPointsID;
        PLuint _donorSiteCountID;
        PLuint _donorSiteGridSizeID;
        PLuint _donorSiteDataOffsetID;
        
        PLuint _numDirectionsID;
 
        PLuint _seedID;       
};



#endif
