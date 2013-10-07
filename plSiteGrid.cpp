#include "plSiteGrid.h"

plSiteGrid::plSiteGrid()
{
}

plSiteGrid::plSiteGrid( const plSeq<plTriangle> &triangles, const plBoundary &boundary, PLbool fineGrain )
{
    // generate interior triangles
    plMeshCutter::findInteriorMesh( _triangles, triangles, boundary );

    if ( fineGrain )
    {
        _generateFineGridPoints(); 
    }
    else
    {
        _generateCoarseGridPoints(); 
    }
    
    _generatePerimeterPoints( boundary ); 
    _calcArea();
    _calcNormal();          
}


plVector3 plSiteGrid::_calcSmoothNormal( const plVector3 &point, const plVector3 &up, PLfloat radius )
{

    plVector3 normal(0,0,0);        
    float     radiusSquared = radius * radius;
    PLint     count = 0;
    
    // Find polygons on top of graft
    for (PLuint i=0; i<_triangles.size(); i++) 
    {
        if ( _triangles[i].normal() * up > 0.001 )
        {        
            PLfloat dist1 = ( _triangles[i].point0() - point ).squaredLength();
            PLfloat dist2 = ( _triangles[i].point1() - point ).squaredLength();
            PLfloat dist3 = ( _triangles[i].point2() - point ).squaredLength();
           
            // if any point of triangle is withing radial sphere, accept
            float minDist = PL_MIN_OF_3( dist1, dist2, dist3 );

            if (minDist <= radiusSquared)
            {        
                normal = normal + _triangles[i].normal();
                count++;
            }
        }
    } 

    if (count == 0)
    {
        // no triangles in radial sphere, just assume previous normal, (this can be bad.....)
        std::cout << "No normal found\n";
        return up;
    }    

    return ( (1.0f/(PLfloat)count) * normal ).normalize();      
}


PLuint plSiteGrid::getFullSSBO() const
{
    plSeq<plVector4> data( dataSize() );  
    for ( PLuint i=0; i < gridSize(); i++ ) { data.add( _points[i]  ); }    
    for ( PLuint i=0; i < gridSize(); i++ ) { data.add( _normals[i] ); }    
    for ( PLuint i=0; i < meshSize(); i++ ) { data.add( plVector4( _triangles[i].point0(), 1.0 ) ); 
                                            data.add( plVector4( _triangles[i].point1(), 1.0 ) );
                                            data.add( plVector4( _triangles[i].point2(), 1.0 ) );
                                            data.add( plVector4( _triangles[i].normal(), 1.0 ) ); }                                                                                      
    for ( PLuint i=0; i < perimSize()*2; i++) { data.add( _perimeter[i] ); }    

    PLuint tempBuffer;
    glGenBuffers(1, &tempBuffer);   
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, tempBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize()*sizeof(plVector4), &data[0], GL_STATIC_READ);
    return tempBuffer;
}


PLuint plSiteGrid::getGridSSBO() const
{
    PLuint gridSize  = _points.size();     
    PLuint dataSize  = gridSize*2;
      
    plSeq<plVector4> data( dataSize );  
    for ( PLuint i=0; i < gridSize; i++ ) { data.add( _points[i]  ); }    
    for ( PLuint i=0; i < gridSize; i++ ) { data.add( _normals[i] ); }                                                                                     
  
    PLuint tempBuffer;
    glGenBuffers(1, &tempBuffer);   
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, tempBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize*sizeof(plVector4), &data[0], GL_STATIC_READ);
    return tempBuffer;
}


PLuint plSiteGrid::getMeshSSBO() const
{
    PLuint meshSize  = _triangles.size();     
    PLuint dataSize  = meshSize*4;
      
    std::cout << "Size: " << dataSize*sizeof(plVector4) << std::endl;   
      
    plSeq<plVector4> data( dataSize );     
    for ( PLuint i=0; i < meshSize; i++ ) { data.add( plVector4( _triangles[i].point0(), 1.0 ) ); 
                                            data.add( plVector4( _triangles[i].point1(), 1.0 ) );
                                            data.add( plVector4( _triangles[i].point2(), 1.0 ) );
                                            data.add( plVector4( _triangles[i].normal(), 1.0 ) ); }                                                                                      
  
    PLuint tempBuffer;
    glGenBuffers(1, &tempBuffer);   
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, tempBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize*sizeof(plVector4), &data[0], GL_STATIC_READ);
    return tempBuffer;
}


void plSiteGrid::_generateCoarseGridPoints()
{   
    plSet<plPointAndNormal> p;
    for (PLuint i=0; i < _triangles.size(); i++)
    {   
        plVector3 smoothNormal0 = _calcSmoothNormal( _triangles[i].point0(), _triangles[i].normal(), PL_NORMAL_SMOOTHING_RADIUS );   
        plVector3 smoothNormal1 = _calcSmoothNormal( _triangles[i].point1(), _triangles[i].normal(), PL_NORMAL_SMOOTHING_RADIUS );  
        plVector3 smoothNormal2 = _calcSmoothNormal( _triangles[i].point2(), _triangles[i].normal(), PL_NORMAL_SMOOTHING_RADIUS );  
        p.insert( plPointAndNormal( _triangles[i].point0(), smoothNormal0 ) );
        p.insert( plPointAndNormal( _triangles[i].point1(), smoothNormal1 ) );
        p.insert( plPointAndNormal( _triangles[i].point2(), smoothNormal2 ) );
    }
    
    plSet<plPointAndNormal>::iterator p_itr = p.begin();
    plSet<plPointAndNormal>::iterator p_end = p.end();
    
    for ( ; p_itr != p_end; ++p_itr )
    {
        _points.add ( plVector4( p_itr->point,  1) );
        _normals.add( plVector4( p_itr->normal, 1) );
    }
}    


void plSiteGrid::_generateFineGridPoints()
{
    const PLfloat GRID_SPACING = 0.6f;
    
    plSet<plPointAndNormal> p;
    
    // select points in each triangle at approx spacing    
    for (PLuint i=0; i < _triangles.size(); i++)
    {
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
                plVector3 point = origin + k*-u + j*v;
                plVector3 bCoord = _triangles[i].barycentricCoords( point );

                if ( bCoord.x < -0.001 || bCoord.y < -0.001 || bCoord.z < -0.001 )
                {
                    break;  // outside of triangle edge, go to next row
                }
                plVector3 smoothNormal = _calcSmoothNormal( point, _triangles[i].normal(), 4.0f );  
                p.insert( plPointAndNormal( point, smoothNormal ) );                
            }   
        }
        
    }
    
    plSet<plPointAndNormal>::iterator p_itr = p.begin();
    plSet<plPointAndNormal>::iterator p_end = p.end();
    
    for ( ; p_itr != p_end; ++p_itr )
    {
        _points.add ( plVector4( p_itr->point,  1) );
        _normals.add( plVector4( p_itr->normal, 1) );
    }
    
}

void plSiteGrid::_generatePerimeterPoints( const plBoundary &boundary )
{
    PLuint size = boundary.size();
    
    for (PLuint i=0; i < size; i++)
    {
        _perimeter.add( plVector4( boundary.points(i), 1) );
    }

    for (PLuint i=0; i < size; i++)
    {
        plVector3 d = boundary.points((i+1)%size) - boundary.points(i);
        plVector3 n = 0.5f * (boundary.normals((i+1)%size) + boundary.normals(i));        
        _perimeter.add( plVector4( (d ^ n).normalize(), 1.0f ) );
    }
}

void plSiteGrid::_calcArea()
{
    _area = 0;
    for (PLuint i=0; i< _triangles.size(); i++)
    {
        _area += _triangles[i].getArea();
    }    
    std::cout << "\t\tSite area: " << _area << "\n";
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


PLuint getGroupFullSSBO( const plSeq<plSiteGrid> &sites )
{
    // find total data size
    PLuint dataSize  = 0;
    for (PLuint i=0; i < sites.size(); i++ )
    {
        dataSize += sites[i].dataSize();
    }

    std::cout << "Size: " << dataSize*sizeof(plVector4) << std::endl;   
     

    plSeq<plVector4> data( dataSize );  
    
    for (PLuint i=0; i < sites.size(); i++ )
    {
        for ( PLuint j=0; j < sites[i].gridSize(); j++ )   { data.add( sites[i].points(j) );  }    
        for ( PLuint j=0; j < sites[i].gridSize(); j++ )   { data.add( sites[i].normals(j) ); }    
        for ( PLuint j=0; j < sites[i].meshSize(); j++ )   { data.add( plVector4( sites[i].triangles(j).point0(), 1.0 ) ); 
                                                             data.add( plVector4( sites[i].triangles(j).point1(), 1.0 ) );
                                                             data.add( plVector4( sites[i].triangles(j).point2(), 1.0 ) );
                                                             data.add( plVector4( sites[i].triangles(j).normal(), 1.0 ) ); }                                                                                      
        for ( PLuint j=0; j < sites[i].perimSize()*2; j++) { data.add( sites[i].perimeter(j) ); }    
    }
    
    PLuint tempBuffer;
    glGenBuffers(1, &tempBuffer);   
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, tempBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize*sizeof(plVector4), &data[0], GL_STATIC_READ);
    return tempBuffer;
}


PLuint getGroupGridAndMeshSSBO( const plSeq<plSiteGrid> &sites )
{
    // find total data size
    PLuint dataSize  = 0;
    for (PLuint i=0; i < sites.size(); i++ )
    {
        dataSize += sites[i].gridSize()*2;
        dataSize += sites[i].meshSize()*4;
    }

    plSeq<plVector4> data( dataSize );  
    
    for (PLuint i=0; i < sites.size(); i++ )
    {
        for ( PLuint j=0; j < sites[i].gridSize(); j++ )   { data.add( sites[i].points(j) );  }    
        for ( PLuint j=0; j < sites[i].gridSize(); j++ )   { data.add( sites[i].normals(j) ); }    
        for ( PLuint j=0; j < sites[i].meshSize(); j++ )   { data.add( plVector4( sites[i].triangles(j).point0(), 1.0 ) ); 
                                                             data.add( plVector4( sites[i].triangles(j).point1(), 1.0 ) );
                                                             data.add( plVector4( sites[i].triangles(j).point2(), 1.0 ) );
                                                             data.add( plVector4( sites[i].triangles(j).normal(), 1.0 ) ); }                                                                                         
    }
    
    PLuint tempBuffer;
    glGenBuffers(1, &tempBuffer);   
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, tempBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize*sizeof(plVector4), &data[0], GL_STATIC_READ);
    return tempBuffer;
}


PLuint getGroupGridSSBO( const plSeq<plSiteGrid> &sites )
{
    // find total data size
    PLuint dataSize  = 0;
    for (PLuint i=0; i < sites.size(); i++ )
    {
        dataSize += sites[i].gridSize()*2;
    }
    
    plSeq<plVector4> data( dataSize ); 
    for (PLuint i=0; i < sites.size(); i++ )
    { 
        for ( PLuint j=0; j < sites[i].gridSize(); j++ ) { data.add( sites[i].points(j)  ); }    
        for ( PLuint j=0; j < sites[i].gridSize(); j++ ) { data.add( sites[i].normals(j) ); }                                                                                     
    }
    
    PLuint tempBuffer;
    glGenBuffers(1, &tempBuffer);   
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, tempBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize*sizeof(plVector4), &data[0], GL_STATIC_READ);
    return tempBuffer;
}


