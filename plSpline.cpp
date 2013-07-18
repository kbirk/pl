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
        //_surfaceMesh.draw();
    }
    
    if (_isSelected)
    {
        //_drawSplineSelectionInterface();
    }            
}

PLfloat Q( PLfloat s, PLfloat t, const plSeq<plVector3> &p, const plSeq<PLfloat> &st, const plSeq<PLfloat> &tt)
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


PLfloat funcTest( PLfloat i, PLfloat j)
{
    //std::cout << "i: " << i << "\n";
    return i * j;
}

void plSpline::_computeHermite()
{    
    // p and n for cleaner code
    const plSeq<plVector3> &p = _points;    
    plSeq<plVector3> n;

    // compute averages normals
    n.add( _model->cartilage.getAverageNormal( 4.0f, p[0], _normals[0]) ); 
    n.add( _model->cartilage.getAverageNormal( 4.0f, p[1], _normals[1]) ); 
    n.add( _model->cartilage.getAverageNormal( 4.0f, p[2], _normals[2]) ); 
    n.add( _model->cartilage.getAverageNormal( 4.0f, p[3], _normals[3]) );  
 
    // get unit directional vectors, (p01 = from p0 to p1)
    plVector3 p01 = (p[1]-p[0]).normalize();
    plVector3 p03 = (p[3]-p[0]).normalize();
    plVector3 p12 = (p[2]-p[1]).normalize();
    plVector3 p32 = (p[2]-p[3]).normalize();
    
    // get corners walls unit normals 
    plVector3 n01 = (0.5*(n[0]+n[1])).normalize();
    plVector3 n03 = (0.5*(n[0]+n[3])).normalize();
    plVector3 n12 = (0.5*(n[1]+n[2])).normalize();
    plVector3 n32 = (0.5*(n[3]+n[2])).normalize();
    
    // clear previous s and t vectors
    _s.clear();  _t.clear();

    // normals of planes between each point   
    plVector3   sn, tn;
    
    // p0 plane normals
    sn = ( p01 ^ -n01 ).normalize();
    tn = ( p03 ^ n03 ).normalize();
    
    _s.add( plMath::projectVectorOnPlane(p03 ^ n[0], sn).normalize() );
    _t.add( plMath::projectVectorOnPlane(p01 ^ -n[0], tn).normalize() ) ; 

    // p1 plane normals
    sn = ( p01 ^ -n01 ).normalize();
    tn = ( p12 ^ n12 ).normalize();
    
    _s.add( plMath::projectVectorOnPlane( p03 ^ n[1], sn).normalize() );
    _t.add( plMath::projectVectorOnPlane( p01 ^ -n[1], tn).normalize() ) ;
    
    // p2 plane normals
    sn = ( p32 ^ -n32 ).normalize();
    tn = ( p12 ^ n12 ).normalize();
    
    _s.add( plMath::projectVectorOnPlane( p12 ^ n[2], sn).normalize() );
    _t.add( plMath::projectVectorOnPlane( p32 ^ -n[2], tn).normalize() ) ;
    
    // p3 plane normals
    sn = ( p32 ^ -n32 ).normalize();
    tn = ( p03 ^ n03 ).normalize();
    
    _s.add( plMath::projectVectorOnPlane( p03 ^ n[3], sn).normalize() );
    _t.add( plMath::projectVectorOnPlane( p32 ^ -n[3], tn).normalize() ) ;

    // find tangents in the s and t planes
    plSeq<PLfloat>   st(4), tt(4); 
    // find the slope of the line along the plane of the spline boundary wall
    // scale by the length between two points to ensure proper scaling
    st.add( (_s[0]*n01) / (_s[0]*p01) * (p[0]-p[1]).length() ); 
    tt.add( (_t[0]*n03) / (_t[0]*p03) * (p[0]-p[3]).length() );     

    st.add( (_s[1]*n01) / (_s[1]*p01) * (p[0]-p[1]).length() ); 
    tt.add( (_t[1]*n12) / (_t[1]*p12) * (p[2]-p[1]).length() ); 

    st.add( (_s[2]*n32) / (_s[2]*p32) * (p[3]-p[2]).length() ); 
    tt.add( (_t[2]*n12) / (_t[2]*p12) * (p[2]-p[1]).length() ); 

    st.add( (_s[3]*n32) / (_s[3]*p32) * (p[3]-p[2]).length() ); 
    tt.add( (_t[3]*n03) / (_t[3]*p03) * (p[0]-p[3]).length() ); 

    const PLfloat inc = 0.05f;
 
    plSeq<plVector3> interleaved_vertices ( ((1.0 / inc) + 1) * 4 * 3 );
    plSeq<PLuint>    indices              ( ((1.0 / inc) + 1) * 6 );

    /*
    for (PLfloat v=0.0; v < 1.0; v+=inc*2.0f)
    {       
        for (PLfloat u=0.0; u < 1.0; u+=inc*2.0f)
        {         
            PLfloat z0 = Q( u,     v,     p, st, tt );
            PLfloat z1 = Q( u+inc, v,     p, st, tt );
            PLfloat z2 = Q( u+inc, v+inc, p, st, tt );
            PLfloat z3 = Q( u,     v+inc, p, st, tt );
        
            // pos0           
            plVector3 n03 = (1.0-v)*n[0] + v*n[3];
            plVector3 n12 = (1.0-v)*n[1] + v*n[2];            
            plVector3 norm = ( (1.0-u)*n03 + u*n12 ).normalize();
            
            plVector3 p03 = (1.0-v)*p[0] + v*p[3];
            plVector3 p12 = (1.0-v)*p[1] + v*p[2];            
            plVector3 pos0 = (1.0-u)*p03 + u*p12 + z0*norm;
            
            // pos1            
            //n03 = (1.0-v)*n[0] + v*n[3];
            //n12 = (1.0-v)*n[1] + v*n[2];            
            norm = ( (1.0-(u+inc))*n03 + (u+inc)*n12 ).normalize();
            
            //p03 = (1.0-v)*p[0] + v*p[3];
            //p12 = (1.0-v)*p[1] + v*p[2];                       
            plVector3 pos1 = (1.0-(u+inc))*p03 + (u+inc)*p12 + z1*norm;
            
            // pos2            
            n03 = (1.0-(v+inc))*n[0] + (v+inc)*n[3];
            n12 = (1.0-(v+inc))*n[1] + (v+inc)*n[2];            
            norm = ( (1.0-(u+inc))*n03 + (u+inc)*n12 ).normalize();
            
            p03 = (1.0-(v+inc))*p[0] + (v+inc)*p[3];
            p12 = (1.0-(v+inc))*p[1] + (v+inc)*p[2];            
            plVector3 pos2 = (1.0-(u+inc))*p03 + (u+inc)*p12 + z2*norm;
                       
            // pos3            
            //n03 = (1.0-(v+inc))*n[0] + (v+inc)*n[3];
            //n12 = (1.0-(v+inc))*n[1] + (v+inc)*n[2];            
            norm = ( (1.0-u)*n03 + u*n12 ).normalize();
            
            //p03 = (1.0-(v+inc))*p[0] + (v+inc)*p[3];
            //p12 = (1.0-(v+inc))*p[1] + (v+inc)*p[2];            
            plVector3 pos3 = (1.0-u)*p03 + u*p12 + z3*norm;

            // quad
            plVector3 faceNorm = ((pos1 - pos0) ^ (pos3 - pos0)).normalize();
               
            plVector3 col0, col1, col2, col3;
            const PLfloat FURTHEST_DISTANCE = 1.0f;
             
            plIntersection intersection(false); 
              
            // colour map values   
             
            intersection = _model->cartilage.rayIntersect( pos0+(10.0f*faceNorm), -faceNorm, false, true );
            if (intersection.exists)
                col0 = plColourMap::map( (intersection.point - pos0).squaredLength()/FURTHEST_DISTANCE );
            else 
                col0 = plVector3(0.2, 0.2, 0.2); 
                                  
            intersection = _model->cartilage.rayIntersect( pos1+(10.0f*faceNorm), -faceNorm, false, true );  
            if (intersection.exists)
                col1 = plColourMap::map((intersection.point - pos1).squaredLength()/FURTHEST_DISTANCE);
            else 
                col1 = plVector3(0.2, 0.2, 0.2);
                
            intersection = _model->cartilage.rayIntersect( pos2+(10.0f*faceNorm), -faceNorm, false, true ); 
            if (intersection.exists)
                col2 = plColourMap::map((intersection.point - pos2).squaredLength()/FURTHEST_DISTANCE);
            else 
                col2 = plVector3(0.2, 0.2, 0.2);
                
            intersection = _model->cartilage.rayIntersect( pos3+(10.0f*faceNorm), -faceNorm, false, true );  
            if (intersection.exists)
                col3 = plColourMap::map((intersection.point - pos3).squaredLength()/FURTHEST_DISTANCE);
            else 
                col3 = plVector3(0.2, 0.2, 0.2);        
                                                                  
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
            
            indices.add(base+0);
            indices.add(base+1);
            indices.add(base+2);
            
            indices.add(base+0);
            indices.add(base+2);
            indices.add(base+3);   
            
            cornerCount += 4;
            centreCount ++;
                            
        }
        
    }    
    */

    std::vector< std::future <void> > splineThreads; //( (1/inc)*(1/inc) );

    for (PLfloat v=0.0; v < 1.0; v+=inc*2.0f)
    {       
        for (PLfloat u=0.0; u < 1.0; u+=inc*2.0f)
        {     
            splineThreads.push_back ( std::async( std::launch::async, &plSpline::_getSplineQuad, this, u, v, inc, p, n, st, tt ) );
            
            //splineThreads.push_back( std::async( std::launch::async, funcTest, u, v )  );
        }
    }
      
    /*
    for (PLuint i = 0; i< splineThreads.size(); i++)
    {
        //splineThreads[i].wait();
        std::cout << "v: " << splineThreads[i].get() << "\n";
    } 
    */
    
    //PLuint axisCount = (1 / inc);

    // process quads 4 at a time
    //  _______
    // |   |   |
    // |_4_|_3_|
    // |   |   |
    // |_1_|_2_|
    /*
    for (PLuint j=0; j < 1/inc; j+=3)
    {
        for (PLuint i=0; i < 1/inc; i+=2)
        {
            
            // quad 1    
            indices.add(i+0);   indices.add(i+1);   indices.add(i+2);   
            indices.add(i+0);   indices.add(i+2);   indices.add(i+3);
            
            // quad 2    
            indices.add(i+1);   indices.add(i+4);   indices.add(i+7);   
            indices.add(i+1);   indices.add(i+7);   indices.add(i+2);
            
        }
    }
    */
    //_surfaceMesh.setBuffers(interleaved_vertices, indices);
}


void plSpline::_getSplineQuad( PLuint u, PLuint v, PLuint inc, const plSeq<plVector3> &p, const plSeq<plVector3> &n, const plSeq<PLfloat> &st, const plSeq<PLfloat> &tt )
{
    PLfloat z0 = Q( u,     v,     p, st, tt );
    PLfloat z1 = Q( u+inc, v,     p, st, tt );
    PLfloat z2 = Q( u+inc, v+inc, p, st, tt );
    PLfloat z3 = Q( u,     v+inc, p, st, tt );

    // pos0           
    plVector3 n03 = (1.0-v)*n[0] + v*n[3];
    plVector3 n12 = (1.0-v)*n[1] + v*n[2];            
    plVector3 norm0 = ( (1.0-u)*n03 + u*n12 ).normalize();
    
    plVector3 p03 = (1.0-v)*p[0] + v*p[3];
    plVector3 p12 = (1.0-v)*p[1] + v*p[2];            
    plVector3 pos0 = (1.0-u)*p03 + u*p12 + z0*norm0;
    
    // pos1                      
    plVector3 norm1 = ( (1.0-(u+inc))*n03 + (u+inc)*n12 ).normalize();                      
    plVector3 pos1  = (1.0-(u+inc))*p03 + (u+inc)*p12 + z1*norm1;
    
    // pos2            
    n03 = (1.0-(v+inc))*n[0] + (v+inc)*n[3];
    n12 = (1.0-(v+inc))*n[1] + (v+inc)*n[2];            
    plVector3 norm2 = ( (1.0-(u+inc))*n03 + (u+inc)*n12 ).normalize();
    
    p03 = (1.0-(v+inc))*p[0] + (v+inc)*p[3];
    p12 = (1.0-(v+inc))*p[1] + (v+inc)*p[2];            
    plVector3 pos2 = (1.0-(u+inc))*p03 + (u+inc)*p12 + z2*norm2;
               
    // pos3                       
    plVector3 norm3 = ( (1.0-u)*n03 + u*n12 ).normalize();          
    plVector3 pos3  = (1.0-u)*p03 + u*p12 + z3*norm3;

    // quad
    plVector3 faceNorm = ((pos1 - pos0) ^ (pos3 - pos0)).normalize();
       
    plVector3 col0, col1, col2, col3;
    const PLfloat FURTHEST_DISTANCE = 1.0f;
    
    // colour map values   
    plIntersection intersection(false); 
    
    intersection = _model->cartilage.rayIntersect( pos0+(10.0f*faceNorm), -faceNorm, false, true );
    if (intersection.exists)
        col0 = plColourMap::map( (intersection.point - pos0).squaredLength()/FURTHEST_DISTANCE );
    else 
        col0 = plVector3(0.2, 0.2, 0.2); 
                          
    intersection = _model->cartilage.rayIntersect( pos1+(10.0f*faceNorm), -faceNorm, false, true );  
    if (intersection.exists)
        col1 = plColourMap::map((intersection.point - pos1).squaredLength()/FURTHEST_DISTANCE);
    else 
        col1 = plVector3(0.2, 0.2, 0.2);
        
    intersection = _model->cartilage.rayIntersect( pos2+(10.0f*faceNorm), -faceNorm, false, true ); 
    if (intersection.exists)
        col2 = plColourMap::map((intersection.point - pos2).squaredLength()/FURTHEST_DISTANCE);
    else 
        col2 = plVector3(0.2, 0.2, 0.2);
        
    intersection = _model->cartilage.rayIntersect( pos3+(10.0f*faceNorm), -faceNorm, false, true );  
    if (intersection.exists)
        col3 = plColourMap::map((intersection.point - pos3).squaredLength()/FURTHEST_DISTANCE);
    else 
        col3 = plVector3(0.2, 0.2, 0.2);                                                     
      
      /*     
    PLint base = interleaved_vertices.size()/3;
    
    interleaved_vertices.add(pos0);     // position
    interleaved_vertices.add(faceNorm); // normal
    interleaved_vertices.add(col0);     // colour
    
    interleaved_vertices.add(pos1);     // position
    interleaved_vertices.add(faceNorm); // normal            
    interleaved_vertices.add(col0);     // colour
    
    interleaved_vertices.add(pos2);     // position
    interleaved_vertices.add(faceNorm); // normal            
    interleaved_vertices.add(col0);     // colour
    
    interleaved_vertices.add(pos3);     // position
    interleaved_vertices.add(faceNorm); // normal
    interleaved_vertices.add(col0);     // colour
    
    indices.add(base+0);
    indices.add(base+1);
    indices.add(base+2);
    
    indices.add(base+0);
    indices.add(base+2);
    indices.add(base+3);   
    */
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








