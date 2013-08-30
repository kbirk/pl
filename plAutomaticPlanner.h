#ifndef __PL_AUTOMATIC_PLANNER_H__
#define __PL_AUTOMATIC_PLANNER_H__

#include "plCommon.h"
#include "plSeq.h"
#include "plPlan.h"
#include "plSiteGrid.h"

#include "plPlannerStage0.h"
#include "plPlannerStage1.h"

namespace plAutomaticPlanner
{
    void calculate( plPlan &plan );
      
    extern plSeq<plSiteGrid>  _donorSiteGrids;
    extern plSeq<plSiteGrid>  _defectSiteGrids;  
           
    extern plSeq<plVector3> DEBUG_GRAFT_LOCATIONS;                   
}


#endif

