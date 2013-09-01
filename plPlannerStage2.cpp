#include "plPlannerStage2.h"

namespace plPlannerStage2
{

    plSeq<plVector4> run( const plSeq<plSiteGrid> &donorSites, const plAnnealingState &state )
    {
        // compile / link stage 1 shader
        plPlannerStage2Shader stage2Shader("./shaders/plannerStage2.comp");
        stage2Shader.bind(); 
        
        
        // calc total grid points (invocations)
        PLuint totalGridPoints = 0;
        plSeq<PLuint> donorGridSizes;
        for (PLuint i=0; i < 1; i++) //donorSites.size(); i++)
        {
            totalGridPoints += donorSites[i].gridSize();
            donorGridSizes.add ( donorSites[i].gridSize()  );
        }

        // generate and fill buffers 
        PLuint donorSiteDataBufferID = donorSites[0].getGridSSBO();         
        PLuint rmsBuffer             = createSSBO( totalGridPoints*PL_MAX_GRAFTS_PER_SOLUTION, -1.0f );
        PLuint donorOutputBufferID   = createSSBO( PL_MAX_GRAFTS_PER_SOLUTION*2, plVector4(-1,-1,-1,-1) );
   
        // bind buffers
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, donorSiteDataBufferID );    
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, rmsBuffer             );  
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, donorOutputBufferID   );  

        PLuint workgroups = ceil( totalGridPoints / (PLfloat) PL_STAGE2_GROUP_SIZE); // ensure enough workgroups are used

        // set state uniforms
        stage2Shader.setGraftUniforms( state.graftCount,
                                       state.graftRadii );

        stage2Shader.setSiteUniforms( 1, //donorSites.size(),
                                      donorGridSizes);
                                      
        // call compute shader with 1D workgrouping
        glDispatchCompute( workgroups, 1, 1 );
        
        // memory barrier      
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
                                         

        // copy donor data into independant buffer
        plSeq<plVector4> donorData( PL_MAX_GRAFTS_PER_SOLUTION*2, plVector4(-1,-1,-1,-1) ); 
                      
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, donorOutputBufferID);            
        plVector4 *donors = readSSBO<plVector4>( 0, PL_MAX_GRAFTS_PER_SOLUTION*2 );
        memcpy( &donorData[0], &donors[0], PL_MAX_GRAFTS_PER_SOLUTION*2*sizeof( plVector4 ) );    
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        // unbind buffers
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 );           
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, 0 );    
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, 0 );

        // delete buffers
        glDeleteBuffers( 1, &donorSiteDataBufferID  );
        glDeleteBuffers( 1, &rmsBuffer              );
        glDeleteBuffers( 1, &donorOutputBufferID    );
        
        return donorData;
        
    }
    

}























