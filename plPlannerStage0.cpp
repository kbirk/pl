#include "plPlannerStage0.h"


plAnnealingGroup::plAnnealingGroup( PLfloat initialEnergy )
    : _invoEnergiesSSBO      ( PL_STAGE_0_INVOCATIONS*sizeof( PLfloat ) ),      
      _invoGraftPositionsSSBO( PL_STAGE_0_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION*sizeof( plVector4 ) ),
      _invoGraftNormalsSSBO  ( PL_STAGE_0_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION*sizeof( plVector4 ) ),
      _invoGraftRadiiSSBO    ( PL_STAGE_0_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION*sizeof( PLfloat ) ),
      _invoGraftCountsSSBO   ( PL_STAGE_0_INVOCATIONS*sizeof( PLuint  ) ),
      
      _groupEnergiesSSBO       ( PL_STAGE_0_NUM_GROUPS*sizeof( PLfloat ) ), // &std::vector<PLfloat>( PL_STAGE_0_NUM_GROUPS, initialEnergy )[0] ),      
      _groupGraftPositionsSSBO ( PL_STAGE_0_NUM_GROUPS*PL_MAX_GRAFTS_PER_SOLUTION*sizeof( plVector4 ) ),
      _groupGraftNormalsSSBO   ( PL_STAGE_0_NUM_GROUPS*PL_MAX_GRAFTS_PER_SOLUTION*sizeof( plVector4 ) ),
      _groupGraftRadiiSSBO     ( PL_STAGE_0_NUM_GROUPS*PL_MAX_GRAFTS_PER_SOLUTION*sizeof( PLfloat ) ),
      _groupGraftCountsSSBO    ( PL_STAGE_0_NUM_GROUPS*sizeof( PLuint  ) )
{ 
    std::vector<PLfloat> energies( PL_STAGE_0_NUM_GROUPS, initialEnergy );
    _groupEnergiesSSBO.set< PLfloat >( energies, PL_STAGE_0_NUM_GROUPS );
}


void plAnnealingGroup::bind()
{       
    _invoEnergiesSSBO.bind      ( 2 );
    _invoGraftPositionsSSBO.bind( 3 );
    _invoGraftNormalsSSBO.bind  ( 4 );
    _invoGraftRadiiSSBO.bind    ( 5 );
    _invoGraftCountsSSBO.bind   ( 6 );

    _groupEnergiesSSBO.bind      ( 7 );
    _groupGraftPositionsSSBO.bind( 8 );
    _groupGraftNormalsSSBO.bind  ( 9 );
    _groupGraftRadiiSSBO.bind    ( 10 );
    _groupGraftCountsSSBO.bind   ( 11 );
}


void plAnnealingGroup::unbind()
{   
    _invoEnergiesSSBO.unbind      ( 2 );
    _invoGraftPositionsSSBO.unbind( 3 );
    _invoGraftNormalsSSBO.unbind  ( 4 );
    _invoGraftRadiiSSBO.unbind    ( 5 );
    _invoGraftCountsSSBO.unbind   ( 6 );

    _groupEnergiesSSBO.unbind      ( 7 );
    _groupGraftPositionsSSBO.unbind( 8 );
    _groupGraftNormalsSSBO.unbind  ( 9 );
    _groupGraftRadiiSSBO.unbind    ( 10 );
    _groupGraftCountsSSBO.unbind   ( 11 ); 
}


void plAnnealingGroup::getSolution( plDefectSolution &solution, const plPlanningBufferData &planningData )
{
    PLuint index;
    PLfloat energy;
    getLowestGroupInfo( index, energy );

    // read graft count
    _groupGraftCountsSSBO.readBytes<PLuint>( &solution.graftCount, sizeof(PLuint), 0, index*sizeof(PLuint) );
    _groupGraftPositionsSSBO.read<plVector4>( solution.graftPositions, solution.graftCount, 0, index*PL_MAX_GRAFTS_PER_SOLUTION );
    _groupGraftNormalsSSBO.read<plVector4>( solution.graftNormals, solution.graftCount, 0, index*PL_MAX_GRAFTS_PER_SOLUTION );
    _groupGraftRadiiSSBO.read<PLfloat>( solution.graftRadii, solution.graftCount, 0, index*PL_MAX_GRAFTS_PER_SOLUTION ); 
    
    // re-compute positions as perturbations will shift them off the mesh surface!
    for ( PLuint i=0; i < solution.graftCount; i++ )
    {
        plVector3 rayOrigin   ( solution.graftPositions[i].x, solution.graftPositions[i].y, solution.graftPositions[i].z );
        plVector3 rayDirection( solution.graftNormals[i].x,   solution.graftNormals[i].y,   solution.graftNormals[i].z   );
    
        plIntersection intersection = plMath::rayIntersect( planningData.defectSite.triangles, rayOrigin, rayDirection, true );

        solution.graftPositions[i] = plVector4( intersection.point,  1.0f );
        solution.graftSurfaceNormals.push_back( plVector4( intersection.normal, 1.0f ) ); //plVector4( planningData.defectSite.getSmoothNormal( intersection.point, intersection.normal, PL_NORMAL_SMOOTHING_RADIUS ), 1.0f ) );
    }   
}


void plAnnealingGroup::getLowestGroupInfo( PLuint &index, float &energy )
{
    // read energies
    std::vector< PLfloat > energies;
    _groupEnergiesSSBO.read<PLfloat>( energies, PL_STAGE_0_NUM_GROUPS );

    // find best group
    PLfloat lowestEnergy = FLT_MAX;
    PLuint  lowestGroup  = 0;
    
    for ( PLuint i=0; i < PL_STAGE_0_NUM_GROUPS; i++ )
    {    
        if ( energies[i] < lowestEnergy )
        {    
            lowestEnergy = energies[i];
            lowestGroup = i;
        }
    }

    energy = lowestEnergy;
    index  = lowestGroup;
}


PLuint logBN( float base, float num )
{
    return log10( num ) / log10( base );
}


namespace plPlannerStage0
{

    void run( plDefectSolution &defectSolution, const plPlanningBufferData &planningData, plPlan& plan )
    {       
        std::vector< std::string > shaderfiles;
        
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/defines.hcmp" ); 
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/geometry.hcmp" );
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/defectSite.hcmp" );  
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/state.hcmp" );        
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/annealing.hcmp" );
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/rand.hcmp" );        
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/stage0.hcmp" );

        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/geometry.cmp" );
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/defectSite.cmp" );  
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/state.cmp" );        
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/annealing.cmp" );
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/rand.cmp" );        
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/stage0.cmp" );

        // compile / link stage 0 shader
        plPlannerShader stage0Shader( shaderfiles );
        
        if ( !stage0Shader.good() )
            return;
        
        stage0Shader.bind(); // bind shader  
        stage0Shader.setDefectSiteUniforms( planningData.defectSite ); 

        plSSBO triangleAreaSSBO ( planningData.defectSite.triangles.size()*PL_STAGE_0_INVOCATIONS*sizeof( PLfloat ) ); 
        plAnnealingGroup annealingBuffers( planningData.defectSite.area );

        planningData.defectSiteSSBO.bind( 0 );
        triangleAreaSSBO.bind( 1 ); 
        annealingBuffers.bind(); // 2, 3, 4, 5, 6, 7, 8 , 9, 10, 11

        PLfloat temperature = PL_STAGE_0_INITIAL_TEMPERATURE;

        PLuint TOTAL_ITERATIONS = logBN( 1.0 - PL_STAGE_0_COOLING_RATE, PL_STAGE_0_STOPPING_TEMPERATURE );
        PLuint iterationNum = 0;
        
        // simulated annealing                
        while ( temperature > PL_STAGE_0_STOPPING_TEMPERATURE )
        {
            // group iteration
            stage0Shader.setLocalLoadUniform  ( 0 );         
            stage0Shader.setTemperatureUniform( temperature ); 

            PLuint its = PLuint( PL_STAGE_0_ITERATIONS * temperature ) + 1;
           
            for ( PLint i=its; i>=0; i-- )
            {       
                // local iteration
                // call compute shader with 1D workgrouping
#ifndef SKIP_COMPUTE_SHADER
                glDispatchCompute( PL_STAGE_0_NUM_GROUPS, 1, 1 );
#endif
                // memory barrier
                glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );
                
                stage0Shader.setLocalLoadUniform( i );  
                stage0Shader.setSeedUniform();
            }

            PLuint bestGroup;
            PLfloat energy;
            // get best group info 
            annealingBuffers.getLowestGroupInfo( bestGroup, energy );   

            // cool temperature
            temperature *= 1 - PL_STAGE_0_COOLING_RATE;
            
            plUtility::printProgressBar( iterationNum++ / (float)TOTAL_ITERATIONS );
        }
        plUtility::printProgressBar( 1.0 );

        // load global solution from annealing state to defect state
        annealingBuffers.getSolution( defectSolution, planningData );
        
        /* DEBUG
        std::cout << std::endl << "DEBUG: " << std::endl;
        for ( PLuint i=0; i<defectSolution.graftCount; i++)
        {
            std::cout << "Graft " << i << ",    Position: " << defectSolution.graftPositions[i] 
                                       << ",    Normal: "   << defectSolution.graftNormals[i] 
                                       << ",    Radius: "   << defectSolution.graftRadii[i] << std::endl;         
        }
        std::cout << std::endl;
        */

        // unbind and delete site and temporary buffers
        planningData.defectSiteSSBO.unbind( 0 );
        triangleAreaSSBO.unbind( 1 ); 
        annealingBuffers.unbind();
    }

}






















