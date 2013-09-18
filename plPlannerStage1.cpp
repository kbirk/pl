#include "plPlannerStage1.h"


void plRmsData::update( PLuint bufferSize, PLuint rmsBuffer, const plVector4 &dir )
{
    plRmsSet set( bufferSize, dir );   

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, rmsBuffer);            
    PLfloat *rms = readSSBO<PLfloat>( 0, bufferSize );
    memcpy( &set.rms[0], &rms[0], bufferSize*sizeof( PLfloat ) );    
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    sets.add( set );
}  


PLuint plRmsData::getValuesSSBO() const
{                  
    PLuint dataSize = sets.size() * sets[0].rms.size();
    
    plSeq<float> data( dataSize );  
    for ( PLuint i=0; i < sets.size(); i++ ) 
    { 
        for ( PLuint j=0; j < sets[i].rms.size(); j++ ) 
        {
            data.add( sets[i].rms[j] ); 
        }      
    }
       
    PLuint tempBuffer;
    glGenBuffers(1, &tempBuffer);   
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, tempBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize*sizeof(float), &data[0], GL_STATIC_READ);
    return tempBuffer;
}


PLuint plRmsData::getDirectionsSSBO() const
{
    PLuint dataSize = sets.size();
    
    plSeq<plVector4> data( dataSize );  
    for ( PLuint i=0; i < sets.size(); i++ ) { data.add( sets[i].direction ); }
       
    PLuint tempBuffer;
    glGenBuffers(1, &tempBuffer);   
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, tempBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize*sizeof(plVector4), &data[0], GL_STATIC_READ);
    return tempBuffer;
}    


namespace plPlannerStage1
{

    void run( plRmsData &rmsData, const plSiteGrid &defectSite, const plSeq<plSiteGrid> &donorSites, const plDefectState &defectState )
    {
        
        // compile / link stage 1 shader
        plPlannerStage1Shader stage1Shader("./shaders/plannerStage1.comp");
        stage1Shader.bind(); 
        
        // calc total grid points (invocations)
        PLuint totalGridPoints = 0;
        plSeq<PLuint> donorGridSizes;
        plSeq<PLuint> donorMeshSizes;       
        plSeq<PLuint> donorPerimSizes;
        plSeq<PLuint> donorByteOffset;
        for (PLuint i=0; i < donorSites.size(); i++)
        {
            totalGridPoints +=  donorSites[i].gridSize();
            donorGridSizes.add  ( donorSites[i].gridSize()  );
            donorMeshSizes.add  ( donorSites[i].meshSize()  );           
            donorPerimSizes.add ( donorSites[i].perimSize() );
            if ( i == 0)
                donorByteOffset.add ( 0 );
            else
                donorByteOffset.add ( donorSites[i-1].gridSize()*2 + donorSites[i-1].meshSize()*4 + donorSites[i-1].perimSize()*2 );
        }

        const PLuint TRIANGLE_BUFFER_SIZE = totalGridPoints*PL_STAGE1_MAX_CAP_TRIANGLES*4;
        const PLuint RMS_BUFFER_SIZE      = totalGridPoints*PL_MAX_GRAFTS_PER_SOLUTION;      

        // generate and fill buffers 
        PLuint defectSiteDataBufferID      = defectSite.getMeshSSBO();
        PLuint donorSiteDataBufferID       = getGroupFullSSBO( donorSites );   
        PLuint tempDefectTrianglesBufferID = createSSBO<plVector4>( TRIANGLE_BUFFER_SIZE );
        PLuint tempDonorTrianglesBufferID  = createSSBO<plVector4>( TRIANGLE_BUFFER_SIZE );        
        PLuint rmsOutputBuffer             = createSSBO<PLfloat>( RMS_BUFFER_SIZE );
   
        // bind buffers
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, defectSiteDataBufferID      );    
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, donorSiteDataBufferID       );  
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, tempDefectTrianglesBufferID );  
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, tempDonorTrianglesBufferID  );  
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, rmsOutputBuffer             );  
    
        const PLuint NUM_WORKGROUPS = ceil( totalGridPoints / (PLfloat) PL_STAGE1_GROUP_SIZE); // ensure enough workgroups are used

        // set state uniforms
        stage1Shader.setGraftUniforms( defectState.graftCount,
                                       defectState.graftPositions,
                                       defectState.graftNormals,
                                       defectState.graftRadii );
        // set site uniforms
        stage1Shader.setSiteUniforms( defectSite.meshSize(),
                                      donorSites.size(),
                                      donorMeshSizes,
                                      donorGridSizes,
                                      donorPerimSizes,
                                      donorByteOffset );
                
        float da = (2.0f * PL_PI) / (float)(PL_STAGE1_NUM_DIRECTIONS);  
          
        for (PLuint i=0; i<PL_STAGE1_NUM_DIRECTIONS; i++)
        {       
            float theta = i * da;
             
            // get direction    
            plVector4 direction( sin( theta ), 0.0f, cos( theta ), 1.0f );
            
            // set direction uniform
            stage1Shader.setDirectionUniform( direction );  
                                                
            // call compute shader with 1D workgrouping
            glDispatchCompute( NUM_WORKGROUPS, 1, 1 );
            
            // memory barrier      
            glMemoryBarrier(GL_ALL_BARRIER_BITS); //GL_SHADER_STORAGE_BARRIER_BIT);

            // update for direction  
            rmsData.update( RMS_BUFFER_SIZE, rmsOutputBuffer, direction );
        
            std::cout << "\t Z Direction: " << direction << std::endl;

        }
        
        // unbind buffers
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 );           
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, 0 );    
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, 0 );
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, 0 );
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, 0 );
        
        // delete buffers
        glDeleteBuffers( 1, &defectSiteDataBufferID      );
        glDeleteBuffers( 1, &donorSiteDataBufferID       );
        glDeleteBuffers( 1, &tempDefectTrianglesBufferID );
        glDeleteBuffers( 1, &tempDonorTrianglesBufferID  );
        glDeleteBuffers( 1, &rmsOutputBuffer             );

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























