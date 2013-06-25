#include "plState.h"
 
plState::plState()
{
    modelSelectedID         = -1;
    
    graftEditMode           = PL_GRAFT_EDIT_MODE_TRANSLATE;
    graftEditAxis           = plVector3(1,0,0);
    graftSelectedIndex      = -1; 
    graftSelectedID         = -1;
    graftHandlesEnabled     = true;
   
    boundarySelectedType    = -1;   
    boundarySelectedID      = -1;      
    boundarySelectedPointID = -1;
    
    splineTangentFactor     = 15;
    splineSelectedID        = -1;
}
 
  
void plState::selectNothing()
{
    // clear all selections, except model (always retained)  
    graftSelectedID         = -1;              
    graftSelectedIndex      = -1; 
    boundarySelectedType    = -1;
    boundarySelectedID      = -1;
    boundarySelectedPointID = -1;
    splineSelectedID        = -1;
}    


void plState::selectGraft(PLuint id, PLuint index)
{
    // clear all, select graft
    selectNothing();
    graftSelectedID         = id;              
    graftSelectedIndex      = index; 
}  


void plState::selectModel(PLuint id)
{
    selectNothing();
    modelSelectedID = id;
}  


void plState::selectBoundary(PLuint type, PLuint id)
{
    selectNothing();
    boundarySelectedType = type;
    boundarySelectedID = id;    
}


void plState::selectBoundaryPoint(PLuint index)
{
    boundarySelectedPointID = index;
}


void plState::selectBoundaryPoint(PLuint type, PLuint id, PLuint index)
{
    selectNothing();
    boundarySelectedType = type;
    boundarySelectedID = id;
    boundarySelectedPointID = index;
}


void plState::selectSpline(PLuint id)
{
    selectNothing();
    splineSelectedID = id;
}



