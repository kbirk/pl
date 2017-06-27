#pragma once

#include "plCommon.h"
#include "plPlannerShader.h"
#include "plPlannerStage1.h"
#include "plPlannerStructs.h"
#include "plVector4.h"

#define PL_STAGE_2_GROUP_SIZE 1024

namespace plPlannerStage2 {
void run(
    std::shared_ptr<plRmsData> rmsData,
    std::shared_ptr<plPlanningBufferData> planningData,
    std::shared_ptr<plDefectSolution> defectSolution,
    std::shared_ptr<plCapIndices> capIndices);
}
