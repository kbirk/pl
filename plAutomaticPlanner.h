#ifndef __PL_AUTOMATIC_PLANNER_H__
#define __PL_AUTOMATIC_PLANNER_H__

#include "plCommon.h"
#include "plSeq.h"
#include "plVector3.h"
#include "plTransform.h"
#include "plPlan.h"

#include "plMesh.h"

#include "plSiteGrid.h"
#include "plPlannerStage0.h"


//#define PL_MAX_GRAFTS_PER_SOLUTION              20

/*
#define PL_ANNEALING_INITIAL_TEMPERATURE        1.0f
#define PL_ANNEALING_COOLING_RATE               0.05f

#define PL_ANNEALING_GROUP_SIZE                 16
#define PL_ANNEALING_NUM_GROUPS                 16
#define PL_ANNEALING_INVOCATIONS                PL_ANNEALING_NUM_GROUPS*PL_ANNEALING_GROUP_SIZE

#define PL_MAX_GRAFTS_PER_SOLUTION              20
*/
#define PL_MAX_DONOR_SITES                      5
#define PL_MAX_CAP_TRIANGLES                    500

/*
class Stage0State
{
    public:
    
        PLfloat           temperature;
        PLfloat           energy;    
        PLuint            graftCount;  
        plSeq<plVector4>  graftPositions;
        plSeq<plVector4>  graftNormals;
        plSeq<PLfloat>    graftRadii;
    
        Stage0State( float initialEnergy )
            : temperature   ( PL_ANNEALING_INITIAL_TEMPERATURE ),    
              energy        ( initialEnergy ),    
              graftCount    ( 0 ),    
              graftPositions( plVector4(-1,-1,-1,-1), PL_MAX_GRAFTS_PER_SOLUTION ),
              graftNormals  ( plVector4(-1,-1,-1,-1), PL_MAX_GRAFTS_PER_SOLUTION ),
              graftRadii    ( -1, PL_MAX_GRAFTS_PER_SOLUTION )
        {        
        }
        
};
*/

namespace plAutomaticPlanner
{
    void calculate( plPlan &plan );
      
    extern plSeq<plSiteGrid>  _donorSiteGrids;
    extern plSeq<plSiteGrid>  _defectSiteGrids;  
           
    extern plSeq<plVector3> DEBUG_GRAFT_LOCATIONS;                   
};

#endif

