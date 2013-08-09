#include "plAutomaticPlanner.h"

plSeq<plSiteGrid> plAutomaticPlanner::_donorSiteGrids;
plSeq<plSiteGrid> plAutomaticPlanner::_defectSiteGrids;

PLuint            plAutomaticPlanner::_gridPointsTextureID; 
PLuint            plAutomaticPlanner::_gridNormalsTextureID;                 
PLuint            plAutomaticPlanner::_siteMeshTextureID;
PLuint            plAutomaticPlanner::_potentialGraftCapsID;
PLuint            plAutomaticPlanner::_potentialGraftAreasID;     
PLuint            plAutomaticPlanner::_stateEnergiesTextureID;
PLuint            plAutomaticPlanner::_stateCapIndicesTextureID;

// DEBUG
plSeq<plMesh>     plAutomaticPlanner::DEBUG_MESH;

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
        std::cout << "\t Calculating defect site grid " << i << " \n";

        plSiteGrid grid( plan.defectSites(i).spline.triangles(), plan.defectSites(i).boundary );
        _defectSiteGrids.add( grid ); 
        
        std::cout << "\t\t " <<  grid.size() << " grid points calculated \n";
    }
    

    for ( PLuint i = 0; i < plan.donorSites().size(); i++)
    {      
        std::cout << "\t Calculating donor site grid " << i << " \n";

        plSiteGrid grid( plan.donorSites(i).model()->cartilage.triangles(), plan.donorSites(i).boundary);
        _donorSiteGrids.add( grid );
        
        std::cout << "\t\t " <<  grid.size() << " grid points calculated \n";
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

void plAutomaticPlanner::_dispatchStage0()
{
    PLuint gridSize = _defectSiteGrids[0].size();

    // compile / link stage 0 shader
    plPlannerStage0Shader stage0("./shaders/plannerStage0.comp");
    
    // bind shader
    stage0.bind();
    
    // get size uniforms
    stage0.setMeshSizeUniform( _defectSiteGrids[0].triangles().size() );
    
    // bind input/output buffers            
    glBindImageTexture(0, _gridPointsTextureID,   0, GL_FALSE, 0, GL_READ_ONLY,  GL_RGBA32F);
    glBindImageTexture(1, _gridNormalsTextureID,  0, GL_FALSE, 0, GL_READ_ONLY,  GL_RGBA32F);
    glBindImageTexture(2, _siteMeshTextureID,     0, GL_FALSE, 0, GL_READ_ONLY,  GL_RGBA32F);
    glBindImageTexture(3, _potentialGraftCapsID,  0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    glBindImageTexture(4, _potentialGraftAreasID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F   );

    // get workgroup number
    PLuint workgroups = ceil( gridSize / (PLfloat) 1024); // ensure enough workgroups are used

    // call compute shader with 1D workgrouping
    glDispatchCompute( workgroups, 1, 1 );
    
    // memory barrier
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
                     
    PLfloat *pixels = new PLfloat[ gridSize * PL_MAX_GRAFT_CAP_TRIANGLES * 4 * 4];
    
    // read output into array            
    glBindTexture(GL_TEXTURE_2D, _potentialGraftCapsID);    
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, &pixels[0]);    

    // unbind shader
    stage0.unbind();
    
    std::cout << pixels[0] << " " << pixels[1];
    
    // DEBU MESH STUFF
    int stride = PL_MAX_GRAFT_CAP_TRIANGLES * (4*4);

    for (int i=0; i<_defectSiteGrids[0].size(); i++) 
    {
        plSeq<plVector3> interleaved_vertices;
        plSeq<PLuint>    indices;

        for (int j=0; j < PL_MAX_GRAFT_CAP_TRIANGLES * (4*4); j+=(4*4) ) 
        {
        
            if ( pixels[i * stride + j] < 0)
            {
                // cap is done if reading -1
                break;
            }            

            plVector3 p0( pixels[i * stride + j + 0],
                          pixels[i * stride + j + 1],
                          pixels[i * stride + j + 2] );
            
            plVector3 p1( pixels[i * stride + j + 4],
                          pixels[i * stride + j + 5],
                          pixels[i * stride + j + 6] );;
                          
            plVector3 p2( pixels[i * stride + j + 8],
                          pixels[i * stride + j + 9],
                          pixels[i * stride + j + 10] ); 
                                       
            plVector3 n ( pixels[i * stride + j + 12],
                          pixels[i * stride + j + 13],
                          pixels[i * stride + j + 14] );
            
            int base = interleaved_vertices.size()/2;
        
            interleaved_vertices.add( p0 );
            interleaved_vertices.add( n );
            
            interleaved_vertices.add( p1 );
            interleaved_vertices.add( n );
            
            interleaved_vertices.add( p2 );
            interleaved_vertices.add( n );
            
            indices.add(base);
            indices.add(base+1);
            indices.add(base+2);
        }
        
        DEBUG_MESH.add( plMesh( interleaved_vertices, indices ) ); 
    }
    
    
    
}


void plAutomaticPlanner::_bufferTextures()
{
    plSiteGrid &grid = _defectSiteGrids[0];

    PLuint meshSize = _defectSiteGrids[0].triangles().size();     
    PLuint gridSize = _defectSiteGrids[0].size();
    
    // buffer grid points 
    glGenTextures(1, &_gridPointsTextureID);
    glBindTexture(GL_TEXTURE_1D, _gridPointsTextureID);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, gridSize, 0, GL_RGBA, GL_FLOAT, &grid.points(0));

    // buffer grid normals
    glGenTextures(1, &_gridNormalsTextureID);
    glBindTexture(GL_TEXTURE_1D, _gridNormalsTextureID);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, gridSize, 0, GL_RGBA, GL_FLOAT, &grid.normals(0));

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

    // fill graft cap texture with -1's          
    plSeq<PLfloat> graftCaps(-1, gridSize*PL_MAX_GRAFT_CAP_TRIANGLES*4*4);   
        
    glGenTextures(1, &_potentialGraftCapsID);                              
    glBindTexture(GL_TEXTURE_2D, _potentialGraftCapsID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, PL_MAX_GRAFT_CAP_TRIANGLES*4, gridSize, 0, GL_RGBA, GL_FLOAT, &graftCaps[0] );

    // fill graft area texture with -1's
    plSeq<PLfloat> graftAreas(-1, gridSize);   
       
    glGenTextures(1, &_potentialGraftAreasID);                              
    glBindTexture(GL_TEXTURE_1D, _potentialGraftAreasID);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, gridSize, 0, GL_R, GL_FLOAT, &graftAreas[0]);

    // fill state energies texture with -1's   
    glGenTextures(1, &_stateEnergiesTextureID);                              
    glBindTexture(GL_TEXTURE_1D, _stateEnergiesTextureID);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, gridSize, 0, GL_R, GL_FLOAT, &graftAreas[0]);

    // fill state cap indices
    plSeq<PLint> stateCaps(-1, gridSize*gridSize);  

    glGenTextures(1, &_stateCapIndicesTextureID);                              
    glBindTexture(GL_TEXTURE_2D, _stateCapIndicesTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, gridSize, gridSize, 0, GL_R, GL_UNSIGNED_INT, &stateCaps[0]);

    glBindTexture(GL_TEXTURE_1D, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

