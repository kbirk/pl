#include "plPlannerStage0.h"

plDefectState::plDefectState()
    : temperature   ( PL_STAGE0_INITIAL_TEMPERATURE ),    
      energy        ( 0 ),    
      graftCount    ( 0 ),    
      graftPositions( PL_MAX_GRAFTS_PER_SOLUTION, plVector4(-1,-1,-1,-1) ),
      graftNormals  ( PL_MAX_GRAFTS_PER_SOLUTION, plVector4(-1,-1,-1,-1) ),
      graftRadii    ( PL_MAX_GRAFTS_PER_SOLUTION, -1 )
{        
}

void plDefectState::createBuffers()
{
    // create state buffers  
    _energiesBufferID       = createSSBO( PL_STAGE0_INVOCATIONS, -1.0f );
    _graftPositionsBufferID = createSSBO( PL_MAX_GRAFTS_PER_SOLUTION*PL_STAGE0_INVOCATIONS, plVector4(-1,-1,-1,-1) );
    _graftNormalsBufferID   = createSSBO( PL_MAX_GRAFTS_PER_SOLUTION*PL_STAGE0_INVOCATIONS, plVector4(-1,-1,-1,-1) );
    _graftRadiiBufferID     = createSSBO( PL_MAX_GRAFTS_PER_SOLUTION*PL_STAGE0_INVOCATIONS, -1.0f );
    _graftCountsBufferID    = createSSBO( PL_STAGE0_INVOCATIONS, -1 );
}


void plDefectState::destroyBuffers()
{
    // delete buffer objects
    glDeleteBuffers( 1, &_energiesBufferID       );
    glDeleteBuffers( 1, &_graftPositionsBufferID );
    glDeleteBuffers( 1, &_graftNormalsBufferID   );
    glDeleteBuffers( 1, &_graftRadiiBufferID     );
    glDeleteBuffers( 1, &_graftCountsBufferID    );
}


void plDefectState::bindBuffers()
{
       
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, _energiesBufferID       );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, _graftPositionsBufferID );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, _graftNormalsBufferID   );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 5, _graftRadiiBufferID     );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 6, _graftCountsBufferID    );
}

void plDefectState::unbindBuffers()
{   
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, 0 );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, 0 );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, 0 );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 5, 0 );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 6, 0 );
}


void plDefectState::update()
{
    PLint bestIndex = _updateEnergy();
    _updateGrafts( bestIndex );
}

float plDefectState::_acceptanceProbability( PLfloat energy, PLfloat newEnergy, PLfloat temperature ) 
{
    // If the new solution is better, accept it
    if (newEnergy < energy) 
    {
        return 1.0f;
    }
    // If the new solution is worse, calculate an acceptance probability
    return exp( -(newEnergy - energy) / temperature );
}

PLint plDefectState::_updateEnergy()
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


void plDefectState::_updateGrafts( PLint index )
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
        
        glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );
    }
            
} 


namespace plPlannerStage0
{

    void run( plDefectState &state, const plSiteGrid &site )
    {
        // compile / link stage 0 shader
        plPlannerStage0Shader stage0Shader("./shaders/plannerStage0.comp");
        stage0Shader.bind();                            // bind shader  
        stage0Shader.setSiteUniforms( site.meshSize(),  // set defect site uniforms 
                                      site.area(), 
                                      site.gridSize(), 
                                      site.perimSize() ); 
         
        // generate and bind site and temporary buffers                              
        PLuint siteDataBufferID             = site.getFullSSBO();
        PLuint overlappedTrianglesBufferID  = createSSBO( site.meshSize()*PL_STAGE0_INVOCATIONS, 0.0f ); // must initialize with 0s
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, siteDataBufferID            );           
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, overlappedTrianglesBufferID );  
                      
        // generate and bind annealing state output buffers    
        state.energy = site.area();  // empty state (no plugs) energy set to total site area
        state.createBuffers();       // create input/output buffers
        state.bindBuffers();         // bind input/output buffers   
            
        // simulated annealing                
        while ( state.temperature > 0.01f )
        {
            stage0Shader.setLocalLoadUniform( 0 );
        
            // set annealing uniforms
            stage0Shader.setAnnealingUniforms( state.temperature,
                                               state.energy,
                                               state.graftCount,
                                               state.graftPositions,
                                               state.graftNormals,
                                               state.graftRadii ); 

            for ( PLuint i=0; i<PL_STAGE0_ITERATIONS; i++ )
            {                   
                // call compute shader with 1D workgrouping
                glDispatchCompute( PL_STAGE0_NUM_GROUPS, 1, 1 );
                
                // memory barrier      
                glMemoryBarrier( GL_ALL_BARRIER_BITS );

                stage0Shader.setLocalLoadUniform( 1 );
            }

            // cool temperature
            state.temperature *= 1.0f - PL_STAGE0_COOLING_RATE;
            
            // update the annealing state 
            state.update(); 
               
            std::cout << "\t Energy: " << state.energy << ",\t" 
                          << state.graftCount << " grafts,\t Temperature: " 
                          << state.temperature << std::endl;   
                
            
        }

        // unbind and delete state output buffers
        state.unbindBuffers();  
        state.destroyBuffers();
        
        // unbind and delete site and temporary buffers
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 );           
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, 0 ); 
        glDeleteBuffers(1, &siteDataBufferID);
        glDeleteBuffers(1, &overlappedTrianglesBufferID);
                        
        // re-compute positions as perturbations will shift them off the mesh surface!
        for (PLuint i=0; i < state.graftCount; i++)
        {
            plVector3 rayOrigin   ( state.graftPositions[i].x, state.graftPositions[i].y, state.graftPositions[i].z );
            plVector3 rayDirection( state.graftNormals[i].x,   state.graftNormals[i].y,   state.graftNormals[i].z   );
        
            plIntersection intersection = plMath::rayIntersect( site.triangles(), rayOrigin, rayDirection );
            state.graftPositions[i] = plVector4( intersection.point, 1.0 );
        }

    }

}























