#include "plSpline.h" 


plSpline::plSpline() 
    : plBoundary()
{
}

plSpline::plSpline( const plBoneAndCartilage &model )
    : plBoundary(model)
{
}

void plSpline::importCSV( const plSeq<plString> &row, const plBoneAndCartilage &model )
{
    plBoundary::importCSV( row, model );
    // construct spline 
    if (size() == 4)
    {
        _computeHermite();
    }
}

PLuint plSpline::addPointAndNormal (const plVector3 &point, const plVector3 &normal)
{
    if (size() < 4)
    {
        PLint ret = plBoundary::addPointAndNormal(point, normal);
        
        if (size() == 4)
        {
            _computeHermite();
        }
        return ret;
    }
    return -1;
}

void plSpline::movePointAndNormal( PLuint index, const plVector3 &point, const plVector3 &normal)
{
    plBoundary::movePointAndNormal( index, point, normal );
    
    if (size() == 4)
    {
        _computeHermite();
    }

}

void plSpline::removePointAndNormal( PLuint index )
{
    plBoundary::removePointAndNormal(index);   
}
        
void plSpline::draw() const
{      
    if (!isVisible)
        return;
   
    plPicking::value.type = PL_PICKING_TYPE_DEFECT_CORNERS;
    plBoundary::draw();
    
    if (size() == 4)
    {
        plPicking::value.type = PL_PICKING_TYPE_DEFECT_SPLINE;       
        plColourStack::load(PL_DEFECT_SPLINE_COLOUR);  
        _surfaceMesh.draw();
    }
    
    if (_isSelected)
    {
        //_drawSplineSelectionInterface();
    }            
}

PLfloat Q( PLfloat s, PLfloat t, const plSeq<PLfloat> &st, const plSeq<PLfloat> &tt)
{
    // hermite blending matrix
    static plMatrix44 h( 2, -3,  0,  1,
                        -2,  3,  0,  0,
                         1, -2,  1,  0,
                         1, -1,  0,  0 );
                          
    plMatrix44  q ( 0.0f,   0.0f, tt[0],  tt[3],
                    0.0f,   0.0f, tt[1],  tt[2],
                    st[0], st[3],  0.0f,   0.0f,
                    st[1], st[2],  0.0f,   0.0f );    
                                     
    plVector4 sc( s*s*s, s*s, s, 1);
    plVector4 tc( t*t*t, t*t, t, 1);  
    
    return (h * sc) * q * (h * tc);                         
}


plSeq<plVector3> plSpline::_averageCornerNormals() const 
{
    const PLfloat AVERAGE_RADIUS = 2.0f;

    plSeq<plVector3> n(4);

    // compute averages normals
    n.add( _model->cartilage.getAverageNormal( AVERAGE_RADIUS, _points[0], _normals[0]) ); 
    n.add( _model->cartilage.getAverageNormal( AVERAGE_RADIUS, _points[1], _normals[1]) ); 
    n.add( _model->cartilage.getAverageNormal( AVERAGE_RADIUS, _points[2], _normals[2]) ); 
    n.add( _model->cartilage.getAverageNormal( AVERAGE_RADIUS, _points[3], _normals[3]) ); 
    
    return n;
}

void plSpline::_computeTangents( plSeq<PLfloat> &st, plSeq<PLfloat> &tt, const plSeq<plVector3> &p, const plSeq<plVector3> &n ) const
{
    // get unit directional vectors, (ex. p01 = from p0 to p1)
    plVector3 p01 = ( p[1]-p[0] ).normalize();
    plVector3 p03 = ( p[3]-p[0] ).normalize();
    plVector3 p12 = ( p[2]-p[1] ).normalize();
    plVector3 p32 = ( p[2]-p[3] ).normalize();
    
    // get spline wall unit normals 
    plVector3 n01 = ( 0.5f*(n[0] + n[1]) ).normalize();
    plVector3 n03 = ( 0.5f*(n[0] + n[3]) ).normalize();
    plVector3 n12 = ( 0.5f*(n[1] + n[2]) ).normalize();
    plVector3 n32 = ( 0.5f*(n[3] + n[2]) ).normalize();

    // s and t vectors at each point
    plSeq<plVector3> s(4), t(4); 

    // normals of planes between each point   
    plVector3 sn, tn;
    
    // p0 plane normals
    sn = ( p01 ^ -n01 ).normalize();
    tn = ( p03 ^ n03 ).normalize();
    
    s.add( plMath::projectVectorOnPlane( p03 ^ n[0], sn).normalize() );
    t.add( plMath::projectVectorOnPlane( p01 ^ -n[0], tn).normalize() ) ; 

    // p1 plane normals
    //sn = ( p01 ^ -n01 ).normalize(); // redundant
    tn = ( p12 ^ n12 ).normalize();
    
    s.add( plMath::projectVectorOnPlane( p03 ^ n[1], sn).normalize() );
    t.add( plMath::projectVectorOnPlane( p01 ^ -n[1], tn).normalize() ) ;
    
    // p2 plane normals
    sn = ( p32 ^ -n32 ).normalize();
    //tn = ( p12 ^ n12 ).normalize();  // redundant
    
    s.add( plMath::projectVectorOnPlane( p12 ^ n[2], sn).normalize() );
    t.add( plMath::projectVectorOnPlane( p32 ^ -n[2], tn).normalize() ) ;
    
    // p3 plane normals
    //sn = ( p32 ^ -n32 ).normalize(); // redundant
    tn = ( p03 ^ n03 ).normalize();
    
    s.add( plMath::projectVectorOnPlane( p03 ^ n[3], sn).normalize() );
    t.add( plMath::projectVectorOnPlane( p32 ^ -n[3], tn).normalize() ) ;
   
    // find the slope of the line along the plane of the spline boundary wall
    // scale by the length between two points to ensure proper scaling
    st.add( (s[0]*n01) / (s[0]*p01) * (p[0]-p[1]).length() ); 
    tt.add( (t[0]*n03) / (t[0]*p03) * (p[0]-p[3]).length() );     

    st.add( (s[1]*n01) / (s[1]*p01) * (p[0]-p[1]).length() ); 
    tt.add( (t[1]*n12) / (t[1]*p12) * (p[2]-p[1]).length() ); 

    st.add( (s[2]*n32) / (s[2]*p32) * (p[3]-p[2]).length() ); 
    tt.add( (t[2]*n12) / (t[2]*p12) * (p[2]-p[1]).length() ); 

    st.add( (s[3]*n32) / (s[3]*p32) * (p[3]-p[2]).length() ); 
    tt.add( (t[3]*n03) / (t[3]*p03) * (p[0]-p[3]).length() ); 

}


void plSpline::_computeHermite()
{    
    // spline colour map computations are very intensive, to prevent unnecessary updates due to frequency of mouse events, add a limit
    if (_timeSinceLastUpdate() < 5 )  
        return;

    // p and n for cleaner code
    const plSeq<plVector3> &p = _points;    
    const plSeq<plVector3> n  = _averageCornerNormals();

    // find tangents in the s and t planes
    plSeq<PLfloat> st(4), tt(4); _computeTangents( st, tt, p, n );
    
    const PLfloat   INC      = 0.02f;  // must divide 1 an odd whole number of times or indexing algorithm will miss a row/column
    const PLuint    NUM_INC  = 1.0f/INC;
    const PLfloat   fNUM_INC = 1.0f/INC;     
    const PLfloat   MAX_DISTANCE = 1.0f;              // colour map max distance, anything beyond this is dark red   
    const plVector3 NO_DATA_COLOUR(0.2, 0.2, 0.2);  // default colour if no data available  

    _triangles = plSeq<plTriangle>( NUM_INC*NUM_INC*2 );
    
    plSeq<plVector3> points ( (NUM_INC+1)*(NUM_INC+1) );
    plSeq<plVector3> colours( (NUM_INC+1)*(NUM_INC+1) );
    
    plSeq<plVector3> interleaved_vertices( NUM_INC * NUM_INC * 6 * 3 );
    plSeq<PLuint>    indices             ( NUM_INC * NUM_INC * 6 );

    for (PLuint j=0; j <= NUM_INC; j++)
    {
        PLfloat v = j/fNUM_INC;

        // interpolate spline corner positions and normals for current v coord
        plVector3 n03 = (1.0f-v)*n[0] + v*n[3];
        plVector3 n12 = (1.0f-v)*n[1] + v*n[2]; 
      
        plVector3 p03 = (1.0f-v)*p[0] + v*p[3];
        plVector3 p12 = (1.0f-v)*p[1] + v*p[2];

        for (PLuint i=0; i <= NUM_INC; i++)
        {
            PLfloat u = i/fNUM_INC;

            // first row, don't triangulate
            PLfloat z = Q( u, v, st, tt );
                 
            // interpolate position and normal for current u coord     
            plVector3 norm = ( (1.0f-u)*n03 + u*n12 ).normalize();           
            plVector3 pos  = (1.0f-u)*p03 + u*p12 + z*norm;          // inflate this point using normal scaled by z value returned by hermite spline
        
            // intersect cartilage
            plIntersection intersection = _model->cartilage.rayIntersect( pos+(10.0f*norm), -norm, false, true ); 
        
            // get colour value
            plVector3 colour = (intersection.exists) ? plColourMap::map( (intersection.point - pos).squaredLength()/MAX_DISTANCE ) : NO_DATA_COLOUR;
             
            points.add( pos );
            colours.add( colour );
            
            if (j > 0 && i > 0 )
            {
                // once past first row, begin triangulating
                PLuint i0 = (i-1) + (j-1) * (NUM_INC+1);
                PLuint i1 = i0 + 1;
                PLuint i2 = i + j * (NUM_INC+1);  
                PLuint i3 = i2 - 1;  
                
                plVector3 normal = ( ( points[i2] - points[i1]) ^ (points[i0] - points[i1]) ).normalize();

                _triangles.add( plTriangle( normal, points[i0], points[i1], points[i2]) );
                _triangles.add( plTriangle( normal, points[i0], points[i2], points[i3]) );

                PLuint base = interleaved_vertices.size() / 3;

                // point 0
                interleaved_vertices.add( points[i0] );     // vertex
                interleaved_vertices.add( normal );         // normal
                interleaved_vertices.add( colours[i0] );    // colour

                // point 1
                interleaved_vertices.add( points[i1] );
                interleaved_vertices.add( normal );
                interleaved_vertices.add( colours[i1] );
                
                // point 2
                interleaved_vertices.add( points[i2] );
                interleaved_vertices.add( normal );
                interleaved_vertices.add( colours[i2] );

                // point 3
                interleaved_vertices.add( points[i3] );
                interleaved_vertices.add( normal );
                interleaved_vertices.add( colours[i3] );

                // triangle indices
                indices.add( base );   indices.add( base+1 );   indices.add( base+2 );
                indices.add( base );   indices.add( base+2 );   indices.add( base+3 );
            }
        }
            
    }   

    _surfaceMesh.setBuffers( interleaved_vertices, indices );
         
    // get min and max extents of model
    plVector3 min, max;
    _getMinMax(min,max);
    
    // build octree       
    _octree.build( min, max, _triangles, 1 );           
    
    // update timer to store time of last update
    _lastUpdate = plTimer::now();
}


PLuint plSpline::_timeSinceLastUpdate()
{
    return plTimer::now() - _lastUpdate;
}
  
void plSpline::_getMinMax(plVector3 &min, plVector3 &max) const
{
    min = plVector3(FLT_MAX, FLT_MAX, FLT_MAX);
    max = -1 * min;

    for ( PLuint i = 0; i < _triangles.size(); i++)
    {  
        const plVector3 &v = _triangles[i].centroid();

        if (v.x < min.x) min.x = v.x;
        if (v.y < min.y) min.y = v.y;
        if (v.z < min.z) min.z = v.z;

        if (v.x > max.x) max.x = v.x;
        if (v.y > max.y) max.y = v.y;
        if (v.z > max.z) max.z = v.z;
    }
}  
  
  
plIntersection plSpline::rayIntersect( const plVector3 &rayOrigin, const plVector3 &rayDirection, PLbool ignoreBehindRay, PLbool backFaceCull ) const
{
    // intersect the octree
    return _octree.rayIntersect( rayOrigin, rayDirection, ignoreBehindRay, backFaceCull );
}    
    
    
/*
void plSpline::_drawSplineSelectionInterface() const
{
    // draw spline
    _plPickingShader->setPickingUniforms(_plPickingState);
       
    plSeq<plVector3> n; 
    n.add( plModelCartilageGetAvgNormal(modelID, 4.0f, corners.points(0), corners.normals(0)) );
    n.add( plModelCartilageGetAvgNormal(modelID, 4.0f, corners.points(1), corners.normals(1)) );
    n.add( plModelCartilageGetAvgNormal(modelID, 4.0f, corners.points(2), corners.normals(2)) );
    n.add( plModelCartilageGetAvgNormal(modelID, 4.0f, corners.points(3), corners.normals(3)) );   
       
    glPushMatrix();
    for (PLuint i = 0; i < corners.size(); i++)
    {      
        _plPickingState->type = PL_PICKING_TYPE_DEFECT_HANDLE_0 + i; 
        _plPickingShader->setPickingUniforms(_plPickingState);
        plColourStack::load( 0.2, 1.0, 0.2 ); 
        plDraw::arrow(corners.points(i), n[i]);
    }
    glPopMatrix();
}
*/








