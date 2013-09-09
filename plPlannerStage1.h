#ifndef __PL_STAGE_1_STATE_H__
#define __PL_STAGE_1_STATE_H__

#include "plCommon.h"
#include "plSeq.h"
#include "plVector4.h"

#include "plSiteGrid.h"
#include "plPlannerStage0.h"
#include "plPlannerStage1Shader.h"

#define PL_STAGE1_GROUP_SIZE                   256

#define PL_STAGE1_NUM_SLICES                   16


#define PL_STAGE1_MAX_DONOR_SITES              5
#define PL_STAGE1_MAX_CAP_TRIANGLES            1024


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
        
        void update( PLuint bufferSize, PLuint rmsBuffer, const plVector4 &dir )
        {
            plRmsSet set( bufferSize, dir );   
        
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, rmsBuffer);            
            PLfloat *rms = readSSBO<PLfloat>( 0, bufferSize );
            memcpy( &set.rms[0], &rms[0], bufferSize*sizeof( PLfloat ) );    
            glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

            sets.add( set );
        }      

};

namespace plPlannerStage1
{

    plRmsData run( const plSiteGrid &defectSite, const plSeq<plSiteGrid> &donorSites, const plDefectState &defectState );
      
}


#endif

