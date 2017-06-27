#pragma once

#include "plCommon.h"
#include "plSSBO.h"
#include "plUtility.h"
#include "plVector4.h"

class plDefectSolution {
public:
    uint32_t graftCount;
    std::vector<plVector4> graftPositions;
    std::vector<plVector4> graftNormals; // normal of graft
    std::vector<plVector4> graftSurfaceNormals; // actual normal of the surface
    std::vector<float32_t> graftRadii;

    plDefectSolution(){};
};

class plCapIndices {
public:
    std::shared_ptr<plSSBO> defectCapIndexSSBO;
    std::shared_ptr<plSSBO> donorCapIndexSSBO;

    plCapIndices()
    {
        defectCapIndexSSBO = std::make_shared<plSSBO>();
        donorCapIndexSSBO = std::make_shared<plSSBO>();
    };
};

class plRmsData {
public:
    std::shared_ptr<plSSBO> rmsSSBO;
    std::shared_ptr<plSSBO> rotationSSBO;

    plRmsData()
    {
        rmsSSBO = std::make_shared<plSSBO>();
        rotationSSBO = std::make_shared<plSSBO>();
    }
};

class plDonorSolution {
public:
    std::vector<plVector4> graftPositions;
    std::vector<plVector4> graftNormals;
    std::vector<plVector4> graftSurfaceNormals;
    std::vector<plVector4> graftXAxes;
    std::vector<uint32_t> graftSiteIndices;
    float32_t rms;

    plDonorSolution(){};
};
