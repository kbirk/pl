#ifndef PL_STAGE_2_STATE_H
#define PL_STAGE_2_STATE_H

#include "plCommon.h"

#include "plVector4.h"

#include "plPlannerStage1.h"
#include "plPlannerStage2Shader.h"

#define PL_STAGE_2_GROUP_SIZE                   1024


class plRmsData
{
    public:
    
        plSSBO rmsSSBO;
        plSSBO rotationSSBO;

        plRmsData() {}  
};


namespace plPlannerStage2
{
    void run( plRmsData &rmsData, const plPlanningBufferData &planningData, const plDefectSolution &defectSolution, const plCapIndices &capIndices ); 
}


#endif

