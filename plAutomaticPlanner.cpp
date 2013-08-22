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

    _bufferTextures();
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


void plAutomaticPlanner::_bufferTextures()
{
    PLuint meshSize = _defectSiteGrids[0].meshSize();     

    // site data SSBO
    _siteDataTextureID = _defectSiteGrids[0].getSSBO();

    // temporary SSBO
    plSeq<PLfloat> overlappingTriangles(0, meshSize*PL_ANNEALING_THREADS);    
    glGenBuffers(1, &_overlappedTrianglesBufferID);     
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _overlappedTrianglesBufferID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, (meshSize*PL_ANNEALING_THREADS)*sizeof(PLfloat), &overlappingTriangles[0], GL_STREAM_COPY);
    

    // state energy output    
    plSeq<PLfloat> stateEnergies(-1, PL_ANNEALING_THREADS);     // fill with -1's
  
    glGenBuffers(1, &_stateEnergiesTextureID);     
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _stateEnergiesTextureID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, PL_ANNEALING_THREADS*sizeof(PLfloat), &stateEnergies[0], GL_STREAM_COPY);
    
    /*
    glGenTextures(1, &_stateEnergiesTextureID);                              
    glBindTexture(GL_TEXTURE_1D, _stateEnergiesTextureID);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, PL_ANNEALING_THREADS, 0, GL_RED, GL_FLOAT, &stateEnergies[0]);
    reportError("stateEnergies");
    */
    // state graft data
    plSeq<PLfloat> statePoints(-1, PL_MAX_GRAFTS_PER_SOLUTION*PL_ANNEALING_THREADS*4); // fill with -1's    
    plSeq<PLfloat> stateRadii(-1, PL_MAX_GRAFTS_PER_SOLUTION*PL_ANNEALING_THREADS);   // fill with -1's    

    glGenBuffers(1, &_stateGraftPositionsTextureID);     
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _stateGraftPositionsTextureID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, (PL_MAX_GRAFTS_PER_SOLUTION*PL_ANNEALING_THREADS)*sizeof(plVector4), &statePoints[0], GL_STREAM_COPY);
    
    /*
    glGenTextures(1, &_stateGraftPositionsTextureID);                              
    glBindTexture(GL_TEXTURE_2D, _stateGraftPositionsTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, PL_MAX_GRAFTS_PER_SOLUTION, PL_ANNEALING_THREADS, 0, GL_RGBA, GL_FLOAT, &statePoints[0]);
    */
    reportError("statePositions");
    
    glGenBuffers(1, &_stateGraftNormalsTextureID);     
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _stateGraftNormalsTextureID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, (PL_MAX_GRAFTS_PER_SOLUTION*PL_ANNEALING_THREADS)*sizeof(plVector4), &statePoints[0], GL_STREAM_COPY);
    
    /*
    glGenTextures(1, &_stateGraftNormalsTextureID);                              
    glBindTexture(GL_TEXTURE_2D, _stateGraftNormalsTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, PL_MAX_GRAFTS_PER_SOLUTION, PL_ANNEALING_THREADS, 0, GL_RGBA, GL_FLOAT, &statePoints[0]);
    */
    reportError("stateNormals");
    
    glGenBuffers(1, &_stateGraftRadiiTextureID);     
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _stateGraftRadiiTextureID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, (PL_MAX_GRAFTS_PER_SOLUTION*PL_ANNEALING_THREADS)*sizeof(PLfloat), &stateRadii[0], GL_STREAM_COPY);
   
    /*
    glGenTextures(1, &_stateGraftRadiiTextureID);                              
    glBindTexture(GL_TEXTURE_2D, _stateGraftRadiiTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, PL_MAX_GRAFTS_PER_SOLUTION, PL_ANNEALING_THREADS, 0, GL_RED, GL_FLOAT, &stateRadii[0]);
    */
    reportError("stateRadii");
    
    // fill graft counts
    plSeq<PLuint> stateGraftCounts(-1, PL_ANNEALING_THREADS); // fill with -1's

    glGenBuffers(1, &_stateGraftCountsTextureID);     
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _stateGraftCountsTextureID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, PL_ANNEALING_THREADS*sizeof(PLuint), &stateGraftCounts[0], GL_STREAM_COPY);
   
    /*
    glGenTextures(1, &_stateGraftCountsTextureID);                              
    glBindTexture(GL_TEXTURE_1D, _stateGraftCountsTextureID);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_R32UI, PL_ANNEALING_THREADS, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, &stateGraftCounts[0]);
    */
    reportError("stateGraftCounts");

    //glBindTexture(GL_TEXTURE_1D, 0);
    //glBindTexture(GL_TEXTURE_2D, 0);
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

    PLuint gridSize   = _defectSiteGrids[0].size();
    PLuint meshSize   = _defectSiteGrids[0].meshSize();    
    PLuint workgroups = 1; //ceil( gridSize / (PLfloat) 1024); // ensure enough workgroups are used
    
    // compile / link stage 0 shader
    plPlannerStage0Shader stage0("./shaders/plannerStage0.comp");

    // bind shader
    stage0.bind(); 
    reportError("stage0 bind");

    plSeq<PLfloat> overlappingTriangles(0, meshSize*PL_ANNEALING_THREADS); 

    // bind input/output buffers  
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _siteDataTextureID);           
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _overlappedTrianglesBufferID);    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, _stateEnergiesTextureID );
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, _stateGraftPositionsTextureID );
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, _stateGraftNormalsTextureID );
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, _stateGraftRadiiTextureID );
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, _stateGraftCountsTextureID );      
    reportError("stage0 image tex");    
               
    PLfloat           stateTemperature   ( PL_ANNEALING_INITIAL_TEMPERATURE );    
    PLfloat           stateEnergy        ( _defectSiteGrids[0].area() );       
    PLuint            stateGraftCount    ( 0 );
    
    plSeq<plVector4>  stateGraftPositions( plVector4(-1,-1,-1,-1), PL_MAX_GRAFTS_PER_SOLUTION );
    plSeq<plVector4>  stateGraftNormals  ( plVector4(-1,-1,-1,-1), PL_MAX_GRAFTS_PER_SOLUTION );
    plSeq<PLfloat>    stateGraftRadii    ( -1, PL_MAX_GRAFTS_PER_SOLUTION );

    plSeq<PLuint> indexOrder(PL_ANNEALING_THREADS); for (PLuint i=0; i< PL_ANNEALING_THREADS; i++) indexOrder.add(i);

           
    while (stateTemperature > 0.01f)
    {
        // set uniforms
        stage0.setAnnealingUniforms( _defectSiteGrids[0].meshSize(), 
                                     _defectSiteGrids[0].area(), 
                                     _defectSiteGrids[0].size(),
                                     _defectSiteGrids[0].perimSize(),
                                     stateTemperature,
                                     stateEnergy,
                                     stateGraftCount,
                                     stateGraftPositions,
                                     stateGraftNormals,
                                     stateGraftRadii );                                     
        reportError("stage0 uniforms");
        
        
        // call compute shader with 1D workgrouping
        glDispatchCompute( workgroups, 1, 1 );
        
        // memory barrier      
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
         
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, _stateEnergiesTextureID);    
        PLfloat *energies = (PLfloat*) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 
                                                         0, 
                                                         PL_ANNEALING_THREADS*sizeof(PLfloat),
                                                         GL_MAP_READ_BIT );    

        // iterate through all state energies and find best one
        shuffle( indexOrder );
        PLint bestIndex = -1;        
        for (PLuint i=0; i < PL_ANNEALING_THREADS; i++ )
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
            for (PLuint i=0; i < stateGraftCount; i++)
            {                 
                stateGraftPositions[i] = positions[i];
            }
            glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
            
            // get graft normals
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, _stateGraftNormalsTextureID);    
            
            plVector4 *normals = (plVector4*) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 
                                                                (bestIndex*PL_MAX_GRAFTS_PER_SOLUTION)*sizeof(plVector4), 
                                                                PL_MAX_GRAFTS_PER_SOLUTION*sizeof(plVector4),
                                                                GL_MAP_READ_BIT );    
            for (PLuint i=0; i < stateGraftCount; i++)
            {                 
                stateGraftNormals[i] = normals[i];
            }
            glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
            
            
            // get graft radii
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, _stateGraftRadiiTextureID);    
            
            PLfloat *radii = (PLfloat*) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 
                                                              (bestIndex*PL_MAX_GRAFTS_PER_SOLUTION)*sizeof(PLfloat), 
                                                              PL_MAX_GRAFTS_PER_SOLUTION*sizeof(PLfloat),
                                                              GL_MAP_READ_BIT );    
            for (PLuint i=0; i < stateGraftCount; i++)
            {                 
                stateGraftRadii[i] = radii[i];
            }
            glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
            
             
            std::cout << "New Best energy: " << stateEnergy << ", " << stateGraftCount << " grafts, Annealing temperature: " << stateTemperature << "\n";" \n";
            
        }

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, _overlappedTrianglesBufferID);
        glBufferData(GL_SHADER_STORAGE_BUFFER, (meshSize*PL_ANNEALING_THREADS)*sizeof(PLfloat), &overlappingTriangles[0],  GL_STREAM_COPY);
    
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

}

