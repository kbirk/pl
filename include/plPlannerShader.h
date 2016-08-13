#pragma once

#include "plCommon.h"
#include "plShader.h"
#include "plPlanningSite.h"
#include "plPlannerStructs.h"

class plPlannerShader : public plShader
{

    public:

        plPlannerShader(const std::vector<std::string > &sourceFiles);

        // global uniform setters
        void setDefectSiteUniforms     (const plPlanningSite &defectSite) const;
        void setDonorSiteUniforms      (const std::vector<plPlanningSite> &donorSites) const;
        void setDefectSolutionUniforms (const plDefectSolution &solution) const;
        void setSeedUniform() const;

        // stage0 uniform setters
        void setTemperatureUniform (float32_t temperature) const;
        void setLocalLoadUniform   (uint32_t loadLocal) const;

        // stage2-3 uniform setters
        void setRotationAngleUniforms (uint32_t numDirections) const;
        void setRotationIndexUniform  (uint32_t rotationIndex) const;

    protected:

        // global uniforms
        uint32_t _defectSiteGridPointCountID;
        uint32_t _defectSiteBoundaryPointCountID;
        uint32_t _defectSiteTriangleCountID;
        uint32_t _defectSiteAreaID;
        uint32_t _defectSiteAvgNormalID;

        uint32_t _donorSiteCountID;
        uint32_t _donorSiteTotalGridPointsID;
        uint32_t _donorSiteTriangleCountsID;
        uint32_t _donorSiteGridPointCountsID;
        uint32_t _donorSiteBoundaryPointCountsID;
        uint32_t _donorSiteDataOffsetsID;

        uint32_t _defectSolutionGraftCountID;
        uint32_t _defectSolutionGraftPositionsID;
        uint32_t _defectSolutionGraftNormalsID;
        uint32_t _defectSolutionGraftRadiiID;
        uint32_t _defectSolutionSurfaceNormalsID;

        uint32_t _seedID;

        // stage 0 uniforms
        uint32_t _temperatureID;
        uint32_t _loadLocalID;

        // stage 2-3 uniforms
        uint32_t _rotationIndexID;
        uint32_t _rotationAnglesID;

        void _getUniformLocations();
};
