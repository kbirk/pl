#ifndef __PL_STAGE_0_STATE_H__
#define __PL_STAGE_0_STATE_H__

#include "plCommon.h"
#include "plUtility.h"
#include "plSeq.h"
#include "plVector4.h"

#include "plSiteGrid.h"
#include "plPlannerStage0Shader.h"

#define PL_MAX_GRAFTS_PER_SOLUTION           16

#define PL_STAGE0_INITIAL_TEMPERATURE        1.0f
#define PL_STAGE0_COOLING_RATE               0.05f

#define PL_STAGE0_GROUP_SIZE                 16
#define PL_STAGE0_NUM_GROUPS                 16
#define PL_STAGE0_INVOCATIONS                PL_STAGE0_NUM_GROUPS*PL_STAGE0_GROUP_SIZE


class plDefectState
{
    public:
    
        PLfloat           temperature;
        PLfloat           energy;    
        PLuint            graftCount;  
        plSeq<plVector4>  graftPositions;
        plSeq<plVector4>  graftNormals;
        plSeq<PLfloat>    graftRadii;
    
        plDefectState( float initialEnergy );
        
        void createBuffers ();
        void destroyBuffers();
        
        void bindBuffers  ();
        void unbindBuffers();
        
        void update(); 
         
    private:
            
        PLuint _energiesBufferID;
        PLuint _graftPositionsBufferID;
        PLuint _graftNormalsBufferID;
        PLuint _graftRadiiBufferID;
        PLuint _graftCountsBufferID;

        PLfloat _acceptanceProbability( PLfloat energy, PLfloat newEnergy, PLfloat temperature ); 

        PLint _updateEnergy  ();
        void  _updateGrafts  ( PLint index );               
};


namespace plPlannerStage0
{

    plDefectState run( const plSiteGrid &site );

}


#endif

