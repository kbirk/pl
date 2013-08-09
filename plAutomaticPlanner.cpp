#include "plAutomaticPlanner.h"

plSeq<plSiteGrid> plAutomaticPlanner::_donorSiteGrids;
plSeq<plSiteGrid> plAutomaticPlanner::_defectSiteGrids;

plSeq<plMesh> plAutomaticPlanner::DEBUG_MESH;

void plAutomaticPlanner::calculate( plPlan &plan )
{
    // generate site grids
    _generateSiteGrids( plan );
        
    std::cout << "\nposition: " << _defectSiteGrids[0].points(0)  << "\n";
    std::cout << "normal: "     << _defectSiteGrids[0].normals(0) << "\n"; 
    
    //  
    _dispatchDefectShader( plan );

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


void plAutomaticPlanner::_dispatchDefectShader( plPlan &plan )
{
    // compile / link compute shader
    plPlannerStage0Shader computeShader("./shaders/plannerStage0.comp");
    
    // buffer data
    computeShader.bufferGridTextures( _defectSiteGrids[0] );
       
    PLtime t0 = plTimer::now();
    
    // DEBUG
    
    PLfloat *pixels = computeShader.dispatch();
    
    //return;

    int stride = PL_MAX_GRAFT_CAP_TRIANGLES * (4*4);

    for (int i=0; i<_defectSiteGrids[0].size(); i++) 
    {
        plSeq<plVector3> interleaved_vertices;
        plSeq<PLuint>    indices;

        for (int j=0; j < PL_MAX_GRAFT_CAP_TRIANGLES * (4*4); j+=(4*4) ) 
        {
        
            if ( pixels[i * stride + j] < 0)
            {
                std::cout << "End at: " << j/16 << "\n";
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
    
    PLtime t1 = plTimer::now();
    
    std::cout << "Compute shader execution time: " << t1 - t0 << " milliseconds \n";
}

