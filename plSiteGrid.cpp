#include "plSiteGrid.h"

plSiteGrid::plSiteGrid()
{
}

plSiteGrid::plSiteGrid( const plSeq<plTriangle> &triangles, const plBoundary &boundary )
{
    // generate interior _triangles
    plMeshCutter::findInteriorMesh( triangles, boundary, _triangles );
      
    _generateGridPoints(); 
    _generatePerimeterPoints( boundary ); 
    _calcArea();
    _calcNormal();   
    
    
}

PLuint plSiteGrid::getFullSSBO() const
{
    PLuint meshSize  = _triangles.size();     
    PLuint gridSize  = _points.size();
    PLuint perimSize = _perimeter.size();
    PLuint dataSize  = (gridSize*2)+(meshSize*4)+perimSize;
      
    plSeq<plVector4> data( dataSize );  
    for ( PLuint i=0; i < gridSize; i++ ) { data.add( _points[i]  ); }    
    for ( PLuint i=0; i < gridSize; i++ ) { data.add( _normals[i] ); }    
    for ( PLuint i=0; i < meshSize; i++ ) { data.add( plVector4( _triangles[i].point0(), 1.0 ) ); 
                                            data.add( plVector4( _triangles[i].point1(), 1.0 ) );
                                            data.add( plVector4( _triangles[i].point2(), 1.0 ) );
                                            data.add( plVector4( _triangles[i].normal(), 1.0 ) ); }                                                                                      
    for ( PLuint i=0; i < perimSize; i++) { data.add( _perimeter[i] ); }    

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


void plSiteGrid::_generateGridPoints()
{
    
    plSet<plPointAndNormal> p;
    for (PLuint i=0; i < _triangles.size(); i++)
    {
        p.insert( plPointAndNormal( _triangles[i].point0(), _triangles[i].normal() ) );
        p.insert( plPointAndNormal( _triangles[i].point0(), _triangles[i].normal() ) );
        p.insert( plPointAndNormal( _triangles[i].point0(), _triangles[i].normal() ) );
    }
    
    plSet<plPointAndNormal>::iterator p_itr = p.begin();
    plSet<plPointAndNormal>::iterator p_end = p.end();
    
    for ( ; p_itr != p_end; ++p_itr )
    {
        _points.add ( plVector4( p_itr->point,  1) );
        _normals.add( plVector4( p_itr->normal, 1) );
    }
    
    /*

    const PLfloat PLUG_RADIUS  = 2.0f;
    const PLfloat GRID_SPACING = 8.0f;
    
    // randomly select points in each triangle to achieve approx spacing    
    
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
                plVector3 p = origin + k*-u + j*v;
                plVector3 bCoord = _triangles[i].barycentricCoords( p );

                if ( bCoord.x < -0.001 || bCoord.y < -0.001 || bCoord.z < -0.001 )
                    break;  // outside of triangle edge, go to next row
                
                _points.add ( plVector4(p, 1) );
                _normals.add( plVector4(_triangles[i].normal(), 1) ); 
                
            }   
        }
        
    }
    
    */
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
    

