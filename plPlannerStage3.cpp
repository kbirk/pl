#include "plPlannerStage3.h"

/*
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
    _donorPositionsBufferID   = createSSBO( PL_STAGE3_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION, plVector4(-1,-1,-1,-1) );
    _donorNormalsBufferID     = createSSBO( PL_STAGE3_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION, plVector4(-1,-1,-1,-1) );
    _donorZDirectionsBufferID = createSSBO( PL_STAGE3_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION, plVector4(-1,-1,-1,-1) );
    _totalRMSBufferID         = createSSBO( PL_STAGE3_INVOCATIONS, -1.0f );
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
        graftPositions.assign  ( PL_MAX_GRAFTS_PER_SOLUTION, plVector4( -1, -1, -1, -1) ); 
        graftNormals.assign    ( PL_MAX_GRAFTS_PER_SOLUTION, plVector4( -1, -1, -1, -1) ); 
        graftZDirections.assign( PL_MAX_GRAFTS_PER_SOLUTION, plVector4( -1, -1, -1, -1) ); 

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
    std::vector<PLfloat> totalRmsData( PL_STAGE3_INVOCATIONS, -1.0f ); 
                  
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _totalRMSBufferID);            
    PLfloat *tempRms = readSSBO<PLfloat>( 0, PL_STAGE3_INVOCATIONS );
    memcpy( &totalRmsData[0], &tempRms[0], PL_STAGE3_INVOCATIONS*sizeof( PLfloat ) );    
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    // find invocation with lowest RMS
    float minRMS   = FLT_MAX;
    PLint minIndex = -1;    

    for (PLuint i=0; i < PL_STAGE3_INVOCATIONS; i++)
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
*/

void plDonorSolution::extractBestSolution( PLuint graftCount, const plSSBO &totalRmsSSBO, const plSSBO &donorSolutionPositionsSSBO, const plSSBO &donorSolutionNormalsSSBO )
{
    std::vector<PLfloat> totalRMS( PL_STAGE_3_INVOCATIONS, 1.0f );

    totalRmsSSBO.read( totalRMS, totalRMS.size() );

    // find invocation with lowest RMS
    float minRMS   = FLT_MAX;
    PLint minIndex = -1;    

    for (PLuint i=0; i < PL_STAGE_3_INVOCATIONS; i++)
    {
        if ( totalRMS[i] > 0 && totalRMS[i] < minRMS )
        {           
            minRMS   = totalRMS[i];
            minIndex = i;            
        }
    }

    if ( minIndex > 0 && minRMS < lowestRMS )
    {
        graftPositions.resize( graftCount, plVector4( -1.0f, -1.0f, -1.0f, -1.0f ) );
        graftNormals.resize( graftCount, plVector4( -1.0f, -1.0f, -1.0f, -1.0f ) );
        lowestRMS = minRMS;

        donorSolutionPositionsSSBO.read( graftPositions, graftPositions.size(), 0, minIndex*PL_MAX_GRAFTS_PER_SOLUTION );
        donorSolutionNormalsSSBO.read( graftNormals, graftNormals.size(), 0, minIndex*PL_MAX_GRAFTS_PER_SOLUTION );
    }

}


namespace plPlannerStage3
{
   
    void run( plDonorSolution &donorSolution, const plPlanningBufferData &planningData, const plDefectSolution &defectSolution, const plRmsData &rmsData )
    {
        std::vector< plString > shaderfiles;
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/defines.hcmp" ); 

        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/geometry.hcmp" );
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/defectSite.hcmp" );  
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/donorSites.hcmp" );
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/defectSolution.hcmp" );
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/donorSolution.hcmp" );
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/rms.hcmp" );   
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/rand.hcmp" );  
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/stage3.hcmp" );

        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/geometry.cmp" );
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/defectSite.cmp" );  
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/donorSites.cmp" ); 
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/defectSolution.cmp" );
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/donorSolution.cmp" );
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/rms.cmp" );      
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/rand.cmp" ); 
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/stage3.cmp" );
        
        // compile / link stage 2 shader
        plPlannerStage3Shader stage3Shader( shaderfiles );
        
        if ( !stage3Shader.good() )
            return;
        
        stage3Shader.bind(); 
 
        // set uniforms
        stage3Shader.setDefectSiteUniforms( planningData.defectSite ); 
        stage3Shader.setDonorSiteUniforms( planningData.donorSites );
        stage3Shader.setDefectSolutionUniforms( defectSolution );
        stage3Shader.setRotationAngleUniforms();

        reportOpenGLError( "compiled/n" );

        // create and initialize cap indices SSBOs to 0
        std::vector<plVector4> donorSolutionFiller( PL_STAGE_3_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION, plVector4( -1.0f, -1.0f, -1.0f, -1.0f ) );  
        plSSBO donorSolutionPositionsSSBO, donorSolutionNormalsSSBO;
        donorSolutionPositionsSSBO.set( donorSolutionFiller, donorSolutionFiller.size() );
        donorSolutionNormalsSSBO.set( donorSolutionFiller, donorSolutionFiller.size() );

        plSSBO totalRmsSSBO;
        std::vector<PLfloat> totalRMS( PL_STAGE_3_INVOCATIONS, 1.0f );
        totalRmsSSBO.set( totalRMS, totalRMS.size() );

        // bind SSBOs
        planningData.defectSiteSSBO.bind( 0 );
        planningData.donorSitesSSBO.bind( 1 );
        rmsData.rmsSSBO.bind( 2 );
        donorSolutionPositionsSSBO.bind( 3 );
        donorSolutionNormalsSSBO.bind( 4 );
        totalRmsSSBO.bind( 5 );


        for (PLuint i=0; i<PL_STAGE_3_ITERATIONS; i++ )
        {
            // update seed uniform
            stage3Shader.setSeedUniform();
            reportOpenGLError( "BEFORE OF SHADER STAGE 3\n" ); 
            // call compute shader with 1D workgrouping
            glDispatchCompute( PL_STAGE_3_NUM_GROUPS, 1, 1 );
            
            // memory barrier      
            glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ); 
            
            donorSolution.extractBestSolution( defectSolution.graftCount, totalRmsSSBO, donorSolutionPositionsSSBO, donorSolutionNormalsSSBO );
 
            std:: cout << "\tIteration " << i <<", Current lowest total RMS: " << donorSolution.lowestRMS << "\n";
        }    

        // no state found                              
        if ( donorSolution.lowestRMS == FLT_MAX )
        {
            donorSolution.graftPositions.clear();
            donorSolution.graftNormals.clear();
            std::cerr << "plPlannerStage3::run() error: Unable to find suitable harvest locations \n";
        }    
        
        // unbind and delete site and rms buffers
        planningData.defectSiteSSBO.unbind( 0 );
        planningData.donorSitesSSBO.unbind( 1 );
        rmsData.rmsSSBO.unbind( 2 );
        donorSolutionPositionsSSBO.unbind( 3 );
        donorSolutionNormalsSSBO.unbind( 4 );
        totalRmsSSBO.unbind( 5 );
        
        /*
        if (donorSolution.graftPositions.size() > 0 )
        {
            std::cout << "\tDonor locations and normals:\n";    
            for (PLuint i=0; i < donorSolution.graftCount; i++)
            {
                std::cout << "\t\tPosition "    << i << ": \t" << donorSolution.graftPositions[i]   << "\n";
                std::cout << "\t\tNormal "      << i << ": \t" << donorSolution.graftNormals[i]     << "\n";
            }
        }
        */





        /*
        reportOpenGLError( "BEFORE OF SHADER STAGE 3\n" ); 
        // compile / link stage 1 shader
        plPlannerStage3Shader stage3Shader( PL_FILE_PREPATH"shaders/plannerStage3.comp" );
        stage3Shader.bind(); 
              
        // calc total grid points (invocations)
        PLuint totalGridPoints = 0;
        std::vector<PLuint> donorGridSizes;
        std::vector<PLuint> donorByteOffset;
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
        stage3Shader.setGraftUniforms( defectState.graftCount,
                                       rmsInput.sets.size(),    // number of individual directions
                                       defectState.graftRadii );

        stage3Shader.setSiteUniforms( donorSites.size(),
                                      totalGridPoints,
                                      donorGridSizes,
                                      donorByteOffset );
            
        for (PLuint i=0; i<PL_STAGE3_ITERATIONS; i++ )
        {
            // update seed uniform
            stage3Shader.setSeedUniform();
            reportOpenGLError( "BEFORE OF SHADER STAGE 3\n" ); 
            // call compute shader with 1D workgrouping
            glDispatchCompute( PL_STAGE3_NUM_GROUPS, 1, 1 );
            
            // memory barrier      
            glMemoryBarrier( GL_ALL_BARRIER_BITS ); //GL_SHADER_STORAGE_BARRIER_BIT);

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
            std::cerr << "plPlannerStage3::run() error: Unable to find suitable harvest locations \n";
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
        */   
    }
    

}























