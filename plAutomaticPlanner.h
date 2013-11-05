#ifndef PL_AUTOMATIC_PLANNER_H
#define PL_AUTOMATIC_PLANNER_H

#include "plCommon.h"

#include "plPlan.h"

#include "plPlanningBufferData.h"
#include "plPlannerStage0.h"
#include "plPlannerStage1.h"
#include "plPlannerStage2.h"
#include "plPlannerStage3.h"

/*
    TODO:
    
        - store donor x , and defect x
        
        - boundary position is valid checks
        
        - use deviation from defect site normal as a penalty function
        
*/



namespace plAutomaticPlanner
{
    void calculate( plPlan &plan );                    
}


#endif

