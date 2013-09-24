#include "plPlannerStage0.h"

plAnnealingGroup::plAnnealingGroup( PLfloat initialEnergy )
    : temperature   ( PL_STAGE0_INITIAL_TEMPERATURE ),
      energies      ( PL_STAGE0_NUM_GROUPS, initialEnergy ),    
      graftCounts   ( PL_STAGE0_NUM_GROUPS, 0 ),    
      graftPositions( PL_STAGE0_NUM_GROUPS*PL_MAX_GRAFTS_PER_SOLUTION, plVector4(-1,-1,-1,-1) ),
      graftNormals  ( PL_STAGE0_NUM_GROUPS*PL_MAX_GRAFTS_PER_SOLUTION, plVector4(-1,-1,-1,-1) ),
      graftRadii    ( PL_STAGE0_NUM_GROUPS*PL_MAX_GRAFTS_PER_SOLUTION, -1.0 )
{ 
    _createBuffers();      // create input/output buffers
    _bindBuffers();        // bind input/output buffers   
    updateGroupBuffer();   // upload group states to GPU    
}

plAnnealingGroup::~plAnnealingGroup()
{
    _unbindBuffers();
    _destroyBuffers();
}

void plAnnealingGroup::_createBuffers()
{
    // SSBOs 
    _energiesBufferID       = createSSBO( PL_STAGE0_INVOCATIONS, -1.0f );
    _graftPositionsBufferID = createSSBO( PL_MAX_GRAFTS_PER_SOLUTION*PL_STAGE0_INVOCATIONS, plVector4(-1,-1,-1,-1) );
    _graftNormalsBufferID   = createSSBO( PL_MAX_GRAFTS_PER_SOLUTION*PL_STAGE0_INVOCATIONS, plVector4(-1,-1,-1,-1) );
    _graftRadiiBufferID     = createSSBO( PL_MAX_GRAFTS_PER_SOLUTION*PL_STAGE0_INVOCATIONS, -1.0f );
    _graftCountsBufferID    = createSSBO( PL_STAGE0_INVOCATIONS, -1 );
    
        
    PLuint bufferSize =  PL_STAGE0_NUM_GROUPS * sizeof(PLfloat) +                                   // energies
                         PL_STAGE0_NUM_GROUPS * sizeof(PLuint) +                                    // counts
                         PL_STAGE0_NUM_GROUPS*PL_MAX_GRAFTS_PER_SOLUTION * sizeof(plVector4)*2 +    // positions, normals
                         PL_STAGE0_NUM_GROUPS*PL_MAX_GRAFTS_PER_SOLUTION * sizeof(PLfloat);         // radii
    
    _groupStateBufferID = createSSBO( bufferSize, (GLvoid*)(NULL) );

}


void plAnnealingGroup::_destroyBuffers()
{
    // delete buffer objects
    glDeleteBuffers( 1, &_energiesBufferID       );
    glDeleteBuffers( 1, &_graftPositionsBufferID );
    glDeleteBuffers( 1, &_graftNormalsBufferID   );
    glDeleteBuffers( 1, &_graftRadiiBufferID     );
    glDeleteBuffers( 1, &_graftCountsBufferID    );
    glDeleteBuffers( 1, &_groupStateBufferID     );
}


void plAnnealingGroup::_bindBuffers()
{       
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, _energiesBufferID       );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, _graftPositionsBufferID );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, _graftNormalsBufferID   );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 5, _graftRadiiBufferID     );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 6, _graftCountsBufferID    );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 7, _groupStateBufferID     );
}

void plAnnealingGroup::_unbindBuffers()
{   
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, 0 );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, 0 );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, 0 );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 5, 0 );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 6, 0 );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 7, 0 );
}


void plAnnealingGroup::updateGroupBuffer() const
{
    PLuint byteOffset = 0;  
       
    // energies        
    copyToSSBO( _groupStateBufferID, energies, byteOffset );
    byteOffset += energies.size()*sizeof(PLfloat);
    
    // graft counts              
    copyToSSBO( _groupStateBufferID, graftCounts, byteOffset );
    byteOffset += graftCounts.size()*sizeof(PLuint);
     
    // graft positions 
    copyToSSBO( _groupStateBufferID, graftPositions, byteOffset );
    byteOffset += graftPositions.size()*sizeof(plVector4);
         
    // graft normals 
    copyToSSBO( _groupStateBufferID, graftNormals, byteOffset );
    byteOffset += graftNormals.size()*sizeof(plVector4);
    
    // graft radii 
    copyToSSBO( _groupStateBufferID, graftRadii, byteOffset );
    
}


void plAnnealingGroup::convergeWorkGroups()
{
    // for each workgroup, converge to local best
    for (PLuint i=0; i < PL_STAGE0_NUM_GROUPS; i++)
    {
        // find best energy
        PLint bestLocalIndex = _updateEnergy( i );

        // update the graft data for workgroup
        _updateWorkGroups( i, bestLocalIndex );   
    }
    updateGroupBuffer();   // upload group states to GPU  
}


void plAnnealingGroup::convergeGlobal()
{
    std::cout << "CONVERGE!" << std::endl;

    PLint bestGroup = getCurrentBest();

    // for each workgroup, converge to local best
    for (PLuint i=0; i < PL_STAGE0_NUM_GROUPS; i++)
    {
        energies   [ i ] = energies   [ bestGroup ];
        graftCounts[ i ] = graftCounts[ bestGroup ];
        
        for (PLuint j=0; j < PL_MAX_GRAFTS_PER_SOLUTION; j++)
        {
            graftPositions[ i * PL_MAX_GRAFTS_PER_SOLUTION + j ] = graftPositions[ bestGroup * PL_MAX_GRAFTS_PER_SOLUTION + j ];
            graftNormals  [ i * PL_MAX_GRAFTS_PER_SOLUTION + j ] = graftNormals  [ bestGroup * PL_MAX_GRAFTS_PER_SOLUTION + j ];
            graftRadii    [ i * PL_MAX_GRAFTS_PER_SOLUTION + j ] = graftRadii    [ bestGroup * PL_MAX_GRAFTS_PER_SOLUTION + j ];
        }
    }
    updateGroupBuffer();
}


PLint plAnnealingGroup::getCurrentBest() const
{
    PLint index = -1;
    PLfloat energy = FLT_MAX;
    for (PLuint i=0; i < PL_STAGE0_NUM_GROUPS; i++ )
    {                  
        if ( energies[i] < energy )
        {    
            energy = energies[i];
            index = i;
        }
    }
    return index;
}


void plAnnealingGroup::getGlobalSolution( plDefectState &state )
{
    PLint bestGroup = getCurrentBest();

    state.graftCount     = graftCounts[ bestGroup ];  
    state.graftPositions = graftPositions.extract ( bestGroup*PL_MAX_GRAFTS_PER_SOLUTION, graftCounts[ bestGroup ] );
    state.graftNormals   = graftNormals.extract   ( bestGroup*PL_MAX_GRAFTS_PER_SOLUTION, graftCounts[ bestGroup ] );
    state.graftRadii     = graftRadii.extract     ( bestGroup*PL_MAX_GRAFTS_PER_SOLUTION, graftCounts[ bestGroup ] );   
}


float plAnnealingGroup::_acceptanceProbability( PLfloat energy, PLfloat newEnergy ) 
{
    // If the new solution is better, accept it
    if (newEnergy < energy) 
    {
        return 1.0f;
    }
    // If the new solution is worse, calculate an acceptance probability
    return exp( -(newEnergy - energy) / temperature );
}


PLint plAnnealingGroup::_updateEnergy( PLuint groupIndex )
{
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, _energiesBufferID );    
    
    PLfloat *newEnergies = readSSBO<PLfloat>( groupIndex*PL_STAGE0_GROUP_SIZE, PL_STAGE0_GROUP_SIZE ); //groupIndex*PL_STAGE0_GROUP_SIZE, PL_STAGE0_GROUP_SIZE );

    // randomize order each iteration to prevent bias    
    plSeq<PLuint> indexOrder( PL_STAGE0_GROUP_SIZE ); for (PLuint i=0; i<PL_STAGE0_GROUP_SIZE; i++) indexOrder.add(i);
    plUtility::shuffle( indexOrder );
    
    // iterate through all state energies and find best one
    PLint bestIndex = -1;        
    for (PLuint i=0; i < PL_STAGE0_GROUP_SIZE; i++ )
    {    
        // global index in energies array
        PLuint j = indexOrder[i];                   // local index
        PLfloat r = ((float) rand() / (RAND_MAX));
        if ( _acceptanceProbability( energies[groupIndex], newEnergies[j] ) > r )
        {    
            energies[groupIndex] = newEnergies[j];
            bestIndex = j;
        }
        
    }
    
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
    
    return bestIndex;
}


void plAnnealingGroup::_updateWorkGroups( PLuint groupIndex, PLint localIndex )
{
    if ( localIndex != -1 )
    {    
        // get reference to starting index of specific work group
        PLuint    &graftCountStart     = graftCounts   [ groupIndex ];
        plVector4 &graftPositionsStart = graftPositions[ groupIndex*PL_MAX_GRAFTS_PER_SOLUTION ];
        plVector4 &graftNormalsStart   = graftNormals  [ groupIndex*PL_MAX_GRAFTS_PER_SOLUTION ];
        PLfloat   &graftRadiiStart     = graftRadii    [ groupIndex*PL_MAX_GRAFTS_PER_SOLUTION ];

        PLuint indexOffset = ( (groupIndex * PL_STAGE0_GROUP_SIZE) + localIndex) ; 
          
        // get graft count        
        copyFromSSBO( &graftCountStart, _graftCountsBufferID, indexOffset * sizeof( PLuint ), sizeof(PLuint) );

        indexOffset =  (groupIndex * PL_STAGE0_GROUP_SIZE * PL_MAX_GRAFTS_PER_SOLUTION) + (localIndex * PL_MAX_GRAFTS_PER_SOLUTION);
        
        // get graft positions
        copyFromSSBO( &graftPositionsStart, _graftPositionsBufferID, indexOffset * sizeof( plVector4 ), graftCounts[ groupIndex ]*sizeof( plVector4 ) );        
        // get graft normals
        copyFromSSBO( &graftNormalsStart,   _graftNormalsBufferID,   indexOffset * sizeof( plVector4 ), graftCounts[ groupIndex ]*sizeof( plVector4 ) );       
        // get graft radii
        copyFromSSBO( &graftRadiiStart,     _graftRadiiBufferID,      indexOffset * sizeof( PLfloat ), graftCounts[ groupIndex ]*sizeof( PLfloat ) );
    }     
} 


namespace plPlannerStage0
{

    void run( plDefectState &defectState, const plSiteGrid &site )
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
        plAnnealingGroup states( site.area() ); // empty state (no plugs) energy set to total site area
        
        PLuint count = 0;
        
        // simulated annealing                
        while ( states.temperature > 0.01f )
        {
            stage0Shader.setLocalLoadUniform  ( 0 );         
            stage0Shader.setTemperatureUniform( states.temperature ); 

            for ( PLint i=PL_STAGE0_ITERATIONS; i>=0; i-- )
            {                   
                // call compute shader with 1D workgrouping
                glDispatchCompute( PL_STAGE0_NUM_GROUPS, 1, 1 );
                
                // memory barrier      
                glMemoryBarrier( GL_ALL_BARRIER_BITS );

                stage0Shader.setLocalLoadUniform( i );
            }

            // cool temperature
            states.temperature -= PL_STAGE0_COOLING_RATE;
            
            // update the annealing state 
            states.convergeWorkGroups();               
            //states.updateGroupBuffer();  
               
            PLint bestGroup = states.getCurrentBest();   
               
            std::cout << "\t Best Work Group: " << bestGroup <<", Energy: " << states.energies[ bestGroup ] << ",\t" 
                          << states.graftCounts[ bestGroup ] << " grafts,\t Temperature: " 
                          << states.temperature << std::endl;   
            
            /*    
            if (count > PL_GLOBAL_CONVERGE_START)
            {
                if ( count % PL_GLOBAL_CONVERGE_INC == 0 )
                {
                    states.convergeGlobal();
                }
                
            }
            */
            count++;
            
        }

        // load global solution from annealing state to defect state
        states.getGlobalSolution( defectState );
        
        // unbind and delete site and temporary buffers
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 );           
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, 0 ); 
        glDeleteBuffers(1, &siteDataBufferID);
        glDeleteBuffers(1, &overlappedTrianglesBufferID);
                        
        // re-compute positions as perturbations will shift them off the mesh surface!
        for (PLuint i=0; i < defectState.graftCount; i++)
        {
            plVector3 rayOrigin   ( defectState.graftPositions[i].x, defectState.graftPositions[i].y, defectState.graftPositions[i].z );
            plVector3 rayDirection( defectState.graftNormals[i].x,   defectState.graftNormals[i].y,   defectState.graftNormals[i].z   );
        
            plIntersection intersection = plMath::rayIntersect( site.triangles(), rayOrigin, rayDirection );
            defectState.graftPositions[i] = plVector4( intersection.point, 1.0 );
        }

    }

}























