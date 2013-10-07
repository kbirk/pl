#ifndef __PL_STAGE_0_STATE_H__
#define __PL_STAGE_0_STATE_H__

#include "plCommon.h"
#include "plUtility.h"
#include "plSeq.h"
#include "plVector4.h"

#include "plSiteGrid.h"
#include "plPlannerStage0Shader.h"

#define PL_MAX_GRAFTS_PER_SOLUTION           16
#define PL_MAX_DONOR_SITES                   5
#define PL_MAX_CAP_TRIANGLES                 2048

#define PL_STAGE0_INITIAL_TEMPERATURE        1.0f
#define PL_STAGE0_COOLING_RATE               0.002f

#define PL_STAGE0_GROUP_SIZE                 64
#define PL_STAGE0_NUM_GROUPS                 16
#define PL_STAGE0_INVOCATIONS                PL_STAGE0_NUM_GROUPS*PL_STAGE0_GROUP_SIZE

#define PL_STAGE0_ITERATIONS                 10


class plDefectState
{
    public:
  
        PLuint            graftCount;  
        plSeq<plVector4>  graftPositions;
        plSeq<plVector4>  graftNormals;
        plSeq<PLfloat>    graftRadii;
    
        plDefectState() {};
}; 


class plAnnealingGroup
{
    public:
            
        plSeq<PLfloat>    energies;    
        plSeq<PLuint>     graftCounts;  
        plSeq<plVector4>  graftPositions;
        plSeq<plVector4>  graftNormals;
        plSeq<PLfloat>    graftRadii;
    
        PLfloat           temperature;
    
        plAnnealingGroup( PLfloat initialEnergy );        
        ~plAnnealingGroup();
        
        void convergeWorkGroups();
        void convergeGlobal();
        
        void updateGroupBuffer() const;
        
        PLint getCurrentBest() const;
        
        void getGlobalSolution( plDefectState &state );
         
    private:
            
        PLuint _groupStateBufferID;    
            
        PLuint _energiesBufferID;
        PLuint _graftPositionsBufferID;
        PLuint _graftNormalsBufferID;
        PLuint _graftRadiiBufferID;
        PLuint _graftCountsBufferID;

        void _createBuffers ();
        void _destroyBuffers();
        
        void _bindBuffers  ();
        void _unbindBuffers();

        PLfloat _acceptanceProbability( PLfloat energy, PLfloat newEnergy ); 

        PLint _updateEnergy     ( PLuint groupIndex);
        void  _updateWorkGroups ( PLuint groupIndex, PLint localIndex );               
};


namespace plPlannerStage0
{

    void run( plDefectState &state, const plSiteGrid &site );

}


#endif

