#ifndef __PL_STAGE_1_STATE_H__
#define __PL_STAGE_1_STATE_H__

#include "plCommon.h"
#include "plSeq.h"
#include "plVector4.h"

#include "plSiteGrid.h"
#include "plPlannerStage0.h"
#include "plPlannerStage1Shader.h"

#define PL_STAGE1_GROUP_SIZE                           256

#define PL_STAGE1_MAX_DONOR_SITES                      5
#define PL_STAGE1_MAX_CAP_TRIANGLES                    500

namespace plPlannerStage1
{

    void run( const plSiteGrid &defectSite, const plSeq<plSiteGrid> &donorSites, const plAnnealingState &state );
      
}


#endif

