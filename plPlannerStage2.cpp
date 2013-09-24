#include "plPlannerStage2.h"

plDonorState::plDonorState()
    : graftPositions  ( PL_MAX_GRAFTS_PER_SOLUTION, plVector4(-1,-1,-1,-1) ),
      graftNormals    ( PL_MAX_GRAFTS_PER_SOLUTION, plVector4(-1,-1,-1,-1) ),
      graftZDirections( PL_MAX_GRAFTS_PER_SOLUTION, plVector4(-1,-1,-1,-1) ),
      totalRms        ( FLT_MAX )
{
    _createBuffers();
    _bindBuffers();
    
}


plDonorState::~plDonorState()
{
    _unbindBuffers();
    _destroyBuffers();
}


void plDonorState::_createBuffers()
{
    // create state buffers  
    _donorPositionsBufferID   = createSSBO( PL_STAGE2_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION, plVector4(-1,-1,-1,-1) );
    _donorNormalsBufferID     = createSSBO( PL_STAGE2_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION, plVector4(-1,-1,-1,-1) );
    _donorZDirectionsBufferID = createSSBO( PL_STAGE2_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION, plVector4(-1,-1,-1,-1) );
    _totalRMSBufferID         = createSSBO( PL_STAGE2_INVOCATIONS, -1.0f );
}


void plDonorState::_destroyBuffers()
{
    // delete buffer objects
    glDeleteBuffers( 1, &_donorPositionsBufferID   );
    glDeleteBuffers( 1, &_donorNormalsBufferID     );
    glDeleteBuffers( 1, &_donorZDirectionsBufferID );
    glDeleteBuffers( 1, &_totalRMSBufferID );
}


void plDonorState::_bindBuffers()
{      
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, _donorPositionsBufferID   );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, _donorNormalsBufferID     );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 5, _donorZDirectionsBufferID );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 6, _totalRMSBufferID         );
}

void plDonorState::_unbindBuffers()
{   
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, 0 );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, 0 );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 5, 0 );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 6, 0 );
}

void plDonorState::update()
{
    // get solution index with lowest rms
    PLint index = _getLowestRmsIndex();    

    if ( index != -1 )
    {              
        // new lowest rms  
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

}


PLint plDonorState::_getLowestRmsIndex()
{
    // copy total rms into client side array
    plSeq<PLfloat> totalRmsData( PL_STAGE2_INVOCATIONS, -1.0f ); 
                  
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _totalRMSBufferID);            
    PLfloat *tempRms = readSSBO<PLfloat>( 0, PL_STAGE2_INVOCATIONS );
    memcpy( &totalRmsData[0], &tempRms[0], PL_STAGE2_INVOCATIONS*sizeof( PLfloat ) );    
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

    if ( totalRms < minRMS )    // local state is worse than current state
        return -1;  
    
    totalRms = minRMS;    
    return minIndex;
}


namespace plPlannerStage2
{

    plDonorState run( const plSeq<plSiteGrid> &donorSites, const plDefectState &defectState, const plRmsData &rmsInput )
    {
        // compile / link stage 1 shader
        plPlannerStage2Shader stage2Shader("./shaders/plannerStage2.comp");
        stage2Shader.bind(); 
              
        // calc total grid points (invocations)
        PLuint totalGridPoints = 0;
        plSeq<PLuint> donorGridSizes;
        plSeq<PLuint> donorByteOffset;
        for (PLuint i=0; i < donorSites.size(); i++)
        {
            totalGridPoints += donorSites[i].gridSize();
            donorGridSizes.add ( donorSites[i].gridSize()  );
            if ( i == 0)
                donorByteOffset.add ( 0 );
            else
                donorByteOffset.add ( donorSites[i-1].gridSize()*2 );

        }

        // generate and fill buffers 
        PLuint donorSiteDataBufferID       = getGroupGridSSBO( donorSites );       
        PLuint rmsValuesInputBufferID      = rmsInput.getValuesSSBO();
        PLuint rmsDirectionsInputBufferID  = rmsInput.getDirectionsSSBO(); 
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, donorSiteDataBufferID      );    
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, rmsValuesInputBufferID     ); 
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, rmsDirectionsInputBufferID ); 
       
        // generate and bind donor state output buffers    
        plDonorState donorState; 

        // set state uniforms
        stage2Shader.setGraftUniforms( defectState.graftCount,
                                       rmsInput.sets.size(),    // number of individual directions
                                       defectState.graftRadii );

        stage2Shader.setSiteUniforms( donorSites.size(),
                                      totalGridPoints,
                                      donorGridSizes,
                                      donorByteOffset );
            
        for (PLuint i=0; i<PL_STAGE2_ITERATIONS; i++ )
        {
            // update seed uniform
            stage2Shader.setSeedUniform( i );
        
            // call compute shader with 1D workgrouping
            glDispatchCompute( PL_STAGE2_NUM_GROUPS, 1, 1 );
            
            // memory barrier      
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

            // update donor state
            donorState.update();
            
            std:: cout << "\tIteration " << i <<", Current lowest total RMS: " << donorState.totalRms << "\n";
        }    
         
        // no state found                              
        if ( donorState.totalRms == FLT_MAX )
        {
            donorState.graftPositions.clear();
            donorState.graftNormals.clear();
            donorState.graftZDirections.clear();
            std::cerr << "plPlannerStage2::run() error: Unable to find suitable harvest locations \n";
        }    
        
        // unbind and delete site and rms buffers
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 );           
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, 0 ); 
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, 0 ); 
        glDeleteBuffers( 1, &donorSiteDataBufferID      );
        glDeleteBuffers( 1, &rmsValuesInputBufferID     );
        glDeleteBuffers( 1, &rmsDirectionsInputBufferID );
        
        if (donorState.graftPositions.size() > 0 )
        {
            std::cout << "\tDonor locations and normals:\n";    
            for (PLuint i=0; i < defectState.graftCount; i++)
            {
                std::cout << "\t\tPosition "    << i << ": \t" << donorState.graftPositions[i]   << "\n";
                std::cout << "\t\tNormal "      << i << ": \t" << donorState.graftNormals[i]     << "\n";
                std::cout << "\t\tZ Direction " << i << ": \t" << donorState.graftZDirections[i] << "\n";
            }
        }
        return donorState;
        
    }
    

}























