#pragma once

#include "plCommon.h"
#include "plUtility.h"
#include "plVector4.h"
#include "plSSBO.h"

class plDefectSolution
{
    public:

        uint32_t                  graftCount;
        std::vector<plVector4>  graftPositions;
        std::vector<plVector4>  graftNormals;         // normal / direction of graft
        std::vector<plVector4>  graftSurfaceNormals;  // actual normal of the surface
        std::vector<float32_t>    graftRadii;

        plDefectSolution() {};
};


class plCapIndices
{
    public:

        plSSBO defectCapIndexSSBO;
        plSSBO donorCapIndexSSBO;

        plCapIndices() {};
};


class plRmsData
{
    public:

        plSSBO rmsSSBO;
        plSSBO rotationSSBO;

        plRmsData() {}
};


class plDonorSolution
{
    public:

        std::vector<plVector4>  graftPositions;
        std::vector<plVector4>  graftNormals;
        std::vector<plVector4>  graftSurfaceNormals;
        std::vector<plVector4>  graftXAxes;
        std::vector<uint32_t>     graftSiteIndices;
        float32_t                   rms;

        plDonorSolution() {};

};
