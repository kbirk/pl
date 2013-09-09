#ifndef __PL_STAGE_2_STATE_H__
#define __PL_STAGE_2_STATE_H__

#include "plCommon.h"
#include "plSeq.h"
#include "plVector4.h"

#include "plSiteGrid.h"
#include "plPlannerStage0.h"
#include "plPlannerStage1.h"
#include "plPlannerStage2Shader.h"

#define PL_STAGE2_GROUP_SIZE                 256
#define PL_STAGE2_NUM_GROUPS                 16
#define PL_STAGE2_INVOCATIONS                PL_STAGE2_NUM_GROUPS*PL_STAGE2_GROUP_SIZE

class plDonorState
{
    public:
     
        plSeq<plVector4>  graftPositions;
        plSeq<plVector4>  graftNormals;
        plSeq<plVector4>  graftZDirections;
    
        plDonorState();
        
        void createBuffers ();
        void destroyBuffers();
        
        void bindBuffers  ();
        void unbindBuffers();
        
        void update(); 
        
    private:
    
        PLuint _donorPositionsBufferID;
        PLuint _donorNormalsBufferID;
        PLuint _donorZDirectionsBufferID;       
        PLuint _totalRMSBufferID; 
          
        PLint  _getLowestRmsIndex(); 
            
};


namespace plPlannerStage2
{
    plDonorState run( const plSeq<plSiteGrid> &donorSites, const plDefectState &defectState, const plRmsData &rmsInput );      
}


#endif

