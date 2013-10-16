#ifndef PL_STAGE_3_STATE_H
#define PL_STAGE_3_STATE_H

#include "plCommon.h"
#include "plSeq.h"
#include "plVector4.h"

#include "plSiteGrid.h"
#include "plPlannerStage0.h"
#include "plPlannerStage2.h"
#include "plPlannerStage3Shader.h"

#define PL_STAGE3_ITERATIONS                 8
#define PL_STAGE3_GROUP_SIZE                 256
#define PL_STAGE3_NUM_GROUPS                 1
#define PL_STAGE3_INVOCATIONS                PL_STAGE3_NUM_GROUPS*PL_STAGE3_GROUP_SIZE

class plDonorState
{
    public:
     
        plSeq<plVector4>  graftPositions;
        plSeq<plVector4>  graftNormals;
        plSeq<plVector4>  graftZDirections;
    
        float             totalRms;
    
        plDonorState();
        
        ~plDonorState();

        void update(); 
        
    private:
    
        PLuint _donorPositionsBufferID;
        PLuint _donorNormalsBufferID;
        PLuint _donorZDirectionsBufferID;       
        PLuint _totalRMSBufferID; 
        
        void _createBuffers ();
        void _destroyBuffers();
        
        void _bindBuffers  ();
        void _unbindBuffers();
          
        PLint  _getLowestRmsIndex(); 
            
};


namespace plPlannerStage3
{
    plDonorState run( const plSeq<plSiteGrid> &donorSites, const plDefectState &defectState, const plRmsData &rmsInput );      
}


#endif

