#include "plPlannerStage1.h"

namespace plPlannerStage1
{

    void run( const plSiteGrid &defectSite, const plSeq<plSiteGrid> &donorSites, const plAnnealingState &state )
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

        // generate and fill buffers 
        PLuint defectSiteDataBufferID      = defectSite.getMeshSSBO();
        //PLuint donorSiteDataBufferID       =     
        PLuint tempDefectTrianglesBufferID = createSSBO( totalGridPoints*PL_STAGE1_MAX_CAP_TRIANGLES*4, plVector4(-1,-1,-1,-1) );
        PLuint tempDonorTrianglesBufferID  = createSSBO( totalGridPoints*PL_STAGE1_MAX_CAP_TRIANGLES*4, plVector4(-1,-1,-1,-1) );        
        PLuint rmsOutputBuffer             = createSSBO( totalGridPoints*PL_MAX_GRAFTS_PER_SOLUTION, -1 );
   
        // bind buffers
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, defectSiteDataBufferID      );    
        //glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, donorSiteDataBufferID       );  
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, tempDefectTrianglesBufferID );  
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, tempDonorTrianglesBufferID  );  
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, rmsOutputBuffer             );  
    
        PLuint workgroups = ceil( totalGridPoints / (PLfloat) PL_STAGE1_GROUP_SIZE); // ensure enough workgroups are used

        // set state uniforms
        stage1Shader.setGraftUniforms( state.graftCount,
                                       state.graftPositions,
                                       state.graftNormals,
                                       state.graftRadii );

        stage1Shader.setSiteUniforms( defectSite.meshSize(),
                                      donorSites.size(),
                                      donorMeshSizes,
                                      donorGridSizes,
                                      donorPerimSizes );
                                      
                                      

        // copy rms data into independant buffer
        plSeq<PLfloat> rmsData( totalGridPoints*PL_MAX_GRAFTS_PER_SOLUTION, -1.0 ); 
                      
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, rmsOutputBuffer);            
        PLfloat *rms = readSSBO<PLfloat>( 0, totalGridPoints*PL_MAX_GRAFTS_PER_SOLUTION );
        memcpy( &rmsData[0], &rms[0], totalGridPoints*PL_MAX_GRAFTS_PER_SOLUTION*sizeof( PLfloat ) );    
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        // unbind buffers
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 );           
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, 0 );    
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, 0 );
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, 0 );
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, 0 );
        
        // delete buffers
        glDeleteBuffers( 1, &defectSiteDataBufferID      );
        //glDeleteBuffers( 1, &donorSiteDataBufferID       );
        glDeleteBuffers( 1, &tempDefectTrianglesBufferID );
        glDeleteBuffers( 1, &tempDonorTrianglesBufferID  );
        glDeleteBuffers( 1, &rmsOutputBuffer             );

    }

}























