#pragma once

#include "plCommon.h"
#include "plPlannerShader.h"
#include "plPlannerStage0.h"
#include "plPlannerStructs.h"
#include "plSSBO.h"
#include "plVector4.h"

#define PL_STAGE_1_GROUP_SIZE 1024

namespace plPlannerStage1 {
void run(
    std::shared_ptr<plCapIndices> capData,
    std::shared_ptr<plPlanningBufferData> planningData,
    std::shared_ptr<plDefectSolution> defectSolution);
}
