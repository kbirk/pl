#include "plPlanningSite.h"

plPlanningSite::plPlanningSite()
{
}


plPlanningSite::plPlanningSite( const std::vector<plTriangle> &tris, const plBoundary &boundary, PLbool fineGrain )
{
    // generate interior triangles
    plMeshCutter::findInteriorMesh( triangles, tris, boundary );

    if ( fineGrain )
    {
        _generateFineGridPoints(); 
    }
    else
    {
        _generateCoarseGridPoints(); 
    }
    
    _generateBoundaryPoints( boundary ); 
    _calcArea();
    _calcNormal();   
       
    std::cout << "\t\t" <<  triangles.size()  << " triangles calculated \n";
    std::cout << "\t\t" <<  gridPoints.size() << " grid points calculated \n";       
}


plPlanningSite::plPlanningSite( plPlanningSite&& site )
    : triangles      ( std::move( site.triangles ) ),
      gridPoints     ( std::move( site.gridPoints ) ), 
      gridNormals    ( std::move( site.gridNormals ) ),
      boundaryPoints ( std::move( site.boundaryPoints ) ),
      boundaryNormals( std::move( site.boundaryNormals ) ),
      area           ( site.area ),
      avgNormal      ( site.avgNormal )
{
}


plPlanningSite& plPlanningSite::operator= ( plPlanningSite&& site )
{
    triangles       = std::move( site.triangles );
    gridPoints      = std::move( site.gridPoints );
    gridNormals     = std::move( site.gridNormals );
    boundaryPoints  = std::move( site.boundaryPoints );
    boundaryNormals = std::move( site.boundaryNormals );
    area            = site.area;
    avgNormal       = site.avgNormal;
    return *this;
}


PLbool plPlanningSite::good() const
{
    return !( gridPoints.size() == 0  ||
              gridNormals.size() == 0 ||
              triangles.size() == 0   ||
              boundaryPoints.size() == 0 ||
              boundaryNormals.size() == 0 ||
              area == 0.0f ||
              avgNormal.length() == 0.0f );
}


plVector3 plPlanningSite::getSmoothNormal( const plVector3 &point, const plVector3 &up, PLfloat radius ) const
{
    plVector3 normal(0,0,0);        
    float radiusSquared = radius * radius;
    PLint count = 0;
    
    // Find polygons on top of graft
    for (PLuint i=0; i<triangles.size(); i++) 
    {
        if ( triangles[i].normal() * up > 0.001 )
        {        
            PLfloat dist1 = ( triangles[i].point0() - point ).squaredLength();
            PLfloat dist2 = ( triangles[i].point1() - point ).squaredLength();
            PLfloat dist3 = ( triangles[i].point2() - point ).squaredLength();
           
            // if any point of triangle is withing radial sphere, accept
            float minDist = PL_MIN_OF_3( dist1, dist2, dist3 );

            if (minDist <= radiusSquared)
            {        
                normal = normal + triangles[i].normal();
                count++;
            }
        }
    } 

    if (count == 0)
    {
        // no triangles in radial sphere, just assume previous normal, (this can be bad.....)
        std::cout << "plPlanningSite::getSmoothNormal() warning: No normal found\n";
        return up;
    }    

    return ( ( 1.0f/(PLfloat)count ) * normal ).normalize();      
}


void plPlanningSite::_bufferGridData( std::vector<plVector4> &data ) const
{
    for ( PLuint i=0; i < gridPoints.size(); i++ )  
        data.push_back( gridPoints[i] ); 
         
    for ( PLuint i=0; i < gridNormals.size(); i++ ) 
        data.push_back( gridNormals[i] );  
}


void plPlanningSite::_bufferMeshData( std::vector<plVector4> &data ) const
{
    for ( PLuint i=0; i < triangles.size(); i++ )   
    { 
        data.push_back( plVector4( triangles[i].point0(), 1.0 ) ); 
        data.push_back( plVector4( triangles[i].point1(), 1.0 ) );
        data.push_back( plVector4( triangles[i].point2(), 1.0 ) );
        data.push_back( plVector4( triangles[i].normal(), 1.0 ) ); 
    }   
}


void plPlanningSite::_bufferBoundaryData( std::vector<plVector4> &data ) const
{
    for ( PLuint i=0; i < boundaryPoints.size(); i++ )  
        data.push_back( boundaryPoints[i] ); 
         
    for ( PLuint i=0; i < boundaryNormals.size(); i++ ) 
        data.push_back( boundaryNormals[i] );  
}


plSSBO plPlanningSite::getSSBO() const
{
    std::vector<plVector4> data;    data.reserve( totalSize() );  
      
    _bufferGridData( data );
    _bufferMeshData( data );
    _bufferBoundaryData( data );    

    PLuint numBytes = totalSize() * sizeof( plVector4 );

    std::cout << "\t\tTotal buffer size: " << numBytes << " bytes " << std::endl;     

    return plSSBO( numBytes, (void*)(&data[0]) );
}


void plPlanningSite::_generateCoarseGridPoints()
{   
    plSet<plPointAndNormal> p;
    for (PLuint i=0; i < triangles.size(); i++)
    {   
        plVector3 smoothNormal0 = getSmoothNormal( triangles[i].point0(), triangles[i].normal(), PL_NORMAL_SMOOTHING_RADIUS );   
        plVector3 smoothNormal1 = getSmoothNormal( triangles[i].point1(), triangles[i].normal(), PL_NORMAL_SMOOTHING_RADIUS );  
        plVector3 smoothNormal2 = getSmoothNormal( triangles[i].point2(), triangles[i].normal(), PL_NORMAL_SMOOTHING_RADIUS );  
        p.insert( plPointAndNormal( triangles[i].point0(), smoothNormal0 ) );
        p.insert( plPointAndNormal( triangles[i].point1(), smoothNormal1 ) );
        p.insert( plPointAndNormal( triangles[i].point2(), smoothNormal2 ) );
    }
    
    plSet<plPointAndNormal>::iterator p_itr = p.begin();
    plSet<plPointAndNormal>::iterator p_end = p.end();
    
    for ( ; p_itr != p_end; ++p_itr )
    {
        gridPoints.push_back( plVector4( p_itr->point,  1) );
        gridNormals.push_back( plVector4( p_itr->normal, 1) );
    }
}    


void plPlanningSite::_generateFineGridPoints()
{
    const PLfloat GRID_SPACING = 0.6f;
    
    plSet<plPointAndNormal> p;
    
    // select points in each triangle at approx spacing    
    for (PLuint i=0; i < triangles.size(); i++)
    {
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
                plVector3 point = origin + k*-u + j*v;
                plVector3 bCoord = triangles[i].barycentricCoords( point );

                if ( bCoord.x < -0.001 || bCoord.y < -0.001 || bCoord.z < -0.001 )
                {
                    break;  // outside of triangle edge, go to next row
                }
                plVector3 smoothNormal = getSmoothNormal( point, triangles[i].normal(), 4.0f );  
                p.insert( plPointAndNormal( point, smoothNormal ) );                
            }   
        }
        
    }
    
    plSet<plPointAndNormal>::iterator p_itr = p.begin();
    plSet<plPointAndNormal>::iterator p_end = p.end();
    
    for ( ; p_itr != p_end; ++p_itr )
    {
        gridPoints.push_back( plVector4( p_itr->point,  1) );
        gridNormals.push_back( plVector4( p_itr->normal, 1) );
    }
    
}


void plPlanningSite::_generateBoundaryPoints( const plBoundary &boundary )
{
    PLuint size = boundary.size();
    
    for (PLuint i=0; i < size; i++)
    {
        boundaryPoints.push_back( plVector4( boundary.points(i), 1) );
    }

    for (PLuint i=0; i < size; i++)
    {
        plVector3 d = boundary.points((i+1)%size) - boundary.points(i);
        plVector3 n = 0.5f * (boundary.normals((i+1)%size) + boundary.normals(i));        
        boundaryNormals.push_back( plVector4( (d ^ n).normalize(), 1.0f ) );
    }
}


void plPlanningSite::_calcArea()
{
    area = 0;
    for (PLuint i=0; i< triangles.size(); i++)
    {
        area += triangles[i].getArea();
    }    
    std::cout << "\t\tSite area: " << area << "\n";
}        


void plPlanningSite::_calcNormal()
{
    avgNormal = plVector3(0, 0, 0);
    for (PLuint i=0; i< triangles.size(); i++)
    {
        avgNormal = avgNormal + triangles[i].normal();
    }  
    avgNormal = (1/(float)triangles.size() * avgNormal).normalize();    
}  

