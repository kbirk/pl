#include "plPlannerStage0.h"

plAnnealingGroup::plAnnealingGroup( PLfloat initialEnergy )
    : temperature   ( PL_STAGE0_INITIAL_TEMPERATURE ),
      energies      ( PL_STAGE0_NUM_GROUPS, initialEnergy ),    
      graftCounts   ( PL_STAGE0_NUM_GROUPS, 0 ),    
      graftPositions( PL_STAGE0_NUM_GROUPS*PL_MAX_GRAFTS_PER_SOLUTION, plVector4(-1,-1,-1,-1) ),
      graftNormals  ( PL_STAGE0_NUM_GROUPS*PL_MAX_GRAFTS_PER_SOLUTION, plVector4(-1,-1,-1,-1) ),
      graftRadii    ( PL_STAGE0_NUM_GROUPS*PL_MAX_GRAFTS_PER_SOLUTION, -1.0 )
{        
}

void reportError( const plString &str  ) 
    {
        GLuint errnum;
        const char *errstr;
        while (errnum = glGetError()) 
        {
            errstr = reinterpret_cast<const char*> (gluErrorString(errnum));
            std::cout << str << " " << errstr << "\n";
        }
    }

void plAnnealingGroup::createBuffers()
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
    
    _groupStateBufferID = createSSBO( bufferSize, (GLvoid*)(NULL) ); //(const GLvoid*)(this) );

    // UBO
    /*
    _groupUniformBufferID  = createBO<( GL_UNIFORM_BUFFER, 
    
    glGenBuffers( 1, &_groupStates );
    glBindBuffer( GL_UNIFORM_BUFFER, _groupStates );

    PLuint bufferSize = ( sizeof(PLfloat)                                                            // temperature
                          PL_STAGE0_NUM_GROUPS * sizeof(PLfloat) +                                   // energies
                          PL_STAGE0_NUM_GROUPS * sizeof(PLuint) +                                    // counts
                          PL_STAGE0_NUM_GROUPS*PL_MAX_GRAFTS_PER_SOLUTION)* sizeof(plVector4)*2 +    // positions, normals
                          PL_STAGE0_NUM_GROUPS*PL_MAX_GRAFTS_PER_SOLUTION * sizeof(PLfloat);         // radii
                         
                                                
                         //////////////HERE!!
    
    glBufferData( GL_UNIFORM_BUFFER, bufferSize, this, GL_STREAM_COPY );
    glBindBuffer( GL_UNIFORM_BUFFER, 0 );
    */
}


void plAnnealingGroup::destroyBuffers()
{
    // delete buffer objects
    glDeleteBuffers( 1, &_energiesBufferID       );
    glDeleteBuffers( 1, &_graftPositionsBufferID );
    glDeleteBuffers( 1, &_graftNormalsBufferID   );
    glDeleteBuffers( 1, &_graftRadiiBufferID     );
    glDeleteBuffers( 1, &_graftCountsBufferID    );
    glDeleteBuffers( 1, &_groupStateBufferID     );
}


void plAnnealingGroup::bindBuffers()
{       
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, _energiesBufferID       );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, _graftPositionsBufferID );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, _graftNormalsBufferID   );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 5, _graftRadiiBufferID     );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 6, _graftCountsBufferID    );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 7, _groupStateBufferID     );
}

void plAnnealingGroup::unbindBuffers()
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
    /*
    PLuint bufferSize =  PL_STAGE0_NUM_GROUPS * sizeof(PLfloat) +                                   // energies
                         PL_STAGE0_NUM_GROUPS * sizeof(PLuint) +                                    // counts
                         PL_STAGE0_NUM_GROUPS*PL_MAX_GRAFTS_PER_SOLUTION * sizeof(plVector4)*2 +    // positions, normals
                         PL_STAGE0_NUM_GROUPS*PL_MAX_GRAFTS_PER_SOLUTION * sizeof(PLfloat);         // radii
                          
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, _groupStateBufferID );     
    glBufferData( GL_SHADER_STORAGE_BUFFER, bufferSize, (const GLvoid*)(&energies[0]), GL_STREAM_COPY );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );
    */
       
    // energies
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, _groupStateBufferID );            
    PLfloat *groupEnergies = (PLfloat*) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 
                                                          0, 
                                                          energies.size()*sizeof(PLfloat), 
                                                          GL_MAP_READ_BIT );                                                                                           
    memcpy( groupEnergies, &energies[0], energies.size()*sizeof( PLfloat ) );
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
    
    // graft counts           
    PLuint *groupCounts = (PLuint*) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 
                                                      energies.size()*sizeof(PLfloat), 
                                                      graftCounts.size()*sizeof(PLuint), 
                                                      GL_MAP_READ_BIT );                                                                                           
    memcpy( groupCounts, &graftCounts[0], graftCounts.size()*sizeof( PLuint ) );
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
    
    // graft positions          
    plVector4 *groupPositions = (plVector4*) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 
                                                               energies.size()*sizeof(PLfloat) + graftCounts.size()*sizeof(PLuint), 
                                                               graftPositions.size()*sizeof(plVector4), 
                                                               GL_MAP_READ_BIT );                                                                                           
    memcpy( groupPositions, &graftPositions[0], graftPositions.size()*sizeof( plVector4 ) );
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
    
    // graft normals          
    plVector4 *groupNormals = (plVector4*) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 
                                                             energies.size()*sizeof(PLfloat) + graftCounts.size()*sizeof(PLuint) + graftPositions.size()*sizeof(plVector4), 
                                                             graftNormals.size()*sizeof(plVector4), 
                                                             GL_MAP_READ_BIT );                                                                                           
    memcpy( groupNormals, &graftNormals[0], graftNormals.size()*sizeof( plVector4 ) );
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
    
    // graft radii           
    PLfloat *groupRadii = (PLfloat*) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 
                                                       energies.size()*sizeof(PLfloat) + graftCounts.size()*sizeof(PLuint) + graftPositions.size()*sizeof(plVector4) + graftNormals.size()*sizeof(plVector4), 
                                                       graftRadii.size()*sizeof(PLfloat), 
                                                       GL_MAP_READ_BIT );                                                                                           
    memcpy( groupRadii, &graftRadii[0], graftRadii.size()*sizeof( PLfloat ) );
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
    
    /*
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, _groupStateBufferID );    
    PLuint *groupEnergies = readSSBO<PLuint>( PL_STAGE0_NUM_GROUPS, PL_STAGE0_NUM_GROUPS );                                                 
    memcpy( &groupEnergies, &energies[0], PL_STAGE0_NUM_GROUPS*sizeof( PLuint ) );
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
    */
    
}


void plAnnealingGroup::convergeGroups()
{
    // for each workgroup, converge to local best
    for (PLuint i=0; i < PL_STAGE0_NUM_GROUPS; i++)
    {
        // find best energy
        PLint bestLocalIndex = _updateEnergy( i );
        
        //std::cout << "Group: " << i << ", best index: " << bestLocalIndex << ", best energy: " << energies[i] << std::endl;
        
        // update the graft data for workgroup
        _updateGrafts( i, bestLocalIndex );   
    }
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
    
    /*
    std::cout << "energies: ";
    for (PLuint i=0; i < PL_STAGE0_GROUP_SIZE; i++)
    {
         std::cout << newEnergies[i] << ", ";
    }
    std::cout << std::endl;
    */
    // randomize order each iteration to prevent bias    
    plSeq<PLuint> indexOrder( PL_STAGE0_GROUP_SIZE ); for (PLuint i=0; i<PL_STAGE0_GROUP_SIZE; i++) indexOrder.add(i);
    plUtility::shuffle( indexOrder );
    
    // iterate through all state energies and find best one
    PLint bestIndex = -1;        
    for (PLuint i=0; i < PL_STAGE0_GROUP_SIZE; i++ )
    {    
        // global index in energies array
        PLuint j = indexOrder[i];                                // local index
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


void plAnnealingGroup::_updateGrafts( PLuint groupIndex, PLint localIndex )
{
    if ( localIndex != -1 )
    {
        PLuint j =  (groupIndex * PL_STAGE0_GROUP_SIZE) + localIndex;
    
        plVector4 &graftPositionsStart = graftPositions[ groupIndex*PL_MAX_GRAFTS_PER_SOLUTION ];
        plVector4 &graftNormalsStart   = graftNormals  [ groupIndex*PL_MAX_GRAFTS_PER_SOLUTION ];
        PLfloat   &graftRadiiStart     = graftRadii    [ groupIndex*PL_MAX_GRAFTS_PER_SOLUTION ];

        // get graft count
        glBindBuffer( GL_SHADER_STORAGE_BUFFER, _graftCountsBufferID );            
        PLuint *counts = readSSBO<PLuint>( j, 1 );
        graftCounts[ groupIndex ] = counts[0];
        glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
        
        PLuint k =  (groupIndex * PL_STAGE0_GROUP_SIZE * PL_MAX_GRAFTS_PER_SOLUTION) + (localIndex * PL_MAX_GRAFTS_PER_SOLUTION);
        
        // get graft positions
        glBindBuffer( GL_SHADER_STORAGE_BUFFER, _graftPositionsBufferID );    
        plVector4 *positions = readSSBO<plVector4>( k, PL_MAX_GRAFTS_PER_SOLUTION );                                                 
        memcpy( &graftPositionsStart, &positions[0], graftCounts[ groupIndex ]*sizeof( plVector4 ) );
        glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

        // get graft normals
        glBindBuffer( GL_SHADER_STORAGE_BUFFER, _graftNormalsBufferID );    
        plVector4 *normals = readSSBO<plVector4>( k, PL_MAX_GRAFTS_PER_SOLUTION );
        memcpy( &graftNormalsStart, &normals[0], graftCounts[ groupIndex ]*sizeof( plVector4 ) );    
        glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
               
        // get graft radii
        glBindBuffer( GL_SHADER_STORAGE_BUFFER, _graftRadiiBufferID );            
        PLfloat *radii = readSSBO<PLfloat>( k, PL_MAX_GRAFTS_PER_SOLUTION );
        memcpy( &graftRadiiStart, &radii[0], graftCounts[ groupIndex ]*sizeof( PLfloat ) );    
        glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
        
        glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );
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
        
        // put in constructor
        states.createBuffers();       // create input/output buffers
        states.bindBuffers();         // bind input/output buffers   
        states.updateGroupBuffer(); 
        //
        
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

                //std::cout << "Itr: " << i << std::endl;

                stage0Shader.setLocalLoadUniform( i );
            }

            // cool temperature
            states.temperature -= PL_STAGE0_COOLING_RATE; //*= 1.0f - PL_STAGE0_COOLING_RATE;
            
            // update the annealing state 
            states.convergeGroups();               
            states.updateGroupBuffer();  
               
            PLint bestGroup = states.getCurrentBest();   
               
            std::cout << "\t Best Work Group: " << bestGroup <<", Energy: " << states.energies[ bestGroup ] << ",\t" 
                          << states.graftCounts[ bestGroup ] << " grafts,\t Temperature: " 
                          << states.temperature << std::endl;   
            
            /*    
            if (count > 10)
            {
                states.convergeGlobal();
                count = 0;
            }
            count++;
            */
        }

        states.getGlobalSolution( defectState );

        // unbind and delete state output buffers
        // put in destructor
        states.unbindBuffers();  
        states.destroyBuffers();
        //
        
        
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























