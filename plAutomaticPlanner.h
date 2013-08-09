#ifndef __PL_AUTOMATIC_PLANNER_H__
#define __PL_AUTOMATIC_PLANNER_H__

#include "plCommon.h"
#include "plSeq.h"
#include "plVector3.h"
#include "plTransform.h"
#include "plPlan.h"

#include "plMesh.h"

#include "plSiteGrid.h"
#include "plPlannerStage0Shader.h"
#include "plPlannerStage1Shader.h"

class plAutomaticPlanner
{
    public:

        static void calculate( plPlan &plan );
           
        
               
    //private:
    
        plAutomaticPlanner();
            
           
        // defectsite grid
        static PLuint        _gridPointsTextureID; 
        static PLuint        _gridNormalsTextureID; 
        
        // defectsite mesh                  
        static PLuint        _siteMeshTextureID;
        
        // possible graft caps and respective areas
        static PLuint        _potentialGraftCapsID;
        static PLuint        _potentialGraftAreasID;   
           
        static PLuint        _stateEnergiesTextureID;
        static PLuint        _stateCapIndicesTextureID;
              
        static plSeq<plSiteGrid>  _donorSiteGrids;
        static plSeq<plSiteGrid>  _defectSiteGrids;
        
        static void _generateSiteGrids( plPlan &plan );        
        
        static void _bufferTextures();   

    
        static void _dispatch();      
        static void _dispatchStage0();
        
         
        static plSeq<plMesh> DEBUG_MESH;  
                        
};

#endif

