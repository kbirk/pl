#include "plSpline.h" 

plSpline::plSpline() 
{
}

void plSpline::draw() const
{      
    if (!_isVisible)
        return;
      
    // draw spline boundary 
    _plPickingState->type = PL_PICKING_TYPE_DEFECT_BOUNDARY;
    boundary.draw();   
     
    // draw spline corners
    _plPickingState->type = PL_PICKING_TYPE_DEFECT_CORNERS;
    corners.draw();
    
    // draw spline corner axes
    if (PL_BOUNDARY_CURRENT_IS_SELECTED && corners.isVisibile())
    {
        drawCornersSelectionInterface();
    }    
    
    // draw spline surface
    if (corners.size() == 4)
    {
        _plPickingState->type = PL_PICKING_TYPE_DEFECT_SPLINE;       
        _plPickingShader->setPickingUniforms(_plPickingState);
        glColor3f(PL_DEFECT_SPLINE_COLOUR); 
        _mesh.draw();
        
        if (PL_DEFECT_SPLINE_CURRENT_IS_SELECTED)
        {
            drawSplineSelectionInterface();
        }            
    }
}

void plSpline::drawCornersSelectionInterface() const
{
    const plSeq<plVector3> &p = corners.points;
    plSeq<plVector3> n;
    n.add( plModelCartilageGetAvgNormal(0, 4.0f, corners.points[0], corners.normals[0]) );
    n.add( plModelCartilageGetAvgNormal(0, 4.0f, corners.points[1], corners.normals[1]) );
    n.add( plModelCartilageGetAvgNormal(0, 4.0f, corners.points[2], corners.normals[2]) );
    n.add( plModelCartilageGetAvgNormal(0, 4.0f, corners.points[3], corners.normals[3]) );
        
    plVector3 nm = corners.getAvgNormal();
    
    glColor3f( 1.0, 0.2, 0.2 );
    for (PLint i = 0; i < s.size(); i++)
    {
        glPushMatrix();
        glTranslatef( nm.x*6, nm.y*6, nm.z*6 );
        plDrawArrow(p[i], s[i], 4.0f, 0.5f);
        glPopMatrix();
    }

    glColor3f( 0.2, 0.2, 1.0);
    for (PLint i = 0; i < t.size(); i++)
    {
        glPushMatrix();
        glTranslatef( nm.x*6, nm.y*6, nm.z*6 );
        plDrawArrow(p[i], t[i], 4.0f, 0.5f);
        glPopMatrix();        
    }
  
    glColor3f( 0.2, 1.0, 0.2 );
    for (PLint i = 0; i < n.size(); i++)
    {
        glPushMatrix();
        glTranslatef( nm.x*6, nm.y*6, nm.z*6 );
        plDrawArrow(p[i], n[i], 4.0f, 0.5f);
        glPopMatrix();        
    }
    
}

PLfloat Q( PLfloat s, PLfloat t, const plSeq<plVector3> &p, const plSeq<PLfloat> &st, const plSeq<PLfloat> &tt)
{
    // hermite blending matrix
    static plMatrix44 h( 2, -3,  0,  1,
                        -2,  3,  0,  0,
                         1, -2,  1,  0,
                         1, -1,  0,  0 );
                          
    plMatrix44  qm( 0.0f,   0.0f, tt[0],  tt[3],
                    0.0f,   0.0f, tt[1],  tt[2],
                    st[0], st[3],  0.0f,   0.0f,
                    st[1], st[2],  0.0f,   0.0f );    
                                     
    plVector4 sc( s*s*s, s*s, s, 1);
    plVector4 tc( t*t*t, t*t, t, 1);  
    
    return (h * sc) * qm * (h * tc);                         
}

void plSpline::computeHermiteSpline()
{    
    // p and n for cleaner code
    const plSeq<plVector3> &p = corners.points;    
    plSeq<plVector3> n;

    // compute averages normals
    n.add( plModelCartilageGetAvgNormal(0, 4.0f, corners.points[0], corners.normals[0]) );
    n.add( plModelCartilageGetAvgNormal(0, 4.0f, corners.points[1], corners.normals[1]) );
    n.add( plModelCartilageGetAvgNormal(0, 4.0f, corners.points[2], corners.normals[2]) );
    n.add( plModelCartilageGetAvgNormal(0, 4.0f, corners.points[3], corners.normals[3]) );

    // get unit directional vectors, (p01 = from p0 to p1)
    plVector3 p01 = (p[1]-p[0]).normalize();
    plVector3 p03 = (p[3]-p[0]).normalize();
    plVector3 p12 = (p[2]-p[1]).normalize();
    plVector3 p32 = (p[2]-p[3]).normalize();
    
    // average unit normals between each corners 
    plVector3 n01 = (0.5*(n[0]+n[1])).normalize();
    plVector3 n03 = (0.5*(n[0]+n[3])).normalize();
    plVector3 n12 = (0.5*(n[1]+n[2])).normalize();
    plVector3 n32 = (0.5*(n[3]+n[2])).normalize();
    
    // clear previous s and t vectors
    s.clear();  t.clear();

    // normals of planes between each point   
    plVector3   sn, tn;
    
    // p0 plane normals
    sn = ( p01 ^ -n01 ).normalize();
    tn = ( p03 ^ n03 ).normalize();
    
    s.add( plProjectVectorOnPlane(p03 ^ n[0], sn).normalize() );
    t.add( plProjectVectorOnPlane(p01 ^ -n[0], tn).normalize() ) ; 

    // p1 plane normals
    sn = ( p01 ^ -n01 ).normalize();
    tn = ( p12 ^ n12 ).normalize();
    
    s.add( plProjectVectorOnPlane( p03 ^ n[1], sn).normalize() );
    t.add( plProjectVectorOnPlane( p01 ^ -n[1], tn).normalize() ) ;
    
    // p2 plane normals
    sn = ( p32 ^ -n32 ).normalize();
    tn = ( p12 ^ n12 ).normalize();
    
    s.add( plProjectVectorOnPlane( p12 ^ n[2], sn).normalize() );
    t.add( plProjectVectorOnPlane( p32 ^ -n[2], tn).normalize() ) ;
    
    // p3 plane normals
    sn = ( p32 ^ -n32 ).normalize();
    tn = ( p03 ^ n03 ).normalize();
    
    s.add( plProjectVectorOnPlane( p03 ^ n[3], sn).normalize() );
    t.add( plProjectVectorOnPlane( p32 ^ -n[3], tn).normalize() ) ;

    // find tangents in the s and t planes
    plSeq<PLfloat>   st(4), tt(4); 
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

    const PLfloat inc = 0.05f;
 
    plSeq<plVector3>    interleaved_vertices( ((1.0 / inc) + 1) * 4 * 3 );
    plSeq<unsigned int> indices             ( ((1.0 / inc) + 1) * 6 );

    PLfloat percent = 0;

    for (PLfloat v=0.0; v < 1.0; v+=inc)
    {        
        for (PLfloat u=0.0; u < 1.0; u+=inc)
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
            n03 = (1.0-v)*n[0] + v*n[3];
            n12 = (1.0-v)*n[1] + v*n[2];            
            norm = ( (1.0-(u+inc))*n03 + (u+inc)*n12 ).normalize();
            
            p03 = (1.0-v)*p[0] + v*p[3];
            p12 = (1.0-v)*p[1] + v*p[2];                       
            plVector3 pos1 = (1.0-(u+inc))*p03 + (u+inc)*p12 + z1*norm;
            
            // pos2            
            n03 = (1.0-(v+inc))*n[0] + (v+inc)*n[3];
            n12 = (1.0-(v+inc))*n[1] + (v+inc)*n[2];            
            norm = ( (1.0-(u+inc))*n03 + (u+inc)*n12 ).normalize();
            
            p03 = (1.0-(v+inc))*p[0] + (v+inc)*p[3];
            p12 = (1.0-(v+inc))*p[1] + (v+inc)*p[2];            
            plVector3 pos2 = (1.0-(u+inc))*p03 + (u+inc)*p12 + z2*norm;
                       
            // pos3            
            n03 = (1.0-(v+inc))*n[0] + (v+inc)*n[3];
            n12 = (1.0-(v+inc))*n[1] + (v+inc)*n[2];            
            norm = ( (1.0-u)*n03 + u*n12 ).normalize();
            
            p03 = (1.0-(v+inc))*p[0] + (v+inc)*p[3];
            p12 = (1.0-(v+inc))*p[1] + (v+inc)*p[2];            
            plVector3 pos3 = (1.0-u)*p03 + u*p12 + z3*norm;
            
            // quad
            plVector3 faceNorm = ((pos1 - pos0) ^ (pos3 - pos0)).normalize();
               
            plVector3 point, filler;
            plVector3 col0, col1, col2, col3;
            const PLfloat FURTHEST_DISTANCE = 1.0f;
               
            // colour map values      
            if (plModelCartilageIntersect(point, filler, 0, pos0+(10*faceNorm), -faceNorm, false, true))
                col0 = plColourMap( (point - pos0).squaredLength()/FURTHEST_DISTANCE );
            else 
                col0 = plVector3(0.2, 0.2, 0.2); 
                                
            if (plModelCartilageIntersect(point, filler, 0, pos1+(10*faceNorm), -faceNorm, false, true))
                col1 = plColourMap((point - pos1).squaredLength()/FURTHEST_DISTANCE);
            else 
                col1 = plVector3(0.2, 0.2, 0.2);
                
            if (plModelCartilageIntersect(point, filler, 0, pos2+(10*faceNorm), -faceNorm, false, true))
                col2 = plColourMap((point - pos2).squaredLength()/FURTHEST_DISTANCE);
            else 
                col2 = plVector3(0.2, 0.2, 0.2);
                
            if (plModelCartilageIntersect(point, filler, 0, pos3+(10*faceNorm), -faceNorm, false, true))
                col3 = plColourMap((point - pos3).squaredLength()/FURTHEST_DISTANCE);
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
           
            percent += 1 / ((1.0f/inc)*(1.0f/inc));
            
            std::cout << "\r" << (PLint)(percent*100) << "% spline calculation";           
        }
        
    }    
    std::cout << "\r100% spline calculation\n"; 
    _mesh.destroy();
    _mesh = plColourMesh(interleaved_vertices, indices);
}


void plSpline::drawSplineSelectionInterface() const
{
    // draw spline
    _plPickingShader->setPickingUniforms(_plPickingState);
       
    plSeq<plVector3> n; 
    n.add( plModelCartilageGetAvgNormal(0, 4.0f, corners.points[0], corners.normals[0]) );
    n.add( plModelCartilageGetAvgNormal(0, 4.0f, corners.points[1], corners.normals[1]) );
    n.add( plModelCartilageGetAvgNormal(0, 4.0f, corners.points[2], corners.normals[2]) );
    n.add( plModelCartilageGetAvgNormal(0, 4.0f, corners.points[3], corners.normals[3]) );   
       
    glPushMatrix();
    for (PLuint i = 0; i < corners.size(); i++)
    {      
        _plPickingState->type = PL_PICKING_TYPE_DEFECT_HANDLE_0 + i; 
        _plPickingShader->setPickingUniforms(_plPickingState);
        glColor3f( 0.2, 1.0, 0.2 ); 
        plDrawArrow(corners.points[i], n[i]);
    }
    glPopMatrix();
}

////////////////////////////////////////////////

PLbool plDefectSplineIsSelected()
{
    return _plState->boundarySelectedType == PL_PICKING_TYPE_DEFECT_SPLINE;
}












