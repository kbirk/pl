#include "plAutomaticPlanner.h"

plSeq<plSiteGrid> plAutomaticPlanner::_donorSiteGrids;
plSeq<plSiteGrid> plAutomaticPlanner::_defectSiteGrids;

plSeq<plMesh*> plAutomaticPlanner::DEBUG_MESH;

void plAutomaticPlanner::calculate( plPlan &plan )
{
    // generate site grids
    _generateSiteGrids( plan );
    
    
    std::cout << "\nposition: " << _defectSiteGrids[0].points(0) << "\n";
    std::cout << "normal: "      << _defectSiteGrids[0].normals(0) << "\n"; 
    
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
        // calculate interior triangles outlined by boundary
        plSeq<plTriangle> interiorTriangles;                
        plMeshCutter::findInteriorMesh( plan.defectSites(i).spline.triangles(), plan.defectSites(i).boundary, interiorTriangles );
        
        std::cout << "\t\t Interior triangle Count: " << interiorTriangles.size() << "\n";
        
        // compose grid over interior mesh
        plSiteGrid grid;
        _createGrid( grid, interiorTriangles );
        // store it
        _defectSiteGrids.add( grid ); 
        std::cout << "\t\t " <<  grid.size() << " grid points calculated \n";
    }
    

    for ( PLuint i = 0; i < plan.donorSites().size(); i++)
    {      
        std::cout << "\t Calculating donor site grid " << i << " \n";
        plSeq<plTriangle> interiorTriangles;                
        plMeshCutter::findInteriorMesh( plan.donorSites(i).model()->cartilage.triangles(), plan.donorSites(i).boundary, interiorTriangles );
        
        std::cout << "\t\t Interior triangle Count: " << interiorTriangles.size() << "\n";
        
        // compose grid over interior mesh
        plSiteGrid grid;
        _createGrid( grid, interiorTriangles );
        // store it       
        _donorSiteGrids.add( grid );
        std::cout << "\t\t " <<  grid.size() << " grid points calculated \n";
    }    
}
    
    
void plAutomaticPlanner::_dispatchDefectShader( plPlan &plan )
{
    // compile / link compute shader
    plBuildDefectShader computeShader("./shaders/test.comp");
    
    // buffer data
    computeShader.bufferGridTextures( _defectSiteGrids[0] );
    computeShader.bufferSplineTexture( plan.defectSites(0).spline ); 
       
    PLtime t0 = plTimer::now();
    
    // DEBUG
    
    for (PLuint i=0; i<_defectSiteGrids[0].size(); i++)
    {
        
        DEBUG_MESH.add( computeShader.dispatch(i) );    
    
        //PLtime t = plTimer::now();

        /*
        while(true)
        {
            if ( (plTimer::now() - t) > 1000 )
            {
                break;
            }
            usleep(2000);
        }
        */
    }
    
        
    
    //DEBUG_MESH = computeShader.dispatch();
    
    PLtime t1 = plTimer::now();
    
    std::cout << "Compute shader execution time: " << t1 - t0 << " milliseconds \n";
}
     

void plAutomaticPlanner::_createGrid( plSiteGrid &grid, const plSeq<plTriangle> &triangles )
{
    const PLfloat PLUG_RADIUS  = 2.0f;
    const PLfloat GRID_SPACING = 8.0f; //sqrt(3.0f) * PLUG_RADIUS; 
    
    // randomly select points in each triangle to achieve approx spacing    
    grid.clear();
    
    for (PLuint i=0; i < triangles.size(); i++)
    {
        //PLfloat area = triangles[i].getArea();

        plVector3 e01 = triangles[i].point1() - triangles[i].point0();
        plVector3 e12 = triangles[i].point2() - triangles[i].point1();
        plVector3 e20 = triangles[i].point0() - triangles[i].point2();
        
        PLfloat eLength01 = e01.squaredLength();
        PLfloat eLength12 = e12.squaredLength();
        PLfloat eLength20 = e20.squaredLength();
        
        PLfloat longest = PL_MAX_OF_3( eLength01, eLength12, eLength20 );
        
        plVector3 u, v, origin;
        PLfloat uMax, vMax;
        
        if (longest == eLength01)
        {
            plVector3 tangent = ( e01 ^ triangles[i].normal() ).normalize(); 
            origin = triangles[i].point0();
            v = e01.normalize();
            vMax = e01.length();
            u = tangent.normalize();
            uMax = e20 * tangent;    
        }                
        else if (longest == eLength12)
        {            
            plVector3 tangent = ( e12 ^ triangles[i].normal() ).normalize();   
            origin = triangles[i].point1();          
            v = e12.normalize();
            vMax = e12.length();
            u = tangent.normalize();
            uMax = e01 * tangent;  
        }                
        else if (longest == eLength20) 
        {    
            plVector3 tangent = ( e20 ^ triangles[i].normal() ).normalize(); 
            origin = triangles[i].point2();
            v = e20.normalize();
            vMax = e20.length();
            u = tangent.normalize();
            uMax = e12 * tangent;              
        }
        
        // build grid, row by row, right to left (more efficient this way)
        for (PLfloat j=0; j<vMax; j+= GRID_SPACING)
        {
            for (PLfloat k=0; k<uMax; k+= GRID_SPACING)            
            {
                plVector3 p = origin + k*-u + j*v;
                plVector3 bCoord = triangles[i].barycentricCoords( p );

                if ( bCoord.x < -0.001 || bCoord.y < -0.001 || bCoord.z < -0.001 )
                    break;  // outside of triangle edge, go to next row
                
                    grid.addPointAndNormal( plVector4(p,1), plVector4(triangles[i].normal(),1) ); 
                
            }   
        }
        
    }
    
    

}
