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
    const PLfloat AVERAGE_RADIUS = 4.0f;

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
    static PLdouble timeSum = 0;
    static PLdouble timeCount = 0;

    // spline colour map computations are very intensive, to prevent unnecessary updates, limit to less than ~60 times per second
    if (_timeSinceLastUpdate() < 10 )  
        return;

    PLtime start = plTimer::now();

    // p and n for cleaner code
    const plSeq<plVector3> &p = _points;    
    const plSeq<plVector3> n  = _averageCornerNormals();

    // find tangents in the s and t planes
    plSeq<PLfloat> st(4), tt(4); _computeTangents( st, tt, p, n );
    
    const PLfloat INC = 0.04f; // must divide 1 an odd whole number of times or indexing algorithm will miss a row/column

    plSeq<plVector3> interleaved_vertices( ((1.0f / INC) + 1) * 4 * 3 );
    plSeq<PLuint>    indices             ( ((1.0f / INC) + 1) * 6 );

    for (PLfloat v=0.0; v < 1.0; v+=INC*2.0f)
    {       
        for (PLfloat u=0.0; u < 1.0; u+=INC*2.0f)
        {      

            PLfloat z0 = Q( u,     v,     st, tt );
            PLfloat z1 = Q( u+INC, v,     st, tt );
            PLfloat z2 = Q( u+INC, v+INC, st, tt );
            PLfloat z3 = Q( u,     v+INC, st, tt );
        
            // pos0           
            plVector3 n03 = (1.0-v)*n[0] + v*n[3];
            plVector3 n12 = (1.0-v)*n[1] + v*n[2];            
            plVector3 norm0 = ( (1.0-u)*n03 + u*n12 ).normalize();

            plVector3 p03 = (1.0-v)*p[0] + v*p[3];
            plVector3 p12 = (1.0-v)*p[1] + v*p[2];            
            plVector3 pos0 = (1.0-u)*p03 + u*p12 + z0*norm0;
            
            // pos1            
            //n03 = (1.0-v)*n[0] + v*n[3];  // redundant
            //n12 = (1.0-v)*n[1] + v*n[2];  // redundant            
            plVector3 norm1 = ( (1.0-(u+INC))*n03 + (u+INC)*n12 ).normalize();
            
            //p03 = (1.0-v)*p[0] + v*p[3];  // redundant
            //p12 = (1.0-v)*p[1] + v*p[2];  // redundant                     
            plVector3 pos1 = (1.0-(u+INC))*p03 + (u+INC)*p12 + z1*norm1;
            
            // pos2            
            n03 = (1.0-(v+INC))*n[0] + (v+INC)*n[3];
            n12 = (1.0-(v+INC))*n[1] + (v+INC)*n[2];            
            plVector3 norm2 = ( (1.0-(u+INC))*n03 + (u+INC)*n12 ).normalize();
            
            p03 = (1.0-(v+INC))*p[0] + (v+INC)*p[3];
            p12 = (1.0-(v+INC))*p[1] + (v+INC)*p[2];            
            plVector3 pos2 = (1.0-(u+INC))*p03 + (u+INC)*p12 + z2*norm2;
                       
            // pos3            
            //n03 = (1.0-(v+INC))*n[0] + (v+INC)*n[3];  // redundant
            //n12 = (1.0-(v+INC))*n[1] + (v+INC)*n[2];  // redundant            
            plVector3 norm3 = ( (1.0-u)*n03 + u*n12 ).normalize();
            
            //p03 = (1.0-(v+INC))*p[0] + (v+INC)*p[3];  // redundant
            //p12 = (1.0-(v+INC))*p[1] + (v+INC)*p[2];  // redundant            
            plVector3 pos3 = (1.0-u)*p03 + u*p12 + z3*norm3;

            // quad normal
            plVector3 faceNorm = ((pos1 - pos0) ^ (pos3 - pos0)).normalize();
               
            const PLfloat MAX_DISTANCE = 1.0f;
             
            plIntersection intersection(false); 
              
            // default colour if no data available  
            plVector3 NO_DATA_COLOUR(0.2, 0.2, 0.2);
            
            // colour map values   
            /*
            std::future<plIntersection> t_i0 = std::async( std::launch::async, &plModel::rayIntersect, &_model->cartilage, pos0+(10.0f*faceNorm), -faceNorm, false, true);
            std::future<plIntersection> t_i1 = std::async( std::launch::async, &plModel::rayIntersect, &_model->cartilage, pos1+(10.0f*faceNorm), -faceNorm, false, true);
            std::future<plIntersection> t_i2 = std::async( std::launch::async, &plModel::rayIntersect, &_model->cartilage, pos2+(10.0f*faceNorm), -faceNorm, false, true);
            std::future<plIntersection> t_i3 = std::async( std::launch::async, &plModel::rayIntersect, &_model->cartilage, pos3+(10.0f*faceNorm), -faceNorm, false, true); 
             
            plIntersection i0 = t_i0.get();  
            plIntersection i1 = t_i1.get(); 
            plIntersection i2 = t_i2.get(); 
            plIntersection i3 = t_i3.get();   
            */
            plIntersection i0 = _model->cartilage.rayIntersect( pos0+(10.0f*faceNorm), -faceNorm, false, true );   
            plIntersection i1 = _model->cartilage.rayIntersect( pos1+(10.0f*faceNorm), -faceNorm, false, true ); 
            plIntersection i2 = _model->cartilage.rayIntersect( pos2+(10.0f*faceNorm), -faceNorm, false, true );
            plIntersection i3 = _model->cartilage.rayIntersect( pos3+(10.0f*faceNorm), -faceNorm, false, true );  
                 
            plVector3 col0 = (i0.exists) ? plColourMap::map( (i0.point - pos0).squaredLength()/MAX_DISTANCE ) : NO_DATA_COLOUR;
            plVector3 col1 = (i0.exists) ? plColourMap::map( (i1.point - pos1).squaredLength()/MAX_DISTANCE ) : NO_DATA_COLOUR;
            plVector3 col2 = (i0.exists) ? plColourMap::map( (i2.point - pos2).squaredLength()/MAX_DISTANCE ) : NO_DATA_COLOUR;
            plVector3 col3 = (i0.exists) ? plColourMap::map( (i3.point - pos3).squaredLength()/MAX_DISTANCE ) : NO_DATA_COLOUR;          
                                                                              
            PLint base = interleaved_vertices.size()/3;
            
            interleaved_vertices.add(pos0);     // position
            interleaved_vertices.add(faceNorm); // normal
            interleaved_vertices.add(col0);     // colour
            
            interleaved_vertices.add(pos1);     // position
            interleaved_vertices.add(faceNorm); // normal            
            interleaved_vertices.add(col1);     // colour
            
            interleaved_vertices.add(pos2);     // position
            interleaved_vertices.add(faceNorm); // normal            
            interleaved_vertices.add(col2);     // colour
            
            interleaved_vertices.add(pos3);     // position
            interleaved_vertices.add(faceNorm); // normal
            interleaved_vertices.add(col3);     // colour
        }           
    }    



    // process quads 4 at a time
    //  _______
    // |   |   |
    // |_4_|_3_|
    // |   |   |
    // |_1_|_2_|
    
    PLuint quadCount = 1/INC;

    for (PLuint j=0; j < quadCount; j+=2)
    {
        for (PLuint i=0; i < quadCount; i+=2)
        {
            PLuint rowCount = ((quadCount)+1); // number of indices per row

            PLuint k = i*2 + j*rowCount;
            
            PLuint l = rowCount*2 + j*rowCount + i*2;

            // quad 1    
            indices.add(k+0);   indices.add(k+1);   indices.add(k+2);   
            indices.add(k+0);   indices.add(k+2);   indices.add(k+3);

            if ( (i+1) != quadCount)
            {
                // quad 2    
                indices.add(k+1);   indices.add(k+4);   indices.add(k+7);   
                indices.add(k+1);   indices.add(k+7);   indices.add(k+2);
            }

            if ( (i+1) != quadCount && (j+1) != quadCount )
            {
                // quad 3    
                indices.add(k+2);   indices.add(k+7);   indices.add(l+4);   
                indices.add(k+2);   indices.add(l+4);   indices.add(l+1);
            }
            
            if ( (j+1) != quadCount )
            {
                // quad 4   
                indices.add(k+3);   indices.add(k+2);   indices.add(l+1);   
                indices.add(k+3);   indices.add(l+1);   indices.add(l+0);
            }
            
        }
    }
    
    _surfaceMesh.setBuffers(interleaved_vertices, indices);
    
    
    PLtime indTime = plTimer::now() - start;
    
    if (indTime < 1000)
    {
        timeSum += indTime;
        timeCount++;    
        std::cout << "time: " << indTime << " avg: " << timeSum/timeCount << "\n";
    }
    
    
    // update timer to store time of last update
    _lastUpdate = plTimer::now();
}


PLuint plSpline::_timeSinceLastUpdate()
{
    return plTimer::now() - _lastUpdate;
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








