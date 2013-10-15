#ifndef __PL_STAGE_2_STATE_H__
#define __PL_STAGE_2_STATE_H__

#include "plCommon.h"
#include "plSeq.h"
#include "plVector4.h"

#include "plSiteGrid.h"
#include "plPlannerStage1.h"
#include "plPlannerStage2Shader.h"

#define PL_STAGE2_GROUP_SIZE                   1024

class plRmsSet
{
    public:
    
        plVector4     direction;
        plSeq<float>  rms;
        
        plRmsSet() {};        
        plRmsSet( PLuint size, const plVector4 &dir ) 
            : rms( size, -1.0f ), direction( dir )
        {}
};

class plRmsData
{
    public:
    
        plSeq<plRmsSet> sets;

        plRmsData() {}  
        
        void update( PLuint bufferSize, PLuint rmsBuffer, const plVector4 &dir );

        PLuint getValuesSSBO()     const;
        PLuint getDirectionsSSBO() const;
};

namespace plPlannerStage2
{
    void run( plRmsData &rmsData, const plSiteGrid &defectSite, const plSeq<plSiteGrid> &donorSites, const plDefectState &defectState, const plCapIndices &capIndices );      
}


#endif

