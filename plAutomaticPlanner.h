#ifndef PL_AUTOMATIC_PLANNER_H
#define PL_AUTOMATIC_PLANNER_H

#include "plCommon.h"
#include "plSeq.h"
#include "plPlan.h"
#include "plSiteGrid.h"

#include "plPlannerStage0.h"
#include "plPlannerStage1.h"
#include "plPlannerStage2.h"
#include "plPlannerStage3.h"

namespace plAutomaticPlanner
{
    void calculate( plPlan &plan );
      
    // TEMPORARY FOR DEBUG, after will be private  
    extern plSeq<plSiteGrid>  _donorSiteGrids;
    extern plSeq<plSiteGrid>  _defectSiteGrids;                  
}


#endif

