#include "plAutomaticPlanner.h"

plSeq<plSiteGrid> plAutomaticPlanner::_donorSiteGrids;
plSeq<plSiteGrid> plAutomaticPlanner::_defectSiteGrids;

PLuint            plAutomaticPlanner::_gridPointsTextureID; 
PLuint            plAutomaticPlanner::_gridNormalsTextureID;                 
PLuint            plAutomaticPlanner::_siteMeshTextureID;
//PLuint            plAutomaticPlanner::_graftCapAreasTextureID;
PLuint            plAutomaticPlanner::_overlappingTriangleAreasTextureID; 
PLuint            plAutomaticPlanner::_stateEnergiesTextureID;    
PLuint            plAutomaticPlanner::_stateIndicesTextureID;
PLuint            plAutomaticPlanner::_statePerturbationsTextureID;
PLuint            plAutomaticPlanner::_stateGraftCountsTextureID;

// DEBUG
plSeq<plMesh>     plAutomaticPlanner::DEBUG_MESH;
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
        
        std::cout << "\t\t" <<  grid.triangles().size() << " triangles calculated \n";
        std::cout << "\t\t" <<  grid.size() << " grid points calculated \n";
    }
    

    for ( PLuint i = 0; i < plan.donorSites().size(); i++)
    {      
        std::cout << "\tCalculating donor site grid " << i << " \n";

        plSiteGrid grid( plan.donorSites(i).model()->cartilage.triangles(), plan.donorSites(i).boundary);
        _donorSiteGrids.add( grid );
        
        std::cout << "\t\t" <<  grid.triangles().size() << " triangles calculated \n";
        std::cout << "\t\t" <<  grid.size() << " grid points calculated \n";
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
    reportError("start");
    plSiteGrid &grid = _defectSiteGrids[0];

    PLuint meshSize = _defectSiteGrids[0].triangles().size();     
    PLuint gridSize = _defectSiteGrids[0].size();
    
    // buffer grid points 
    glGenTextures(1, &_gridPointsTextureID);
    glBindTexture(GL_TEXTURE_1D, _gridPointsTextureID);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, gridSize, 0, GL_RGBA, GL_FLOAT, &grid.points(0));

    reportError("gridPoints");

    // buffer grid normals
    glGenTextures(1, &_gridNormalsTextureID);
    glBindTexture(GL_TEXTURE_1D, _gridNormalsTextureID);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, gridSize, 0, GL_RGBA, GL_FLOAT, &grid.normals(0));

    reportError("gridNormals");

    // convert to texture format 
    plSeq<plVector4> triangles( plVector4(0,0,0,0), meshSize*4);
    
    for (PLuint i=0; i < meshSize; i++)
    {
        triangles[i]            = plVector4( grid.triangles()[i].point0(), 1);
        triangles[i+meshSize]   = plVector4( grid.triangles()[i].point1(), 1);
        triangles[i+meshSize*2] = plVector4( grid.triangles()[i].point2(), 1);
        triangles[i+meshSize*3] = plVector4( grid.triangles()[i].normal(), 1);
    }
    
    // buffer site triangles 
    glGenTextures(1, &_siteMeshTextureID);
    glBindTexture(GL_TEXTURE_2D, _siteMeshTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, meshSize, 4, 0, GL_RGBA, GL_FLOAT, &triangles[0]);

    reportError("siteMesh");
    
    /*
    // fill graft cap texture with -1's          
    plSeq<PLfloat> graftCaps(-1, gridSize*PL_MAX_GRAFT_CAP_TRIANGLES*4*4);   
        
    glGenTextures(1, &_potentialGraftCapsID);                              
    glBindTexture(GL_TEXTURE_2D, _potentialGraftCapsID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, PL_MAX_GRAFT_CAP_TRIANGLES*4, gridSize, 0, GL_RGBA, GL_FLOAT, &graftCaps[0] );

    reportError("graftCaps");
    */
    
    /*
    // fill graft area texture with -1's
    plSeq<PLfloat> graftAreas(-1, PL_MAX_GRAFTS_PER_SOLUTION*gridSize);   
       
    glGenTextures(1, &_graftCapAreasTextureID);                              
    glBindTexture(GL_TEXTURE_2D, _graftCapAreasTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, PL_MAX_GRAFTS_PER_SOLUTION, gridSize, 0, GL_RED, GL_FLOAT, &graftAreas[0]);

    reportError("graftAreas");
    */
    plSeq<PLfloat> overlappedTriangles(0, meshSize*gridSize);  

    glGenTextures(1, &_overlappingTriangleAreasTextureID);                              
    glBindTexture(GL_TEXTURE_2D, _overlappingTriangleAreasTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, meshSize, gridSize, 0, GL_RED, GL_FLOAT, &overlappedTriangles[0]);

    reportError("overlappedTriangles");

    /*
    // fill graft triangles texture with -1's
    plSeq<PLuint> graftTriangles(-1, PL_MAX_GRAFT_CAP_TRIANGLES*gridSize);  

    glGenTextures(1, &_potentialGraftTrianglesID);                              
    glBindTexture(GL_TEXTURE_2D, _potentialGraftTrianglesID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, PL_MAX_GRAFT_CAP_TRIANGLES, gridSize, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, &graftTriangles[0]);

    reportError("graftTriangles");
    */
    
    // fill state energies with -1's
    plSeq<PLfloat> stateEnergies(-1, gridSize);  

    // fill state energies texture with -1's   
    glGenTextures(1, &_stateEnergiesTextureID);                              
    glBindTexture(GL_TEXTURE_1D, _stateEnergiesTextureID);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, gridSize, 0, GL_RED, GL_FLOAT, &stateEnergies[0]);

    reportError("stateEnergies");

    // fill state indices with -1's
    plSeq<PLuint> stateIndices(-1, PL_MAX_GRAFTS_PER_SOLUTION*gridSize);  

    glGenTextures(1, &_stateIndicesTextureID);                              
    glBindTexture(GL_TEXTURE_2D, _stateIndicesTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, PL_MAX_GRAFTS_PER_SOLUTION, gridSize, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, &stateIndices[0]);

    reportError("stateIndices");
   
    // fill state cap perturbations
    plSeq<PLfloat> statePerturbations(-1, PL_MAX_GRAFTS_PER_SOLUTION*gridSize*16);  

    glGenTextures(1, &_statePerturbationsTextureID);                              
    glBindTexture(GL_TEXTURE_2D, _statePerturbationsTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, PL_MAX_GRAFTS_PER_SOLUTION*4, gridSize, 0, GL_RGBA, GL_FLOAT, &statePerturbations[0]);

    reportError("statePerturbations");

    // fill state graft counts with -1's
    plSeq<PLuint> stateGraftCounts(-1, gridSize);  

    glGenTextures(1, &_stateGraftCountsTextureID);                              
    glBindTexture(GL_TEXTURE_1D, _stateGraftCountsTextureID);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_R32UI, gridSize, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, &stateGraftCounts[0]);

    reportError("stateGraftCounts");

    glBindTexture(GL_TEXTURE_1D, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
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


void plAutomaticPlanner::_dispatchStage0()
{
    PLuint gridSize   = _defectSiteGrids[0].size();
    PLuint meshSize   = _defectSiteGrids[0].triangles().size();    
    PLuint workgroups = ceil( gridSize / (PLfloat) 1024); // ensure enough workgroups are used

    // compile / link stage 0 shader
    plPlannerStage0Shader stage0("./shaders/plannerStage0.comp");

    // bind shader
    stage0.bind(); 

    reportError("stage0 bind");

    // bind input/output buffers            
    glBindImageTexture(0, _gridPointsTextureID,       0, GL_FALSE, 0, GL_READ_ONLY,  GL_RGBA32F);
    glBindImageTexture(1, _gridNormalsTextureID,      0, GL_FALSE, 0, GL_READ_ONLY,  GL_RGBA32F);
    glBindImageTexture(2, _siteMeshTextureID,         0, GL_FALSE, 0, GL_READ_ONLY,  GL_RGBA32F);
    
    //glBindImageTexture(3, _graftCapAreasTextureID,            0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
    glBindImageTexture(3, _overlappingTriangleAreasTextureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
    
    glBindImageTexture(4, _stateEnergiesTextureID,      0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F    );
    glBindImageTexture(5, _stateIndicesTextureID,       0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI   );
    glBindImageTexture(6, _statePerturbationsTextureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F );
    glBindImageTexture(7, _stateGraftCountsTextureID,   0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI   );
       
    reportError("stage0 image tex");    
               
    PLfloat           stateTemperature        (PL_ANNEALING_INITIAL_TEMPERATURE);
    
    PLfloat           stateEnergy        (_defectSiteGrids[0].area());       
    PLuint            stateGraftCount    (0);
    plSeq<PLuint>     stateIndices       (0, PL_MAX_GRAFTS_PER_SOLUTION);
    plSeq<plMatrix44> statePerturbations (plMatrix44(-1.0), PL_MAX_GRAFTS_PER_SOLUTION);      

	plSeq<PLfloat> overlappingTriangles(0, meshSize*gridSize);  
  
    reportError("stage0 before annealing");
    
    while (stateTemperature > 0.1f)
    {
        // set uniforms
        stage0.setAnnealingUniforms( _defectSiteGrids[0].triangles().size(), 
                                     _defectSiteGrids[0].area(), 
                                     stateTemperature,
                                     stateEnergy,
                                     stateGraftCount,
                                     stateIndices,
                                     statePerturbations );

        reportError("stage0 uniforms");
      
        // call compute shader with 1D workgrouping
        glDispatchCompute( workgroups, 1, 1 );
        
        // memory barrier
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
          
        reportError("stage0 mem barr");   
                 
        plSeq<PLfloat> energies( -1.0, gridSize );

        // read energies into array            
        glBindTexture(GL_TEXTURE_1D, _stateEnergiesTextureID);    
        glGetTexImage(GL_TEXTURE_1D, 0, GL_RED, GL_FLOAT, &energies[0]);    
        
        reportError("stage0 get energies");   
        
        // iterate through all state energies and find best one
        PLint bestIndex = -1;
        PLfloat r = ((float) rand() / (RAND_MAX));
        for (PLuint i=0; i < gridSize; i++ )
        {
            if ( acceptanceProbability( stateEnergy, energies[i], stateTemperature ) > r )
            {              
                stateEnergy = energies[i];
                bestIndex = i;
            }
           
        }
        
        if ( bestIndex != -1 )
        {
            //std::cout << "\nBest index: " << bestIndex << "\n";
        
            plSeq<PLuint>     graftCount    (-1, gridSize); 
            plSeq<PLuint>     indices       (-1, PL_MAX_GRAFTS_PER_SOLUTION*gridSize); 
            plSeq<plMatrix44> perturbations (plMatrix44(-1.0), PL_MAX_GRAFTS_PER_SOLUTION*gridSize); 
            
            // state indices
            glBindTexture(GL_TEXTURE_2D, _stateIndicesTextureID);    
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, &indices[0]); 
            reportError("stage0 get indices");
            
            // state count
            glBindTexture(GL_TEXTURE_1D, _stateGraftCountsTextureID);    
            glGetTexImage(GL_TEXTURE_1D, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, &graftCount[0]); 
            reportError("stage0 get count");
            
            // grid perturbations            
            glBindTexture(GL_TEXTURE_2D, _statePerturbationsTextureID);    
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, &perturbations[0][0]); 
            reportError("stage0 get pert"); 
                                 
            PLuint offset = bestIndex*PL_MAX_GRAFTS_PER_SOLUTION;
            stateGraftCount = graftCount[bestIndex];

            std::cout << "graft count: " << stateGraftCount << "\n";
 
            for (PLuint i=0; i < stateGraftCount; i++)
            {
                stateIndices[i]       = indices[offset+i];
                //statePerturbations[i] = perturbations[offset+i]; 
                
                std::cout << "Index: " << stateIndices[i] << "\n"; // << statePerturbations[i] << "\n";;
            }
            
            // get perturbations
            std::cout << "Annealing temperature: " << stateTemperature << "\n";    
            std::cout << "\nNew Best energy: " << stateEnergy << ", " << stateGraftCount << " grafts\n";
            
        }
        
        // reset covered areas back to 0
		glBindTexture(GL_TEXTURE_2D, _overlappingTriangleAreasTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, meshSize, gridSize, 0, GL_RED, GL_FLOAT, &overlappingTriangles[0]);
		
        stateTemperature *= 1.0f-PL_ANNEALING_COOLING_RATE;

    }

    for (PLuint i=0; i < stateGraftCount; i++)
    {
        DEBUG_GRAFT_LOCATIONS.add( plVector3( _defectSiteGrids[0].points( stateIndices[i] ).x,
                                              _defectSiteGrids[0].points( stateIndices[i] ).y,
                                              _defectSiteGrids[0].points( stateIndices[i] ).z ) );
                                              
        DEBUG_GRAFT_LOCATIONS.add( plVector3( _defectSiteGrids[0].normals( stateIndices[i] ).x,
                                              _defectSiteGrids[0].normals( stateIndices[i] ).y,
                                              _defectSiteGrids[0].normals( stateIndices[i] ).z ) );
    }
    
    std::cout << "Size: " << DEBUG_GRAFT_LOCATIONS.size() << "\n";

}

