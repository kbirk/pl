#ifndef PL_RENDERER_H
#define PL_RENDERER_H

#include "plCommon.h"
#include "plSeq.h"
#include "plVector3.h"
#include "plRenderingPipeline.h"

#include "plPhongShader.h"
#include "plMinimalShader.h"
#include "plPickingShader.h"
#include "plTexture2DShader.h"

#include "plPlan.h"
#include "plGraftEditor.h"
#include "plBoundaryEditor.h"
#include "plOctree.h"
#include "plTexture2DMesh.h"
#include "plAutomaticPlanner.h"
#include "plChessBoard.h"
#include "plScan.h"
#include "plDebugSphere.h"
#include "plLaserLine.h"

#include "plPlannerStage1.h"
#include "plPlannerStage2.h"


namespace plRenderer
{  
    void init(); 
      
    void draw( PLuint x, PLuint y );  
     
    void queue ( const plPlan           &plan   );
    void queue ( const plGraftEditor    &editor );
    void queue ( const plBoundaryEditor &editor );
    void queue ( const plTrackedObject  &object );
    void queue ( const plTexture2DMesh  &arthroTexture );
    void queue ( const plChessBoard     &chessboard );
    void queue ( const plScan           &scan );

    void queue ( plDebugSphere      debugSphere );
    void queue ( const std::vector <plDebugSphere> &debugSpheres );
    void queue ( plLaserLine laserLine );
    void queue ( const std::vector <plLaserLine> &laserLine );
    
}











#endif

