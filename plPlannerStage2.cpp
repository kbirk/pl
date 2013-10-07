#include "plPlannerStage2.h"


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


namespace plPlannerStage2
{

    plSeq<plVector3> DEBUG;

    void run( plRmsData &rmsData, const plSiteGrid &defectSite, const plSeq<plSiteGrid> &donorSites, const plDefectState &defectState, const plCapIndices &capIndices )
    {  
        // compile / link stage 1 shader
        plPlannerStage2Shader stage2Shader("./shaders/plannerStage2.comp");
        stage2Shader.bind(); 
        
        // calc total grid points (invocations)
        PLuint totalGridPoints = 0;
        plSeq<PLuint> donorGridSizes;
        plSeq<PLuint> donorMeshSizes;       
        plSeq<PLuint> donorPerimSizes;
        plSeq<PLuint> donorByteOffset;

        plSeq<plVector3> donorPosTEMP; //
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
            //
            for (PLuint j=0; j < donorSites[i].gridSize(); j++)
            {
                donorPosTEMP.add ( plVector3( donorSites[i].points( j ).x, donorSites[i].points( j ).y, donorSites[i].points( j ).z ) );
            }
        }

        const PLuint TRIANGLE_BUFFER_SIZE = totalGridPoints*PL_MAX_CAP_TRIANGLES*4;
        const PLuint RMS_BUFFER_SIZE      = totalGridPoints*PL_MAX_GRAFTS_PER_SOLUTION;      

        // generate and fill buffers 
        PLuint defectSiteDataBufferID   = defectSite.getMeshSSBO();
        PLuint donorSiteDataBufferID    = getGroupFullSSBO( donorSites );   
        PLuint defectCapIndicesBufferID = capIndices.getDefectSSBO();
        PLuint donorCapIndicesBufferID  = capIndices.getDonorSSBO();       
        PLuint rmsOutputBuffer          = createSSBO<PLfloat>( RMS_BUFFER_SIZE );
   
        // bind buffers
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, defectSiteDataBufferID   );    
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, donorSiteDataBufferID    );  
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, defectCapIndicesBufferID );  
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, donorCapIndicesBufferID  );  
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, rmsOutputBuffer          );  
    
        const PLuint NUM_WORKGROUPS = ceil( totalGridPoints / (PLfloat) PL_STAGE2_GROUP_SIZE); // ensure enough workgroups are used

        // set state uniforms
        stage2Shader.setGraftUniforms( defectState.graftCount,
                                       defectState.graftPositions,
                                       defectState.graftNormals,
                                       defectState.graftRadii );
        // set site uniforms
        stage2Shader.setSiteUniforms( defectSite.meshSize(),
                                      donorSites.size(),
                                      donorMeshSizes,
                                      donorGridSizes,
                                      donorPerimSizes,
                                      donorByteOffset );
                
        float da = (2.0f * PL_PI) / (float)(PL_STAGE2_NUM_DIRECTIONS);  
    
        plSeq<plVector3> lowestPosition ( defectState.graftCount, plVector3( -1, -1, -1 ) );
        plSeq<plVector4> lowestDirection( defectState.graftCount, plVector4( -1, -1, -1, -1 ) );        
        plSeq<PLuint>    lowestIndex    ( defectState.graftCount, -1 ); 
        plSeq<PLfloat>   lowestRMS      ( defectState.graftCount, FLT_MAX ); 
           
        for (PLuint i=0; i<PL_STAGE2_NUM_DIRECTIONS; i++)
        {       
            float theta = i * da;
             
            // get direction    
            plVector4 direction( sin( theta ), 0.0f, cos( theta ), 1.0f );
            
            // set direction uniform
            stage2Shader.setDirectionUniform( direction );  
                                                
            // call compute shader with 1D workgrouping
            glDispatchCompute( NUM_WORKGROUPS, 1, 1 );
            
            // memory barrier      
            glMemoryBarrier( GL_ALL_BARRIER_BITS ); //GL_SHADER_STORAGE_BARRIER_BIT);

            // update for direction  
            rmsData.update( RMS_BUFFER_SIZE, rmsOutputBuffer, direction );
        
            // DEBUG
            for (PLuint j=0; j < totalGridPoints; j++)
            {
                for (PLuint k=0; k < defectState.graftCount; k++)
                {
                    PLfloat rms = rmsData.sets[i].rms[ j*PL_MAX_GRAFTS_PER_SOLUTION+k ]; 
                    if ( rms > 0 && rms < lowestRMS[k] )
                    {
                        lowestRMS[k]      = rms;
                        lowestPosition[k] = donorPosTEMP[j];
                        lowestDirection[k] = rmsData.sets[i].direction;
                        lowestIndex[k]     = j;
                    }
                }
            }
            //
            std::cout << "\t Z Direction: " << direction << std::endl;

        }
        
        DEBUG = lowestPosition;
        
        for (PLuint i=0; i < defectState.graftCount; i++)
        {
            std::cout << "Graft " << i << ", pos: " << DEBUG[i] << ", dir: " << lowestDirection[i] << ", index " << lowestIndex[i]  << ", rms: " << lowestRMS[i] << std::endl;
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
        glDeleteBuffers( 1, &defectCapIndicesBufferID );
        glDeleteBuffers( 1, &donorCapIndicesBufferID  );
        glDeleteBuffers( 1, &rmsOutputBuffer             );

        // DEBUG
        /*
        for (PLuint i=0; i < PL_STAGE2_NUM_DIRECTIONS; i++)
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























