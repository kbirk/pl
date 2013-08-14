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

#define PL_ANNEALING_INITIAL_TEMPERATURE        1000.0f
#define PL_ANNEALING_COOLING_RATE               0.005f

#define PL_MAX_GRAFTS_PER_SOLUTION              20
#define PL_MAX_GRAFT_CAP_TRIANGLES              1000


class plAutomaticPlanner
{
    public:

        static void calculate( plPlan &plan );
           
        static plSeq<PLuint>stateGrafts; 
               
    //private:
    
        plAutomaticPlanner();
                       
        // defect site grid
        static PLuint _gridPointsTextureID; 
        static PLuint _gridNormalsTextureID; 
        
        // defect site mesh                  
        static PLuint _siteMeshTextureID;
        
        // possible graft caps and respective areas
        static PLuint _potentialGraftCapsID;
        static PLuint _potentialGraftAreasID;   
        static PLuint _potentialGraftTrianglesID; 
           
        // annealing states and energies
        static PLuint _stateEnergiesTextureID;
        static PLuint _stateGraftIndicesTextureID;
              
        // mesh colouring temporary texture      
        static PLuint       _siteTriangleAreasTextureID;
                      
        static plSeq<plSiteGrid>  _donorSiteGrids;
        static plSeq<plSiteGrid>  _defectSiteGrids;
        
        static void _generateSiteGrids( plPlan &plan );                
        static void _bufferTextures();   
    
        static void _dispatch();      
        static void _dispatchStage0();
        static void _dispatchStage1();
         
        static plSeq<plMesh> DEBUG_MESH;  
                        
};

#endif

