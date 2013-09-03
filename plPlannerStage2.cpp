#include "plPlannerStage2.h"

namespace plPlannerStage2
{

    plSeq<plVector4> run( const plSeq<plSiteGrid> &donorSites, const plAnnealingState &state, const plSeq<PLfloat> &rmsInput )
    {
        const PLuint DONOR_OUTPUT_BUFFER_SIZE = PL_STAGE2_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION*2;

        // compile / link stage 1 shader
        plPlannerStage2Shader stage2Shader("./shaders/plannerStage2.comp");
        stage2Shader.bind(); 
              
        // calc total grid points (invocations)
        PLuint totalGridPoints = 0;
        plSeq<PLuint> donorGridSizes;
        for (PLuint i=0; i < donorSites.size(); i++)
        {
            totalGridPoints += donorSites[i].gridSize();
            donorGridSizes.add ( donorSites[i].gridSize()  );
        }

        // generate and fill buffers 
        PLuint donorSiteDataBufferID  = getGroupGridSSBO( donorSites ); //donorSites[0].getGridSSBO();         
        PLuint rmsBuffer              = createSSBO( rmsInput );
        PLuint donorOutputBufferID    = createSSBO( DONOR_OUTPUT_BUFFER_SIZE, plVector4(-1,-1,-1,-1) );
        PLuint totalRmsOutputBufferID = createSSBO( PL_STAGE2_INVOCATIONS, FLT_MAX );
   
        // bind buffers
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, donorSiteDataBufferID  );    
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, rmsBuffer              );  
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, donorOutputBufferID    );  
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, totalRmsOutputBufferID );
        
        // set state uniforms
        stage2Shader.setGraftUniforms( state.graftCount,
                                       state.graftRadii );

        stage2Shader.setSiteUniforms( donorSites.size(),
                                      totalGridPoints,
                                      donorGridSizes);
                                      
        // call compute shader with 1D workgrouping
        glDispatchCompute( PL_STAGE2_NUM_GROUPS, 1, 1 );
        
        // memory barrier      
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        // copy total rms into client side array
        plSeq<PLfloat> totalRmsData( PL_STAGE2_INVOCATIONS, -1.0f ); 
                      
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, totalRmsOutputBufferID);            
        PLfloat *totalRms = readSSBO<PLfloat>( 0, PL_STAGE2_INVOCATIONS );
        memcpy( &totalRmsData[0], &totalRms[0], PL_STAGE2_INVOCATIONS*sizeof( PLfloat ) );    
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        // find invocation with lowest RMS
        float minRMS   = FLT_MAX;
        PLint minIndex = -1;    

        for (PLuint i=0; i < PL_STAGE2_INVOCATIONS; i++)
        {
            if ( totalRmsData[i] > 0 && totalRmsData[i] < minRMS )
            {
                minRMS   = totalRmsData[i];
                minIndex = i;
            }
        }

        // copy donor data into client side array
        plSeq<plVector4> donorData( state.graftCount*2, plVector4(-1,-1,-1,-1) ); 
            
        // check if solution exists
        if ( minIndex != -1 )
        {                
            std::cout << "\tTotal RMS: " << minRMS << "\n";     
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, donorOutputBufferID);            
            plVector4 *donors = readSSBO<plVector4>( minIndex*PL_MAX_GRAFTS_PER_SOLUTION*2, state.graftCount*2 );
            memcpy( &donorData[0], &donors[0], state.graftCount*2*sizeof( plVector4 ) );    
            glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        }
        else
        {
            std::cerr << "Error: Unable to find suitable harvest locations \n";
            return plSeq<plVector4>();
        }
        
        // unbind buffers
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 );           
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, 0 );    
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, 0 );
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, 0 ); 
        
        // delete buffers
        glDeleteBuffers( 1, &donorSiteDataBufferID  );
        glDeleteBuffers( 1, &rmsBuffer              );
        glDeleteBuffers( 1, &donorOutputBufferID    );
        glDeleteBuffers( 1, &totalRmsOutputBufferID );        
        
        std::cout << "\tDonor locations and normals:\n";    
        for (PLuint i=0; i < donorData.size()/2; i++)
        {
            std::cout << "\t\tPosition " << i << ": \t" << donorData[ i*2 + 0] << "\n";
            std::cout << "\t\tNormal " << i << ": \t" << donorData[ i*2 + 1] << "\n";
        }
        
        return donorData;
        
    }
    

}























