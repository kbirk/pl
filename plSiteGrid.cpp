#include "plSiteGrid.h"

plSiteGrid::plSiteGrid()
{
}

plSiteGrid::plSiteGrid( const plSeq<plTriangle> &triangles, const plBoundary &boundary )
{
    // generate interior _triangles
    plMeshCutter::findInteriorMesh( triangles, boundary, _triangles );
      
    _generateGridPoints();  
    _calcArea();
    _calcNormal();   
}


void plSiteGrid::_generateGridPoints()
{
    const PLfloat PLUG_RADIUS  = 2.0f;
    const PLfloat GRID_SPACING = 8.0f;
    
    // randomly select points in each triangle to achieve approx spacing    
    
    for (PLuint i=0; i < _triangles.size(); i++)
    {
        //PLfloat area = _triangles[i].getArea();

        plVector3 e01 = _triangles[i].point1() - _triangles[i].point0();
        plVector3 e12 = _triangles[i].point2() - _triangles[i].point1();
        plVector3 e20 = _triangles[i].point0() - _triangles[i].point2();
        
        PLfloat eLength01 = e01.squaredLength();
        PLfloat eLength12 = e12.squaredLength();
        PLfloat eLength20 = e20.squaredLength();
        
        PLfloat longest = PL_MAX_OF_3( eLength01, eLength12, eLength20 );
        
        plVector3 u, v, origin;
        PLfloat uMax, vMax;
        
        if (longest == eLength01)
        {
            plVector3 tangent = ( e01 ^ _triangles[i].normal() ).normalize(); 
            origin = _triangles[i].point0();
            v = e01.normalize();
            vMax = e01.length();
            u = tangent.normalize();
            uMax = e20 * tangent;    
        }                
        else if (longest == eLength12)
        {            
            plVector3 tangent = ( e12 ^ _triangles[i].normal() ).normalize();   
            origin = _triangles[i].point1();          
            v = e12.normalize();
            vMax = e12.length();
            u = tangent.normalize();
            uMax = e01 * tangent;  
        }                
        else if (longest == eLength20) 
        {    
            plVector3 tangent = ( e20 ^ _triangles[i].normal() ).normalize(); 
            origin = _triangles[i].point2();
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
                plVector3 bCoord = _triangles[i].barycentricCoords( p );

                if ( bCoord.x < -0.001 || bCoord.y < -0.001 || bCoord.z < -0.001 )
                    break;  // outside of triangle edge, go to next row
                
                _points.add ( plVector4(p, 1) );
                _normals.add( plVector4(_triangles[i].normal(), 1) ); 
                
            }   
        }
        
    }
}


void plSiteGrid::_calcArea()
{
    _area = 0;
    for (PLuint i=0; i< _triangles.size(); i++)
    {
        _area += _triangles[i].getArea();
    }    
}        


void plSiteGrid::_calcNormal()
{
    _normal = plVector3(0, 0, 0);
    for (PLuint i=0; i< _triangles.size(); i++)
    {
        _normal = _normal + _triangles[i].normal();
    }  
    _normal = (1/(float)_triangles.size() * _normal).normalize();    
}               
    

