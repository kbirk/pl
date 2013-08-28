#include "plAutomaticPlanner.h"

plSeq<plSiteGrid> plAutomaticPlanner::_donorSiteGrids;
plSeq<plSiteGrid> plAutomaticPlanner::_defectSiteGrids;

PLuint            plAutomaticPlanner::_siteDataTextureID;

PLuint            plAutomaticPlanner::_overlappedTrianglesBufferID; 
PLuint            plAutomaticPlanner::_stateEnergiesTextureID;    
PLuint            plAutomaticPlanner::_stateGraftPositionsTextureID;
PLuint            plAutomaticPlanner::_stateGraftNormalsTextureID;
PLuint            plAutomaticPlanner::_stateGraftRadiiTextureID;
PLuint            plAutomaticPlanner::_stateGraftCountsTextureID;

// DEBUG
plSeq<plVector3>  plAutomaticPlanner::DEBUG_GRAFT_LOCATIONS;

void plAutomaticPlanner::calculate( plPlan &plan )
{
    // generate site grids
    _generateSiteGrids( plan );

    _createStage0Buffers();
    _dispatch();

} 

void plAutomaticPlanner::_generateSiteGrids( plPlan &plan )
{
    _donorSiteGrids.clear();
    _defectSiteGrids.clear();

    std::cout << "Calculating Plan ... \n";
    
    for ( PLuint i = 0; i < plan.defectSites().size(); i++)
    {   
        std::cout << "\tCalculating defect site grid " << i << " \n";
        plSiteGrid grid( plan.defectSites(i).spline.triangles(), plan.defectSites(i).boundary );
        _defectSiteGrids.add( grid );         
        std::cout << "\t\t" <<  grid.meshSize() << " triangles calculated \n";
        std::cout << "\t\t" <<  grid.size()     << " grid points calculated \n";
    }
    
    for ( PLuint i = 0; i < plan.donorSites().size(); i++)
    {      
        std::cout << "\tCalculating donor site grid " << i << " \n";
        plSiteGrid grid( plan.donorSites(i).model()->cartilage.triangles(), plan.donorSites(i).boundary);
        _donorSiteGrids.add( grid );        
        std::cout << "\t\t" <<  grid.meshSize() << " triangles calculated \n";
        std::cout << "\t\t" <<  grid.size()     << " grid points calculated \n";
    }    
}


void plAutomaticPlanner::_dispatch()
{    
    PLtime t0 = plTimer::now();

    _dispatchStage0();
    
    PLtime t1 = plTimer::now();
    std::cout << "\nAutomatic planner stage 0 complete:\n";     
    std::cout << "\tCompute shader execution time: " << t1 - t0 << " milliseconds \n";
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

template< class T >
PLuint createSSBO( PLuint count, const T &fill )
{
    plSeq<T> filler( fill, count );

    PLuint bufferID;

    glGenBuffers(1, &bufferID);     
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, count*sizeof(T), &filler[0], GL_STREAM_COPY);
    
    return bufferID;
} 

void plAutomaticPlanner::_createStage0Buffers()
{
    PLuint meshSize = _defectSiteGrids[0].meshSize();     

    // site input buffers
    _siteDataTextureID = _defectSiteGrids[0].getFullSSBO();

    // temporary SSBO
    _overlappedTrianglesBufferID  = createSSBO( meshSize*PL_ANNEALING_INVOCATIONS, 0.0f );

    // state ouput buffers output  
    _stateEnergiesTextureID       = createSSBO( PL_ANNEALING_INVOCATIONS, -1.0f );
    _stateGraftPositionsTextureID = createSSBO( PL_MAX_GRAFTS_PER_SOLUTION*PL_ANNEALING_INVOCATIONS, plVector4(-1,-1,-1,-1) );
    _stateGraftNormalsTextureID   = createSSBO( PL_MAX_GRAFTS_PER_SOLUTION*PL_ANNEALING_INVOCATIONS, plVector4(-1,-1,-1,-1) );
    _stateGraftRadiiTextureID     = createSSBO( PL_MAX_GRAFTS_PER_SOLUTION*PL_ANNEALING_INVOCATIONS, -1.0f );
    _stateGraftCountsTextureID    = createSSBO( PL_ANNEALING_INVOCATIONS, -1 );
}


float acceptanceProbability( PLfloat energy, PLfloat newEnergy, PLfloat temperature ) 
{
    // If the new solution is better, accept it
    if (newEnergy < energy) 
    {
        return 1.0f;
    }
    // If the new solution is worse, calculate an acceptance probability
    return exp( -(newEnergy - energy) / temperature );
}

void shuffle( plSeq<PLuint> &array )
{   
    PLuint size = array.size();
    for (PLuint i = 0; i < size-1; i++) 
    {
      PLuint j = i + rand() / (RAND_MAX / (size - i) + 1);
      PLuint t = array[j];
      array[j] = array[i];
      array[i] = t;
    }    
}


void plAutomaticPlanner::_dispatchStage0()
{
    DEBUG_GRAFT_LOCATIONS.clear();
  
    PLuint workgroups = PL_ANNEALING_NUM_GROUPS; //ceil( gridSize / (PLfloat) 1024); // ensure enough workgroups are used
    
    // compile / link stage 0 shader
    plPlannerStage0Shader stage0("./shaders/plannerStage0.comp");

    // bind shader
    stage0.bind();    
      
    // state variables           
    PLfloat           stateTemperature   ( PL_ANNEALING_INITIAL_TEMPERATURE );    
    PLfloat           stateEnergy        ( _defectSiteGrids[0].area() );  // no plug energy will be uncovered area     
    PLuint            stateGraftCount    ( 0 );    
    plSeq<plVector4>  stateGraftPositions( plVector4(-1,-1,-1,-1), PL_MAX_GRAFTS_PER_SOLUTION );
    plSeq<plVector4>  stateGraftNormals  ( plVector4(-1,-1,-1,-1), PL_MAX_GRAFTS_PER_SOLUTION );
    plSeq<PLfloat>    stateGraftRadii    ( -1, PL_MAX_GRAFTS_PER_SOLUTION );

    // order indices, used to randomize order each iteration to prevent bias
    plSeq<PLuint> indexOrder(PL_ANNEALING_INVOCATIONS); for (PLuint i=0; i< PL_ANNEALING_INVOCATIONS; i++) indexOrder.add(i);
    
    // set defect site uniforms
    stage0.setSiteUniforms( _defectSiteGrids[0].meshSize(), 
                            _defectSiteGrids[0].area(), 
                            _defectSiteGrids[0].size(),
                            _defectSiteGrids[0].perimSize() );
      
    // bind input/output buffers  
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _siteDataTextureID);           
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _overlappedTrianglesBufferID);    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, _stateEnergiesTextureID );
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, _stateGraftPositionsTextureID );
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, _stateGraftNormalsTextureID );
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, _stateGraftRadiiTextureID );
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, _stateGraftCountsTextureID );   
         
    while ( stateTemperature > 0.01f)
    {
        // set annealing uniforms
        stage0.setAnnealingUniforms( stateTemperature,
                                     stateEnergy,
                                     stateGraftCount,
                                     stateGraftPositions,
                                     stateGraftNormals,
                                     stateGraftRadii ); 

        // call compute shader with 1D workgrouping
        glDispatchCompute( workgroups, 1, 1 );
        
        // memory barrier      
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
         
        // get state energies 
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, _stateEnergiesTextureID);    
        PLfloat *energies = (PLfloat*) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 
                                                         0, 
                                                         PL_ANNEALING_INVOCATIONS*sizeof(PLfloat),
                                                         GL_MAP_READ_BIT );    

        // iterate through all state energies and find best one
        shuffle( indexOrder );
        
        PLint bestIndex = -1;        
        for (PLuint i=0; i < PL_ANNEALING_INVOCATIONS; i++ )
        {    
            PLuint j = indexOrder[i];          
            PLfloat r = ((float) rand() / (RAND_MAX));              
            if ( acceptanceProbability( stateEnergy, energies[j], stateTemperature ) > r )
            {    
                stateEnergy = energies[j];
                bestIndex = j;
            }
        }
        
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        
        if ( bestIndex != -1 )
        {

            // get graft count
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, _stateGraftCountsTextureID);    
            
            PLuint *graftCount = (PLuint*) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 
                                                             bestIndex*sizeof(PLuint), 
                                                             sizeof(PLuint),
                                                             GL_MAP_READ_BIT );    
            
            stateGraftCount = graftCount[0];
            glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
            
            // get graft positions
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, _stateGraftPositionsTextureID);    
            
            plVector4 *positions = (plVector4*) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 
                                                                  (bestIndex*PL_MAX_GRAFTS_PER_SOLUTION)*sizeof(plVector4), 
                                                                  PL_MAX_GRAFTS_PER_SOLUTION*sizeof(plVector4),
                                                                  GL_MAP_READ_BIT );    
                                                                  
            memcpy( &stateGraftPositions[0], &positions[0], stateGraftCount*sizeof( plVector4 ) );
            /*                                                       
            for (PLuint i=0; i < stateGraftCount; i++)
            {                 
                stateGraftPositions[i] = positions[i];
            }
            */
            glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
            
            // get graft normals
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, _stateGraftNormalsTextureID);    
            
            plVector4 *normals = (plVector4*) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 
                                                                (bestIndex*PL_MAX_GRAFTS_PER_SOLUTION)*sizeof(plVector4), 
                                                                PL_MAX_GRAFTS_PER_SOLUTION*sizeof(plVector4),
                                                                GL_MAP_READ_BIT );   
            memcpy( &stateGraftNormals[0], &normals[0], stateGraftCount*sizeof( plVector4 ) );    
            /*                                                  
            for (PLuint i=0; i < stateGraftCount; i++)
            {                 
                stateGraftNormals[i] = normals[i];
            }
            */
            glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
            
            
            // get graft radii
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, _stateGraftRadiiTextureID);    
            
            PLfloat *radii = (PLfloat*) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 
                                                              (bestIndex*PL_MAX_GRAFTS_PER_SOLUTION)*sizeof(PLfloat), 
                                                              PL_MAX_GRAFTS_PER_SOLUTION*sizeof(PLfloat),
                                                              GL_MAP_READ_BIT );    
            memcpy( &stateGraftRadii[0], &radii[0], stateGraftCount*sizeof( PLfloat ) );    
            /*
            for (PLuint i=0; i < stateGraftCount; i++)
            {                 
                stateGraftRadii[i] = radii[i];
            }
            */
            glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
            
             
            std::cout << "New Best energy: " << stateEnergy << ", " << stateGraftCount << " grafts, Annealing temperature: " << stateTemperature << "\n";" \n";
            
        }

        //glBindBuffer(GL_SHADER_STORAGE_BUFFER, _overlappedTrianglesBufferID);
        //glBufferData(GL_SHADER_STORAGE_BUFFER, (meshSize*PL_ANNEALING_INVOCATIONS)*sizeof(PLfloat), &overlappingTriangles[0],  GL_STREAM_COPY);
    
        stateTemperature *= 1.0f-PL_ANNEALING_COOLING_RATE;
    }

    for (PLuint i=0; i < stateGraftCount; i++)
    {
        plVector4 p = stateGraftPositions[i];
        plVector4 n = stateGraftNormals[i];
        PLfloat   r = stateGraftRadii[i];
        
        DEBUG_GRAFT_LOCATIONS.add( plVector3( p.x, p.y, p.z ) );                                              
        DEBUG_GRAFT_LOCATIONS.add( plVector3( n.x, n.y, n.z ) );
        DEBUG_GRAFT_LOCATIONS.add( plVector3( r, 0.0f, 0.0f ) ); 
    }
    
    // delete buffer objects
    glDeleteBuffers(1, &_siteDataTextureID);
    glDeleteBuffers(1, &_overlappedTrianglesBufferID);
    glDeleteBuffers(1, &_stateEnergiesTextureID);
    glDeleteBuffers(1, &_stateGraftPositionsTextureID);
    glDeleteBuffers(1, &_stateGraftNormalsTextureID);
    glDeleteBuffers(1, &_stateGraftRadiiTextureID);
    glDeleteBuffers(1, &_stateGraftCountsTextureID);

}

/*
void plAutomaticPlanner::_dispatchStage0()
{
    DEBUG_GRAFT_LOCATIONS.clear();

    PLuint gridSize   = _defectSiteGrids[0].size();
    PLuint meshSize   = _defectSiteGrids[0].meshSize();    
    PLuint workgroups = PL_ANNEALING_NUM_GROUPS; //ceil( gridSize / (PLfloat) 1024); // ensure enough workgroups are used
    
    // compile / link stage 0 shader
    plPlannerStage0Shader stage0("./shaders/plannerStage0.comp");

    // bind shader
    stage0.bind(); 
    reportError("stage0 bind");
}
*/

