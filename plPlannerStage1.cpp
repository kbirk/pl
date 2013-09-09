#include "plPlannerStage1.h"

namespace plPlannerStage1
{

    plRmsData run( const plSiteGrid &defectSite, const plSeq<plSiteGrid> &donorSites, const plDefectState &defectState )
    {
        
        // compile / link stage 1 shader
        plPlannerStage1Shader stage1Shader("./shaders/plannerStage1.comp");
        stage1Shader.bind(); 
        
        // calc total grid points (invocations)
        PLuint totalGridPoints = 0;
        plSeq<PLuint> donorMeshSizes;
        plSeq<PLuint> donorGridSizes;
        plSeq<PLuint> donorPerimSizes;
        for (PLuint i=0; i < donorSites.size(); i++)
        {
            totalGridPoints += donorSites[i].gridSize();
            donorMeshSizes.add ( donorSites[i].meshSize()  );
            donorGridSizes.add ( donorSites[i].gridSize()  );
            donorPerimSizes.add( donorSites[i].perimSize() );
        }

        const PLuint TRIANGLE_BUFFER_SIZE = totalGridPoints*PL_STAGE1_MAX_CAP_TRIANGLES*4;
        const PLuint RMS_BUFFER_SIZE      = totalGridPoints*PL_MAX_GRAFTS_PER_SOLUTION;      

        // generate and fill buffers 
        PLuint defectSiteDataBufferID      = defectSite.getMeshSSBO();
        PLuint donorSiteDataBufferID       = getGroupFullSSBO( donorSites ); //donorSites[0].getFullSSBO();    
        PLuint tempDefectTrianglesBufferID = createSSBO( TRIANGLE_BUFFER_SIZE, plVector4(-1,-1,-1,-1) );
        PLuint tempDonorTrianglesBufferID  = createSSBO( TRIANGLE_BUFFER_SIZE, plVector4(-1,-1,-1,-1) );        
        PLuint rmsOutputBuffer             = createSSBO( RMS_BUFFER_SIZE, -1.0f );
   
        // bind buffers
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, defectSiteDataBufferID      );    
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, donorSiteDataBufferID       );  
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, tempDefectTrianglesBufferID );  
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, tempDonorTrianglesBufferID  );  
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, rmsOutputBuffer             );  
    
        const PLuint NUM_WORKGROUPS = ceil( totalGridPoints / (PLfloat) PL_STAGE1_GROUP_SIZE); // ensure enough workgroups are used

        plVector4 direction( 0, 0, 1, 1 );

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
                                      donorPerimSizes );
         
        plRmsData rmsData; 
          
        float da = 2.0f * PL_PI / PL_STAGE1_NUM_SLICES;  
          
        for (uint i=0; i<=PL_STAGE1_NUM_SLICES; i++)
        {       
            float a = (i == PL_STAGE1_NUM_SLICES) ? 0.0 : i * da;
             
            // get direction    
            plVector4 direction( sin( a ), 0.0f, -cos( a ), 1.0f );
            
            // set direction uniform
            stage1Shader.setDirectionUniform( direction );  
                                                
            // call compute shader with 1D workgrouping
            glDispatchCompute( NUM_WORKGROUPS, 1, 1 );
            
            // memory barrier      
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

            // update for direction  
            rmsData.update( RMS_BUFFER_SIZE, rmsOutputBuffer, direction );
        
            std::cout << "\t Z Direction: " << direction << "\n";
        }


        /*
        // copy rms data into independant buffer
        plSeq<PLfloat> rmsData( RMS_BUFFER_SIZE, -1.0f ); 
                      
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, rmsOutputBuffer);            
        PLfloat *rms = readSSBO<PLfloat>( 0, RMS_BUFFER_SIZE );
        memcpy( &rmsData[0], &rms[0], RMS_BUFFER_SIZE*sizeof( PLfloat ) );    
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        */
        
        
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

        /* DEBUG
        std::cout << "rms: "; 
        for (PLuint i=0; i < 50; i++)
        {
            std::cout << "\nGrid Point: " << i << ",";
            for (PLuint j=0; j < state.graftCount; j++)
            {
                std::cout << "\t" << rmsData[i*PL_MAX_GRAFTS_PER_SOLUTION + j];
            }
        } 
        */
        
        return rmsData;
    }

}























