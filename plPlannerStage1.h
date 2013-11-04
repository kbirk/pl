#ifndef PL_STAGE_1_STATE_H
#define PL_STAGE_1_STATE_H

#include "plCommon.h"
#include "plVector4.h"

#include "plPlannerStructs.h"
#include "plPlannerStage0.h"
#include "plPlannerStage1Shader.h"
#include "plSSBO.h"

#define PL_STAGE_1_GROUP_SIZE                   1024


namespace plPlannerStage1
{
    void run( plCapIndices &capData, const plPlanningBufferData &planningData, const plDefectSolution &defectSolution );     
}


#endif

