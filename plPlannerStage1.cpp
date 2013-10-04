#include "plPlannerStage1.h"


void plCapIndices::update( PLuint defectBuffer, PLuint defectSize, PLuint donorBuffer, PLuint donorSize )
{
    defectIndices.fill( defectSize, 0 );
    donorIndices.fill ( donorSize, 0  );
       
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


namespace plPlannerStage1
{

    plMesh DEBUG;

    void run( plCapIndices &capData, const plSiteGrid &defectSite, const plSeq<plSiteGrid> &donorSites, const plDefectState &defectState )
    {     
        // compile / link stage 1 shader
        plPlannerStage1Shader stage1Shader("./shaders/plannerStage1.comp");
        stage1Shader.bind(); 
        
        // calc total grid points (invocations)
        PLuint totalGridPoints = 0;
        plSeq<PLuint> donorGridSizes;
        plSeq<PLuint> donorMeshSizes;       
        //plSeq<PLuint> donorPerimSizes;
        plSeq<PLuint> donorByteOffset;
        for (PLuint i=0; i < donorSites.size(); i++)
        {
            totalGridPoints +=  donorSites[i].gridSize();
            donorGridSizes.add  ( donorSites[i].gridSize()  );
            donorMeshSizes.add  ( donorSites[i].meshSize()  );           
            //donorPerimSizes.add ( donorSites[i].perimSize() );
            if ( i == 0)
                donorByteOffset.add ( 0 );
            else
                donorByteOffset.add ( donorSites[i-1].gridSize()*2 + donorSites[i-1].meshSize()*4 ); //+ donorSites[i-1].perimSize()*2 );
        }

        const PLuint CAP_INDICES_BUFFER_SIZE = totalGridPoints*PL_MAX_CAP_TRIANGLES;

        // generate and fill buffers 
        PLuint defectSiteDataBufferID   = defectSite.getMeshSSBO();
        PLuint donorSiteDataBufferID    = getGroupGridAndMeshSSBO( donorSites );   
        PLuint defectCapIndicesBufferID = createSSBO<PLuint>( CAP_INDICES_BUFFER_SIZE, 0 );
        PLuint donorCapIndicesBufferID  = createSSBO<PLuint>( CAP_INDICES_BUFFER_SIZE, 0 );        

        // bind buffers
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, defectSiteDataBufferID   );    
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, donorSiteDataBufferID    );  
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, defectCapIndicesBufferID );  
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, donorCapIndicesBufferID  );  
    
        const PLuint NUM_WORKGROUPS = ceil( totalGridPoints + defectState.graftCount / (PLfloat) PL_STAGE1_GROUP_SIZE ); // ensure enough workgroups are used

        // set state uniforms
        stage1Shader.setGraftUniforms( defectState.graftCount,
                                       defectState.graftPositions,
                                       defectState.graftNormals );
        // set site uniforms
        stage1Shader.setSiteUniforms( defectSite.meshSize(),
                                      donorSites.size(),
                                      donorMeshSizes,
                                      donorGridSizes,
                                      donorByteOffset );
                                  
        // call compute shader with 1D workgrouping
        glDispatchCompute( NUM_WORKGROUPS, 1, 1 );
        
        // memory barrier      
        glMemoryBarrier( GL_ALL_BARRIER_BITS ); //GL_SHADER_STORAGE_BARRIER_BIT);

        
        capData.update( defectCapIndicesBufferID, defectState.graftCount*PL_MAX_CAP_TRIANGLES, donorCapIndicesBufferID, totalGridPoints*PL_MAX_CAP_TRIANGLES );

        
        plSeq<plTriangle> tris;
        
        //for ( PLuint i=0; i < 5; i++ )
        //{
            PLuint i = 0; //donorSites[0].gridSize() + 10;
            PLuint offset = i*PL_MAX_CAP_TRIANGLES;
        
            std::cout << "Count: " << capData.defectIndices[offset] << std::endl;
            
            for ( PLuint j=1; j < capData.defectIndices[offset]+1; j++ )
            {
                std::cout << capData.defectIndices[ i*PL_MAX_CAP_TRIANGLES + j] << " ";
                               
                tris.add( defectSite.triangles( capData.defectIndices[ i*PL_MAX_CAP_TRIANGLES + j] ) );
            }
                        
            std::cout << std::endl << std::endl;
        //}
        
        
        // DEBUG      
        DEBUG = plMesh( tris );
        
        
        // unbind buffers
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 );           
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, 0 );    
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, 0 );
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, 0 );
        
        // delete buffers
        glDeleteBuffers( 1, &defectSiteDataBufferID   );
        glDeleteBuffers( 1, &donorSiteDataBufferID    );
        glDeleteBuffers( 1, &defectCapIndicesBufferID );
        glDeleteBuffers( 1, &donorCapIndicesBufferID  );

        // DEBUG
        /*
        for (PLuint i=0; i < PL_STAGE1_NUM_DIRECTIONS; i++)
        {
            std::cout << " dir: " << i << "\n";
            for (PLuint j=0; j < 10; j++)
            {
                std::cout << "rms:\t" << rmsData.sets[i].rms[j] << "\n";
            }
        }
        */

    }

}























