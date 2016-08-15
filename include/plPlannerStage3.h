#pragma once

#include "plCommon.h"
#include "plVector4.h"
#include "plPlannerStructs.h"
#include "plPlannerStage0.h"
#include "plPlannerStage2.h"
#include "plPlannerShader.h"

#define PL_STAGE_3_ITERATIONS                 8
#define PL_STAGE_3_GROUP_SIZE                 64
#define PL_STAGE_3_NUM_GROUPS                 1
#define PL_STAGE_3_INVOCATIONS                PL_STAGE_3_NUM_GROUPS*PL_STAGE_3_GROUP_SIZE

class plGreedyGroup
{
    public:

        plGreedyGroup();

        void bind();
        void unbind();

        float32_t lowestRMS() const { return _lowestRMS; }
        void update();
        void getSolution(plDonorSolution &solution, const plPlanningBufferData &planningData);

    private:

        float32_t                _lowestRMS;
        std::vector<plVector4> _lowestPositions;
        std::vector<plVector4> _lowestNormals;
        std::vector<plVector4> _lowestXAxes;
        std::vector<uint32_t>    _lowestSiteIndices;

        plSSBO _donorSolutionPositionsSSBO;
        plSSBO _donorSolutionNormalsSSBO;
        plSSBO _donorSolutionXAxesSSBO;
        plSSBO _donorSolutionSiteIndicesSSBO;
        plSSBO _totalRmsSSBO;
};

namespace plPlannerStage3
{
    void run(plDonorSolution &donorSolution, const plPlanningBufferData &planningData, const plDefectSolution &defectSolution, const plRmsData &rmsInput);
}
