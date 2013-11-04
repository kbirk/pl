#include "plPlannerStage3.h"

plGreedyGroup::plGreedyGroup()
    :   _lowestRMS       ( FLT_MAX ),
        _lowestPositions ( PL_MAX_GRAFTS_PER_SOLUTION, plVector4() ),
        _lowestNormals   ( PL_MAX_GRAFTS_PER_SOLUTION, plVector4() ),        
        _donorSolutionPositionsSSBO ( PL_STAGE_3_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION*sizeof( plVector4 ) ),
        _donorSolutionNormalsSSBO   ( PL_STAGE_3_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION*sizeof( plVector4 ) ),
        _totalRmsSSBO               ( PL_STAGE_3_INVOCATIONS*sizeof( PLfloat ) )       
{   
    // initialize all rms to -1
    std::vector<PLfloat> totalRMS( PL_STAGE_3_INVOCATIONS, 1.0f );
    _totalRmsSSBO.set( totalRMS, totalRMS.size() );
}       
 
       
void plGreedyGroup::bind()
{
    _donorSolutionPositionsSSBO.bind( 3 );
    _donorSolutionNormalsSSBO.bind( 4 );
    _totalRmsSSBO.bind( 5 );
}


void plGreedyGroup::unbind()
{
    _donorSolutionPositionsSSBO.unbind( 3 );
    _donorSolutionNormalsSSBO.unbind( 4 );
    _totalRmsSSBO.unbind( 5 );   
}


void plGreedyGroup::update()
{
    std::vector<PLfloat> totalRMS( PL_STAGE_3_INVOCATIONS, 1.0f );
    _totalRmsSSBO.read( totalRMS, totalRMS.size() );

    // find invocation with lowest RMS
    float minRMS   = FLT_MAX;
    PLint minIndex = -1;    

    // get lowest rms index
    for (PLuint i=0; i < PL_STAGE_3_INVOCATIONS; i++)
    {
        if ( totalRMS[i] > 0 && totalRMS[i] < minRMS )
        {           
            minRMS   = totalRMS[i];
            minIndex = i;            
        }
    }

    // is it better than global solution?
    if ( minIndex > 0 && minRMS < _lowestRMS )
    {
        _lowestRMS = minRMS;

        _donorSolutionPositionsSSBO.read( _lowestPositions, PL_MAX_GRAFTS_PER_SOLUTION, 0, minIndex*PL_MAX_GRAFTS_PER_SOLUTION );
        _donorSolutionNormalsSSBO.read( _lowestNormals, PL_MAX_GRAFTS_PER_SOLUTION, 0, minIndex*PL_MAX_GRAFTS_PER_SOLUTION );
    }

}


void plGreedyGroup::getSolution( plDonorSolution &solution )
{
    solution.graftPositions = _lowestPositions;
    solution.graftNormals   = _lowestNormals;  
    solution.rms = _lowestRMS;
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
        stage3Shader.setRotationAngleUniforms( PL_NUM_COMPARISION_DIRECTIONS );

        reportOpenGLError( "compiled/n" );

        // create and initialize cap indices SSBOs to 0
        plGreedyGroup greedyBuffers;

        // bind SSBOs
        planningData.defectSiteSSBO.bind( 0 );
        planningData.donorSitesSSBO.bind( 1 );
        rmsData.rmsSSBO.bind( 2 );
        greedyBuffers.bind(); // 3, 4, 5


        for (PLuint i=0; i<PL_STAGE_3_ITERATIONS; i++ )
        {
            // update seed uniform
            stage3Shader.setSeedUniform();
            reportOpenGLError( "BEFORE OF SHADER STAGE 3\n" ); 
            // call compute shader with 1D workgrouping
            glDispatchCompute( PL_STAGE_3_NUM_GROUPS, 1, 1 );
            
            // memory barrier      
            glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ); 
            
            // check latest solution
            greedyBuffers.update();
 
            std:: cout << "\tIteration " << i <<", Current lowest total RMS: " << greedyBuffers.lowestRMS() << "\n";
        }    

        greedyBuffers.getSolution( donorSolution );

        // no state found                              
        if ( donorSolution.rms == FLT_MAX )
        {
            std::cerr << "plPlannerStage3::run() error: Unable to find suitable harvest locations \n";
        }    
        
        // unbind and delete site and rms buffers
        planningData.defectSiteSSBO.unbind( 0 );
        planningData.donorSitesSSBO.unbind( 1 );
        rmsData.rmsSSBO.unbind( 2 );
        greedyBuffers.unbind();
        
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
    }
    

}























