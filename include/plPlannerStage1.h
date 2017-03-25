#pragma once

#include "plCommon.h"
#include "plVector4.h"

#include "plPlannerStructs.h"
#include "plPlannerStage0.h"
#include "plPlannerShader.h"
#include "plSSBO.h"

#define PL_STAGE_1_GROUP_SIZE 1024

namespace plPlannerStage1
{
    void run(
        std::shared_ptr<plCapIndices> capData,
        std::shared_ptr<plPlanningBufferData> planningData,
        std::shared_ptr<plDefectSolution> defectSolution);
}
