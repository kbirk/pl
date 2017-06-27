#pragma once

#include "plCommon.h"
#include "plPlan.h"
#include "plPlannerStage0.h"
#include "plPlannerStage1.h"
#include "plPlannerStage2.h"
#include "plPlannerStage3.h"
#include "plPlanningBufferData.h"
#include "plTimer.h"

namespace plAutomaticPlanner {
void calculate(std::shared_ptr<plPlan> plan, uint32_t defectSiteIndex);
}
