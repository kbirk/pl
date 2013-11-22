#ifndef PL_PLANNER_SHADER_H
#define PL_PLANNER_SHADER_H

#include "plCommon.h"
#include "plShader.h"
#include "plPlanningSite.h"
#include "plPlannerStructs.h"

class plPlannerShader : public plShader
{

    public:                    
          
        plPlannerShader( const std::vector< std::string > &sourceFiles ); 
		
        // global uniform setters
        void setDefectSiteUniforms     ( const plPlanningSite &defectSite ) const;
        void setDonorSiteUniforms      ( const std::vector<plPlanningSite> &donorSites ) const;
        void setDefectSolutionUniforms ( const plDefectSolution &solution ) const;
        void setSeedUniform() const;

        // stage0 uniform setters
        void setTemperatureUniform ( PLfloat temperature ) const;        
        void setLocalLoadUniform   ( PLuint loadLocal ) const;

        // stage2-3 uniform setters
        void setRotationAngleUniforms ( PLuint numDirections ) const;
        void setRotationIndexUniform  ( PLuint rotationIndex ) const;

    protected:

        // global uniforms
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
        
        PLuint _seedID;

        // stage 0 uniforms
        PLuint _temperatureID;
        PLuint _loadLocalID;
        
        // stage 2-3 uniforms
        PLuint _rotationIndexID;
        PLuint _rotationAnglesID; 
      
		void _getUniformLocations();
};


#endif
