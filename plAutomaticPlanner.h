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

#define PL_ANNEALING_INITIAL_TEMPERATURE        1.0f
#define PL_ANNEALING_COOLING_RATE               0.025f

#define PL_ANNEALING_THREADS                    512

#define PL_MAX_GRAFTS_PER_SOLUTION              20


class plAutomaticPlanner
{
    public:

        static void calculate( plPlan &plan );
           
        static plSeq<plVector3> DEBUG_GRAFT_LOCATIONS; 
               
    //private:
    
        plAutomaticPlanner();
                             
        // defect site textures        
        static PLuint _siteDataTextureID;

        // mesh colouring temporary texture       
        static PLuint _overlappedTrianglesBufferID;
                   
        // annealing states and energies
        static PLuint _stateEnergiesTextureID;
        static PLuint _stateGraftPositionsTextureID;
        static PLuint _stateGraftNormalsTextureID;
        static PLuint _stateGraftRadiiTextureID;
        static PLuint _stateGraftCountsTextureID;
                               
        static plSeq<plSiteGrid>  _donorSiteGrids;
        static plSeq<plSiteGrid>  _defectSiteGrids;
        
        static void _generateSiteGrids( plPlan &plan );                
        static void _bufferTextures();   
    
        static void _dispatch();      
        static void _dispatchStage0();

                        
};

#endif

