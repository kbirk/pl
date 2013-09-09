#ifndef __PL_STAGE_1_SHADER_H__
#define __PL_STAGE_1_SHADER_H__

#include "plCommon.h"
#include "plShader.h"
#include "plSiteGrid.h"
#include "plSpline.h"
#include "plMatrix44.h"
#include "plVector4.h"

class plPlannerStage1Shader : public plShader
{

    public:                    
          
        plPlannerStage1Shader ( const char *computeFile );

        void getUniformLocations();
        
        void setGraftUniforms ( PLuint count, 
                                const plSeq<plVector4> &positions,
                                const plSeq<plVector4> &normals,
                                const plSeq<PLfloat>   &radii ) const;
                               
        void setSiteUniforms  ( PLuint defectMeshSize, 
                                PLuint donorCount,
                                const plSeq<PLuint> &donorMeshSize,
                                const plSeq<PLuint> &donorGridSize,
                                const plSeq<PLuint> &donorPerimSize) const;
        
        void setDirectionUniform ( const plVector4 &direction ) const;
        
    private:
    
        PLuint _graftCountID;
        PLuint _graftPositionsID;
        PLuint _graftNormalsID;
        PLuint _graftRadiiID;
    
        PLuint _defectSiteMeshSizeID; 
                         
        PLuint _donorSiteCountID;
        PLuint _donorSiteMeshSizeID;
        PLuint _donorSiteGridSizeID;
        PLuint _donorSitePerimSizeID;
 
        PLuint _directionID;       
};



#endif
