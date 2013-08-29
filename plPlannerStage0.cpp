#include "plPlannerStage0.h"

plAnnealingState::plAnnealingState( float initialEnergy )
    : temperature   ( PL_STAGE0_INITIAL_TEMPERATURE ),    
      energy        ( initialEnergy ),    
      graftCount    ( 0 ),    
      graftPositions( plVector4(-1,-1,-1,-1), PL_MAX_GRAFTS_PER_SOLUTION ),
      graftNormals  ( plVector4(-1,-1,-1,-1), PL_MAX_GRAFTS_PER_SOLUTION ),
      graftRadii    ( -1, PL_MAX_GRAFTS_PER_SOLUTION )
{        
}

void plAnnealingState::createBuffers()
{
    // create state buffers  
    _energiesBufferID       = createSSBO( PL_STAGE0_INVOCATIONS, -1.0f );
    _graftPositionsBufferID = createSSBO( PL_MAX_GRAFTS_PER_SOLUTION*PL_STAGE0_INVOCATIONS, plVector4(-1,-1,-1,-1) );
    _graftNormalsBufferID   = createSSBO( PL_MAX_GRAFTS_PER_SOLUTION*PL_STAGE0_INVOCATIONS, plVector4(-1,-1,-1,-1) );
    _graftRadiiBufferID     = createSSBO( PL_MAX_GRAFTS_PER_SOLUTION*PL_STAGE0_INVOCATIONS, -1.0f );
    _graftCountsBufferID    = createSSBO( PL_STAGE0_INVOCATIONS, -1 );
}


void plAnnealingState::destroyBuffers()
{
    // delete buffer objects
    glDeleteBuffers( 1, &_energiesBufferID       );
    glDeleteBuffers( 1, &_graftPositionsBufferID );
    glDeleteBuffers( 1, &_graftNormalsBufferID   );
    glDeleteBuffers( 1, &_graftRadiiBufferID     );
    glDeleteBuffers( 1, &_graftCountsBufferID    );
}


void plAnnealingState::bindBuffers()
{
       
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, _energiesBufferID       );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, _graftPositionsBufferID );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, _graftNormalsBufferID   );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 5, _graftRadiiBufferID     );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 6, _graftCountsBufferID    );
}

void plAnnealingState::unbindBuffers()
{
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 );           
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, 0 );    
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, 0 );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, 0 );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, 0 );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 5, 0 );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 6, 0 );
}


void plAnnealingState::update()
{
    PLint bestIndex = _updateEnergy();
    _updateGrafts( bestIndex );
}

float plAnnealingState::_acceptanceProbability( PLfloat energy, PLfloat newEnergy, PLfloat temperature ) 
{
    // If the new solution is better, accept it
    if (newEnergy < energy) 
    {
        return 1.0f;
    }
    // If the new solution is worse, calculate an acceptance probability
    return exp( -(newEnergy - energy) / temperature );
}

PLint plAnnealingState::_updateEnergy()
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _energiesBufferID);    
    
    PLfloat *energies = readSSBO<PLfloat>( 0, PL_STAGE0_INVOCATIONS );
    
    // randomize order each iteration to prevent bias    
    plSeq<PLuint> indexOrder(PL_STAGE0_INVOCATIONS); for (PLuint i=0; i< PL_STAGE0_INVOCATIONS; i++) indexOrder.add(i);
        
    plUtility::shuffle( indexOrder );
    
    // iterate through all state energies and find best one
    PLint bestIndex = -1;        
    for (PLuint i=0; i < PL_STAGE0_INVOCATIONS; i++ )
    {    
        PLuint j = indexOrder[i];          
        PLfloat r = ((float) rand() / (RAND_MAX));              
        if ( _acceptanceProbability( energy, energies[j], temperature ) > r )
        {    
            energy = energies[j];
            bestIndex = j;
        }
    }
    
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    return bestIndex;

}


void plAnnealingState::_updateGrafts( PLint index )
{
    if ( index != -1 )
    {
        // get graft count
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, _graftCountsBufferID);            
        PLuint *graftCounts = readSSBO<PLuint>( index, 1 );
        graftCount = graftCounts[0];
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        
        // get graft positions
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, _graftPositionsBufferID);    
        plVector4 *positions = readSSBO<plVector4>( index*PL_MAX_GRAFTS_PER_SOLUTION, PL_MAX_GRAFTS_PER_SOLUTION );                                                    
        memcpy( &graftPositions[0], &positions[0], graftCount*sizeof( plVector4 ) );
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        
        // get graft normals
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, _graftNormalsBufferID);    
        plVector4 *normals = readSSBO<plVector4>( index*PL_MAX_GRAFTS_PER_SOLUTION, PL_MAX_GRAFTS_PER_SOLUTION );
        memcpy( &graftNormals[0], &normals[0], graftCount*sizeof( plVector4 ) );    
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
               
        // get graft radii
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, _graftRadiiBufferID);            
        PLfloat *radii = readSSBO<PLfloat>( index*PL_MAX_GRAFTS_PER_SOLUTION, PL_MAX_GRAFTS_PER_SOLUTION );
        memcpy( &graftRadii[0], &radii[0], graftCount*sizeof( PLfloat ) );    
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    }
            
} 


namespace plPlannerStage0
{

    plAnnealingState run( const plSiteGrid &site )
    {
        // compile / link stage 0 shader
        plPlannerStage0Shader stage0Shader("./shaders/plannerStage0.comp");
        stage0Shader.bind();                            // bind shader  
        stage0Shader.setSiteUniforms( site.meshSize(),  // set defect site uniforms 
                                      site.area(), 
                                      site.size(), 
                                      site.perimSize() ); 
         
        // generate and bind site and temporary data                              
        PLuint _siteDataBufferID             = site.getFullSSBO();
        PLuint _overlappedTrianglesBufferID  = createSSBO( site.meshSize()*PL_STAGE0_INVOCATIONS, 0.0f ); // must initialize with 0s
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, _siteDataBufferID            );           
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, _overlappedTrianglesBufferID );  
                      
        // generate and bind annealing state data    
        plAnnealingState state( site.area() );  // empty state (no plugs) energy set to total site area
        state.createBuffers();                  // create input/output buffers
        state.bindBuffers();                    // bind input/output buffers   
            
        // simulated annealing                
        while ( state.temperature > 0.01f)
        {
            // set annealing uniforms
            stage0Shader.setAnnealingUniforms( state.temperature,
                                               state.energy,
                                               state.graftCount,
                                               state.graftPositions,
                                               state.graftNormals,
                                               state.graftRadii ); 

            // call compute shader with 1D workgrouping
            glDispatchCompute( PL_STAGE0_NUM_GROUPS, 1, 1 );
            
            // memory barrier      
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
            
            // update the annealing state 
            state.update(); 
            
            std::cout << "New Best energy: " << state.energy << ", " 
                      << state.graftCount << " grafts, Annealing temperature: " 
                      << state.temperature << "\n";

            // cool temperature
            state.temperature *= 1.0f-PL_STAGE0_COOLING_RATE;
        }

        state.unbindBuffers();  
        state.destroyBuffers(); // destroy buffers as they are not needed anymore
        
        // delete site and temporary data
        glDeleteBuffers(1, &_siteDataBufferID);
        glDeleteBuffers(1, &_overlappedTrianglesBufferID);
        
        return state;

    }

}























