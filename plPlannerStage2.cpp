#include "plPlannerStage2.h"

plDonorState::plDonorState()
    : graftPositions  ( PL_MAX_GRAFTS_PER_SOLUTION, plVector4(-1,-1,-1,-1) ),
      graftNormals    ( PL_MAX_GRAFTS_PER_SOLUTION, plVector4(-1,-1,-1,-1) ),
      graftZDirections( PL_MAX_GRAFTS_PER_SOLUTION, plVector4(-1,-1,-1,-1) )
{
}


void plDonorState::createBuffers()
{
    // create state buffers  
    _donorPositionsBufferID   = createSSBO( PL_STAGE2_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION, plVector4(-1,-1,-1,-1) );
    _donorNormalsBufferID     = createSSBO( PL_STAGE2_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION, plVector4(-1,-1,-1,-1) );
    _donorZDirectionsBufferID = createSSBO( PL_STAGE2_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION, plVector4(-1,-1,-1,-1) );
    _totalRMSBufferID         = createSSBO( PL_STAGE2_INVOCATIONS, FLT_MAX );
}


void plDonorState::destroyBuffers()
{
    // delete buffer objects
    glDeleteBuffers( 1, &_donorPositionsBufferID   );
    glDeleteBuffers( 1, &_donorNormalsBufferID     );
    glDeleteBuffers( 1, &_donorZDirectionsBufferID );
    glDeleteBuffers( 1, &_totalRMSBufferID );
}


void plDonorState::bindBuffers()
{      
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, _donorPositionsBufferID   );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, _donorNormalsBufferID     );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, _donorZDirectionsBufferID );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 5, _totalRMSBufferID         );
}

void plDonorState::unbindBuffers()
{   
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, 0 );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, 0 );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, 0 );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 5, 0 );
}

void plDonorState::update()
{
    // get solution index with lowest rms
    PLint index = _getLowestRmsIndex();    

    if ( index != -1 )
    {                
        graftPositions.fill  ( PL_MAX_GRAFTS_PER_SOLUTION, plVector4( -1, -1, -1, -1) ); 
        graftNormals.fill    ( PL_MAX_GRAFTS_PER_SOLUTION, plVector4( -1, -1, -1, -1) ); 
        graftZDirections.fill( PL_MAX_GRAFTS_PER_SOLUTION, plVector4( -1, -1, -1, -1) ); 

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, _donorPositionsBufferID);            
        plVector4 *positions = readSSBO<plVector4>( index*PL_MAX_GRAFTS_PER_SOLUTION, PL_MAX_GRAFTS_PER_SOLUTION );
        memcpy( &graftPositions[0], &positions[0], PL_MAX_GRAFTS_PER_SOLUTION*sizeof( plVector4 ) );    
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, _donorNormalsBufferID);            
        plVector4 *normals = readSSBO<plVector4>( index*PL_MAX_GRAFTS_PER_SOLUTION, PL_MAX_GRAFTS_PER_SOLUTION );
        memcpy( &graftNormals[0], &normals[0], PL_MAX_GRAFTS_PER_SOLUTION*sizeof( plVector4 ) );    
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, _donorZDirectionsBufferID);            
        plVector4 *directions = readSSBO<plVector4>( index*PL_MAX_GRAFTS_PER_SOLUTION, PL_MAX_GRAFTS_PER_SOLUTION );
        memcpy( &graftZDirections[0], &directions[0], PL_MAX_GRAFTS_PER_SOLUTION*sizeof( plVector4 ) );    
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    }
    else
    {
        std::cerr << "plDonorState::update() error: Unable to find suitable harvest locations \n";
    }
 
}


PLint plDonorState::_getLowestRmsIndex()
{
    // copy total rms into client side array
    plSeq<PLfloat> totalRmsData( PL_STAGE2_INVOCATIONS, -1.0f ); 
                  
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _totalRMSBufferID);            
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
    
    std::cout << "\tLowest Total RMS: " << minRMS << "\n"; 
    
    return minIndex;
}


namespace plPlannerStage2
{

    plDonorState run( const plSeq<plSiteGrid> &donorSites, const plDefectState &defectState, const plRmsData &rmsInput )
    {
        //const PLuint DONOR_OUTPUT_BUFFER_SIZE = PL_STAGE2_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION*2;

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
        PLuint donorSiteDataBufferID    = getGroupGridSSBO( donorSites ); //donorSites[0].getGridSSBO();         
        PLuint rmsInputBufferID         = createSSBO( rmsInput.sets[0].rms );
        //PLuint donorPositionsBufferID   = createSSBO( DONOR_OUTPUT_BUFFER_SIZE, plVector4(-1,-1,-1,-1) );
        //PLuint donorNormalsBufferID     = createSSBO( DONOR_OUTPUT_BUFFER_SIZE, plVector4(-1,-1,-1,-1) );
        //PLuint donorZDirectionsBufferID = createSSBO( DONOR_OUTPUT_BUFFER_SIZE, plVector4(-1,-1,-1,-1) );
        //PLuint totalRmsBufferID         = createSSBO( PL_STAGE2_INVOCATIONS, FLT_MAX );
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, donorSiteDataBufferID  );    
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, rmsInputBufferID       );  
        
/////////////

        // generate and bind donor state output buffers    
        plDonorState donorState; 
        donorState.createBuffers();  // create output buffers
        donorState.bindBuffers();    // bind output buffers   
////////////////   
   
        /*
        // bind buffers
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, donorSiteDataBufferID  );    
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, rmsInputBufferID       );  
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, donorPositionsBufferID    );  
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, donorNormalsBufferID    );
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, donorZDirectionsBufferID    );
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 5, totalRmsBufferID );
        */
        
        // set state uniforms
        stage2Shader.setGraftUniforms( defectState.graftCount,
                                       defectState.graftRadii );

        stage2Shader.setSiteUniforms( donorSites.size(),
                                      totalGridPoints,
                                      donorGridSizes);
                                      
        // call compute shader with 1D workgrouping
        glDispatchCompute( PL_STAGE2_NUM_GROUPS, 1, 1 );
        
        // memory barrier      
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        // update donor state
        donorState.update();

        /*
        // copy total rms into client side array
        plSeq<PLfloat> totalRmsData( PL_STAGE2_INVOCATIONS, -1.0f ); 
                      
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, totalRmsBufferID);            
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
        plSeq<plVector4> donorData( defectState.graftCount*2, plVector4(-1,-1,-1,-1) ); 
            
        // check if solution exists
        if ( minIndex != -1 )
        {                
            std::cout << "\tTotal RMS: " << minRMS << "\n";     
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, donorOutputBufferID);            
            plVector4 *donors = readSSBO<plVector4>( minIndex*PL_MAX_GRAFTS_PER_SOLUTION*2, defectState.graftCount*2 );
            memcpy( &donorData[0], &donors[0], defectState.graftCount*2*sizeof( plVector4 ) );    
            glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        }
        else
        {
            std::cerr << "Error: Unable to find suitable harvest locations \n";
            return plSeq<plVector4>();
        }
        */
        
        // unbind and delete site and rms buffers
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 );           
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, 0 ); 
        glDeleteBuffers(1, &donorSiteDataBufferID);
        glDeleteBuffers(1, &rmsInputBufferID);
        
        // unbind and delete state output buffers
        donorState.unbindBuffers();  
        donorState.destroyBuffers();
        
        
        
        /*
        // unbind buffers
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 );           
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, 0 );    
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, 0 );
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, 0 ); 
        
        // delete buffers
        glDeleteBuffers( 1, &donorSiteDataBufferID  );
        glDeleteBuffers( 1, &rmsInputBufferID       );
        glDeleteBuffers( 1, &donorOutputBufferID    );
        glDeleteBuffers( 1, &totalRmsBufferID       );        
        */
        std::cout << "\tDonor locations and normals:\n";    
        for (PLuint i=0; i < defectState.graftCount; i++)
        {
            std::cout << "\t\tPosition "    << i << ": \t" << donorState.graftPositions[i]   << "\n";
            std::cout << "\t\tNormal "      << i << ": \t" << donorState.graftNormals[i]     << "\n";
            std::cout << "\t\tZ Direction " << i << ": \t" << donorState.graftZDirections[i] << "\n";
        }
        
        return donorState;
        
    }
    

}























