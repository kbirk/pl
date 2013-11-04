#ifndef PL_STAGE_2_SHADER_H
#define PL_STAGE_2_SHADER_H

#include "plCommon.h"
#include "plShader.h"
#include "plSpline.h"
#include "plMatrix44.h"
#include "plVector4.h"
#include "plPlanningSite.h"
#include "plPlannerStructs.h"

class plPlannerStage2Shader : public plShader
{

    public:                    

        plPlannerStage2Shader ( const std::vector< plString > &computeFiles );
        
        void getUniformLocations();
        
        void setDefectSiteUniforms( const plPlanningSite &defectSite ) const;
        void setDonorSiteUniforms( const std::vector<plPlanningSite> &donorSites ) const;
        void setDefectSolutionUniforms ( const plDefectSolution &solution ) const;
        void setRotationAngleUniforms( PLuint numDirections ) const;
        void setRotationIndexUniform( PLuint rotationIndex ) const;
        
    private:
    
        PLuint _defectSiteGridPointCountID;
        PLuint _defectSiteBoundaryPointCountID;
        PLuint _defectSiteTriangleCountID;
        PLuint _defectSiteAreaID;
        PLuint _defectSiteAvgNormalID;
         
        PLuint _donorSiteCountID;         
        PLuint _donorSiteTotalGridPointsID;
        PLuint _donorSiteTriangleCountsID;
        PLuint _donorSiteGridPointCountsID;
        PLuint _donorSiteBoundaryPointCountsID;
        PLuint _donorSiteDataOffsetsID;
          
        PLuint _defectSolutionGraftCountID;
        PLuint _defectSolutionGraftPositionsID;
        PLuint _defectSolutionGraftNormalsID;
        PLuint _defectSolutionGraftRadiiID;
        PLuint _defectSolutionSurfaceNormalsID;         
        
        PLuint _rotationIndexID;
        PLuint _rotationAnglesID;       
};



#endif
