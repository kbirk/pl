#include "plAutomaticPlanner.h"

plSeq<plSiteGrid> plAutomaticPlanner::_donorSiteGrids;
plSeq<plSiteGrid> plAutomaticPlanner::_defectSiteGrids;

void plAutomaticPlanner::calculate( plPlan &plan )
{
    std::cout << "Calculating Plan ... \n";
    /*
    // Draw defect boundary 
    for ( PLuint i = 0; i < _defectSites.size(); i++)
    {
        // calculate interior triangles outlined by boundary
        plSeq<plTriangle> interiorTriangles;                
        plMeshCutter::findInteriorMesh( _defectSites(i).model().triangles(), _defectSites(i).boundary, interiorTriangles );
        // compose grid over interior mesh
        plSiteGrid grid;
        _createGrid( grid, interiorTriangles );
        // store it
        _defectSiteGrids.add( grid ); 
    }
    */
       
    // Draw harvest boundaries   
    for ( PLuint i = 0; i < plan.donorSites().size(); i++)
    {      
        std::cout << "\t Calculating donor site grid " << i << " \n";
        plSeq<plTriangle> interiorTriangles;                
        plMeshCutter::findInteriorMesh( plan.donorSites(i).model()->cartilage.triangles(), plan.donorSites(i).boundary, interiorTriangles );
        
        std::cout << "\t Interior triangle Count: " << interiorTriangles.size() << "\n";
        
        // compose grid over interior mesh
        plSiteGrid grid;
        _createGrid( grid, interiorTriangles );
        // store it       
        _donorSiteGrids.add( grid );
        std::cout << "\t " <<  grid.points.size() << " grid points calculated \n";
    }    


}

void plAutomaticPlanner::_createGrid( plSiteGrid &grid, const plSeq<plTriangle> &triangles )
{
    const PLfloat PLUG_RADIUS  = 2.0f;
    const PLfloat GRID_SPACING = sqrt(3.0f) * PLUG_RADIUS; 
    
    // randomly select points in each triangle to achieve approx spacing    
    grid.points.clear();
    grid.normals.clear();
    
    for (PLuint i=0; i < triangles.size(); i++)
    {
        PLfloat area = triangles[i].getArea();

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
        
        std::cout << "---------------------------------------------------\n";
        std::cout << "uMax: " << uMax << " u: " << u << "\n";
        std::cout << "vMax: " << vMax << " v: " << v << "\n";
        
        std::cout << "grid spacing: " << GRID_SPACING << "\n";
        
        // build grid, row by row, right to left (more efficient this way)
        for (PLfloat j=0; j<vMax; j+= GRID_SPACING)
        {
            for (PLfloat k=uMax; k>0; k-= GRID_SPACING)            
            {
                plVector3
            
                std::cout << "k: " << k << " j: " << j << "\n"; 
            
                std::cout << "p: " << k*u + j*v << "\n";
                std::cout << "p0: " << triangles[i].point0() << " p1: " << triangles[i].point1() << " p1: " << triangles[i].point2() << "\n";
            
                plVector3 bCoord = triangles[i].barycentricCoords( k*u + j*v );
                
                std::cout << bCoord << "\n";
                
                if ( bCoord.x < 0 || bCoord.y < 0 || bCoord.z < 0 )
                    break;  // outside of triangle edge, go to next row
                    
                grid.points.add ( bCoord );    
                grid.normals.add( triangles[i].normal() ); 
            }   
        }
        
        
        
    }
    
    

}
