#ifndef PL_STAGE_2_STATE_H
#define PL_STAGE_2_STATE_H

#include "plCommon.h"

#include "plVector4.h"
#include "plPlannerStructs.h"
#include "plPlannerStage1.h"
#include "plPlannerShader.h"

#define PL_STAGE_2_GROUP_SIZE                   1024


namespace plPlannerStage2
{
    void run( plRmsData &rmsData, const plPlanningBufferData &planningData, const plDefectSolution &defectSolution, const plCapIndices &capIndices ); 
}


#endif

