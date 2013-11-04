#include "plPlannerStage1.h"

/*
void plCapIndices::update( PLuint defectBuffer, PLuint defectSize, PLuint donorBuffer, PLuint donorSize )
{
    //defectIndices.fill( defectSize, 0 );
    //donorIndices.fill ( donorSize, 0  );
    defectIndices.assign( defectSize, 0 );
    donorIndices.assign ( donorSize, 0  );  

    glBindBuffer( GL_SHADER_STORAGE_BUFFER, defectBuffer );            
    PLuint *defectData = readSSBO<PLuint>( 0, defectSize );
    memcpy( &defectIndices[0], &defectData[0], defectSize*sizeof( PLuint ) );    
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    glBindBuffer( GL_SHADER_STORAGE_BUFFER, donorBuffer );            
    PLuint *donorData = readSSBO<PLuint>( 0, donorSize );
    memcpy( &donorIndices[0], &donorData[0], donorSize*sizeof( PLuint ) );    
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
} 


PLuint plCapIndices::getDefectSSBO() const
{
    PLuint tempBuffer;
    glGenBuffers(1, &tempBuffer);   
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, tempBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, defectIndices.size()*sizeof(PLuint), &defectIndices[0], GL_STATIC_READ);
    return tempBuffer;
} 


PLuint plCapIndices::getDonorSSBO() const
{
    PLuint tempBuffer;
    glGenBuffers(1, &tempBuffer);   
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, tempBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, donorIndices.size()*sizeof(PLuint), &donorIndices[0], GL_STATIC_READ);
    return tempBuffer;
} 
*/

namespace plPlannerStage1
{
    void run( plCapIndices &capData, const plPlanningBufferData &planningData, const plDefectSolution &defectSolution )
    {    
        
        std::vector< plString > shaderfiles;
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/defines.hcmp" ); 

        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/geometry.hcmp" );
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/defectSite.hcmp" );  
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/donorSites.hcmp" );
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/defectSolution.hcmp" );
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/capIndices.hcmp" );       
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/stage1.hcmp" );

        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/geometry.cmp" );
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/defectSite.cmp" );  
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/donorSites.cmp" ); 
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/defectSolution.cmp" );
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/capIndices.cmp" );        
        shaderfiles.push_back( PL_FILE_PREPATH"shaders/planning/stage1.cmp" );

        // compile / link stage 1 shader
        plPlannerStage1Shader stage1Shader( shaderfiles );
        
        if ( !stage1Shader.good() )
            return;
        
        stage1Shader.bind(); 

        // set uniforms
        stage1Shader.setDefectSiteUniforms( planningData.defectSite ); 
        stage1Shader.setDonorSiteUniforms( planningData.donorSites );
        stage1Shader.setDefectSolutionUniforms( defectSolution );
    
        reportOpenGLError( "compiled/n" );

        // create and initialize cap indices SSBOs to 0
        std::vector<PLuint> defectIndices( defectSolution.graftCount*PL_MAX_CAP_TRIANGLES, 0 );
        std::vector<PLuint> donorIndices( planningData.totalDonorGridPoints()*PL_MAX_CAP_TRIANGLES, 0 );

        capData.defectCapIndexSSBO.set( defectIndices, defectIndices.size() );
        capData.donorCapIndexSSBO.set ( donorIndices,  donorIndices.size()  );

        // bind SSBOs
        planningData.defectSiteSSBO.bind( 0 );
        planningData.donorSitesSSBO.bind( 1 );
        capData.defectCapIndexSSBO.bind( 2 );
        capData.donorCapIndexSSBO.bind( 3 );
        
        reportOpenGLError( "bind/n" );
            
        const PLuint NUM_WORKGROUPS = ceil( planningData.totalDonorGridPoints() + defectSolution.graftCount / (PLfloat) PL_STAGE_1_GROUP_SIZE ); // ensure enough workgroups are used    
            
        // call compute shader with 1D workgrouping
        glDispatchCompute( NUM_WORKGROUPS, 1, 1 );
        
        // memory barrier      
        glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );

        reportOpenGLError( "go/n" );

        // DEBUG
        capData.defectCapIndexSSBO.read( defectIndices, defectIndices.size() );
        capData.donorCapIndexSSBO.read ( donorIndices,  donorIndices.size()  );
        
        std::cout << std::endl << "DEBUG: " << std::endl;       
        for ( PLuint i=0; i<defectIndices.size(); i+=PL_MAX_CAP_TRIANGLES )
        {
            std::cout << "Graft " << i/PL_MAX_CAP_TRIANGLES << ",\t cap index count: " << defectIndices[i] << std::endl;
        }
        std::cout << std::endl;
        /*
        std::cout << "Donor index counts: " << std::endl;
        for ( PLuint i=0; i<PL_MAX_CAP_TRIANGLES*100; i+=PL_MAX_CAP_TRIANGLES )
        {
            std::cout << i/PL_MAX_CAP_TRIANGLES << ": " << donorIndices[i] << "  ";
        }
        */
        //

        planningData.defectSiteSSBO.unbind( 0 );
        planningData.donorSitesSSBO.unbind( 1 );
        capData.defectCapIndexSSBO.unbind( 2 );
        capData.donorCapIndexSSBO.unbind( 3 );
        
        reportOpenGLError( "done/n" );
    
    }
    
}























