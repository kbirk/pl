#ifndef PL_STAGE_0_STATE_H
#define PL_STAGE_0_STATE_H

#include "plCommon.h"
#include "plUtility.h"

#include "plVector4.h"
#include "plPlannerStructs.h"
#include "plPlannerStage0Shader.h"
#include "plPlanningBufferData.h"

#include "plRenderer.h"

#include "plPlan.h"

#define PL_MAX_GRAFTS_PER_SOLUTION           16
#define PL_MAX_DONOR_SITES                   5
#define PL_MAX_CAP_TRIANGLES                 2048
#define PL_NUM_COMPARISION_DIRECTIONS        32

#define PL_STAGE_0_ITERATIONS                 5
#define PL_STAGE_0_GROUP_SIZE                 64
#define PL_STAGE_0_NUM_GROUPS                 16
#define PL_STAGE_0_INVOCATIONS                PL_STAGE_0_NUM_GROUPS*PL_STAGE_0_GROUP_SIZE

#define PL_STAGE_0_INITIAL_TEMPERATURE        1.0f
#define PL_STAGE_0_COOLING_RATE               0.05f //0.00175f


class plAnnealingGroup
{
    public:
            
        plAnnealingGroup( PLfloat initialEnergy );        
       
        void bind();
        void unbind();

        void getSolution( plDefectSolution &solution, const plPlanningBufferData &planningData );
        void getBestGroupInfo( PLuint *index, PLfloat *energy, PLuint *graftCount );

    private:
            
        plSSBO _invoEnergiesSSBO;
        plSSBO _invoGraftPositionsSSBO;
        plSSBO _invoGraftNormalsSSBO;
        plSSBO _invoGraftRadiiSSBO;
        plSSBO _invoGraftCountsSSBO;

        plSSBO _groupEnergiesSSBO;
        plSSBO _groupGraftPositionsSSBO;
        plSSBO _groupGraftNormalsSSBO;
        plSSBO _groupGraftRadiiSSBO;
        plSSBO _groupGraftCountsSSBO;
      
};


namespace plPlannerStage0
{

    void run( plDefectSolution &state, const plPlanningBufferData &planningData, plPlan& plan );

}




#endif

