#ifndef __PL_STAGE_2_STATE_H__
#define __PL_STAGE_2_STATE_H__

#include "plCommon.h"
#include "plSeq.h"
#include "plVector4.h"

#include "plSiteGrid.h"
#include "plPlannerStage0.h"
#include "plPlannerStage1.h"
#include "plPlannerStage2Shader.h"

#define PL_STAGE2_GROUP_SIZE                 256
#define PL_STAGE2_NUM_GROUPS                 16
#define PL_STAGE2_INVOCATIONS                PL_STAGE2_NUM_GROUPS*PL_STAGE2_GROUP_SIZE


namespace plPlannerStage2
{
    plSeq<plVector4> run( const plSeq<plSiteGrid> &donorSites, const plAnnealingState &state, const plSeq<PLfloat> &rmsInput );
      
}


#endif

