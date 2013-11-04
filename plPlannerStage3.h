#ifndef PL_STAGE_3_STATE_H
#define PL_STAGE_3_STATE_H

#include "plCommon.h"

#include "plVector4.h"

#include "plPlannerStage0.h"
#include "plPlannerStage2.h"
#include "plPlannerStage3Shader.h"

#define PL_STAGE_3_ITERATIONS                 8
#define PL_STAGE_3_GROUP_SIZE                 256
#define PL_STAGE_3_NUM_GROUPS                 1
#define PL_STAGE_3_INVOCATIONS                PL_STAGE_3_NUM_GROUPS*PL_STAGE_3_GROUP_SIZE

class plDonorSolution
{
    public:
     
        std::vector<plVector4>  graftPositions;
        std::vector<plVector4>  graftNormals;
        float                   lowestRMS;

        plDonorSolution() 
            : lowestRMS( FLT_MAX ) 
        {}

        void extractBestSolution( PLuint graftCount, const plSSBO &totalRmsSSBO, const plSSBO &donorSolutionPositionsSSBO, const plSSBO &donorSolutionNormalsSSBO );
       
};


namespace plPlannerStage3
{
    void run( plDonorSolution &donorSolution, const plPlanningBufferData &planningData, const plDefectSolution &defectSolution, const plRmsData &rmsInput );
}


#endif

