#include "plGraft.h"

plGraft::plGraft()
{

}

void plGraft::drawSelectionInterface() const
{
    if (!PL_GRAFT_HANDLES_ENABLED)
        return;

    // draw axes and handles
    glMatrixMode( GL_MODELVIEW );
    
    glPushMatrix();
    glTranslatef( 0, PL_GRAFT_HANDLE_OFFSET, 0 );

    switch(_plState->graftEditMode)
    {
        case PL_GRAFT_EDIT_MODE_TRANSLATE:   
        case PL_GRAFT_EDIT_MODE_ROTATE:
        {
            glColor3f( 0.2, 0.2, 0.2 ); 
            plDrawSphere( 1.5 );

            // x axis
            glColor3f( 1.0, 0.2, 0.2 ); 
            
            _plPickingState->type = PL_PICKING_TYPE_GRAFT_HANDLE_X;
            _plPickingShader->setPickingUniforms(_plPickingState);
        
            (_plState->graftEditMode == PL_GRAFT_EDIT_MODE_TRANSLATE) ? plDrawArrow( plVector3(1,0,0) ) : plDrawCircleArrow( plVector3(1,0,0) );

            if (PL_GRAFT_EDIT_SHOW_Y_HANDLE)    
            {
                // y axis
                glColor3f( 0.2, 1.0, 0.2 );  

                _plPickingState->type = PL_PICKING_TYPE_GRAFT_HANDLE_Y;
                _plPickingShader->setPickingUniforms(_plPickingState);

                (_plState->graftEditMode == PL_GRAFT_EDIT_MODE_TRANSLATE) ? plDrawArrow( plVector3(0,1,0) ) : plDrawCircleArrow( plVector3(0,1,0) );
            }

            // z axis
            glColor3f( 0.2, 0.2, 1.0 ); 
            _plPickingState->type = PL_PICKING_TYPE_GRAFT_HANDLE_Z;
            _plPickingShader->setPickingUniforms(_plPickingState);
        
            (_plState->graftEditMode == PL_GRAFT_EDIT_MODE_TRANSLATE) ? plDrawArrow( plVector3(0,0,1) ) : plDrawCircleArrow( plVector3(0,0,1) );
            
            break;
        }
        case PL_GRAFT_EDIT_MODE_LENGTH:
        {       
            glColor3f( 0.2, 1.0, 0.2 ); 

            _plPickingState->type = PL_PICKING_TYPE_GRAFT_HANDLE_Y;
            _plPickingShader->setPickingUniforms(_plPickingState);

            plDrawArrow( plVector3(0, PL_HANDLE_LENGTH-PL_ARROW_LENGTH-1, 0), plVector3(0,1,0), PL_HANDLE_LENGTH-PL_ARROW_LENGTH);            
            plDrawArrow( plVector3(0, PL_HANDLE_LENGTH-PL_ARROW_LENGTH, 0), plVector3(0,-1,0), PL_HANDLE_LENGTH-PL_ARROW_LENGTH);

            break;     
        }

        
    }
    glPopMatrix();
}

void _plSetGraftBoneColour()
{
    if (PL_GRAFT_NONE_SELECTED || PL_GRAFT_CURRENT_IS_SELECTED)
    {
        // no grafts are selected
        if (PL_GRAFT_IS_DEFECT)
        {
            glColor3f( PL_GRAFT_DEFECT_BONE_COLOUR );
        }
        else
        {
            glColor3f( PL_GRAFT_DONOR_BONE_COLOUR );
        }
    }
    else
    {
        // a graft is selected and it is not this one, dull colour
        if (PL_GRAFT_IS_DEFECT)
        {
            glColor3f( PL_GRAFT_DEFECT_BONE_COLOUR_DULL );
        }
        else
        {
            glColor3f( PL_GRAFT_DONOR_BONE_COLOUR_DULL );
        }       
    }
       
}

void _plSetGraftCartilageColour()
{
    if (PL_GRAFT_NONE_SELECTED || PL_GRAFT_CURRENT_IS_SELECTED)
    {
        // no grafts are selected
        if (PL_GRAFT_IS_DEFECT)
        {
            glColor3f( PL_GRAFT_DEFECT_CARTILAGE_COLOUR );
        }
        else
        {
            glColor3f( PL_GRAFT_DONOR_CARTILAGE_COLOUR );
        }
    }
    else
    {
        // a graft is selected and it is not this one, dull colour
        if (PL_GRAFT_IS_DEFECT)
        {
            glColor3f( PL_GRAFT_DEFECT_CARTILAGE_COLOUR_DULL );
        }
        else
        {
            glColor3f( PL_GRAFT_DONOR_CARTILAGE_COLOUR_DULL );
        }       
    } 
}

void plGraft::draw() const
{
    if (!isVisibile())
        return;

    // Draw at harvest location
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    {
        harvestTransform.apply();
        _plPickingState->index = PL_PICKING_INDEX_GRAFT_DONOR;
        _plPickingShader->setPickingUniforms(_plPickingState);
        drawGraft();

        if (PL_GRAFT_CURRENT_IS_SELECTED && PL_GRAFT_SELECTED_IS_DONOR) 
        {
            drawSelectionInterface();
            _plPickingState->type = PL_PICKING_TYPE_GRAFT;   // reset after handle is drawn
        }
    }
    glPopMatrix();

    // Draw at recipient location
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    {
        recipientTransform.apply();
        glTranslatef( 0, height_offset, 0 );
        _plPickingState->index = PL_PICKING_INDEX_GRAFT_DEFECT;
        _plPickingShader->setPickingUniforms(_plPickingState);
        drawGraft();

        if (PL_GRAFT_CURRENT_IS_SELECTED && PL_GRAFT_SELECTED_IS_DEFECT) 
        {
            drawSelectionInterface();
        }
    }
    glPopMatrix();
}


void plGraft::drawGraft() const
{
    // draw cartilage cap
    if (cartilageCap.polys.size() > 0)
    {
        _plSetGraftCartilageColour();
        cartilageMesh.draw();
    }
    // draw bone cap
    _plSetGraftBoneColour();
    boneMesh.draw();
    
    // Mark at tool alignment direction on cartilage
    plVector3 mark = radius * markDirection;

    // First, find the closest top perimeter point in the mark direction.
    float minDist = FLT_MAX;
    float minY;

    const plCap &cap = (cartilageCap.polys.size() == 0) ? boneCap : cartilageCap;

    for (int i=0; i<cap.perimeter.size(); i++) 
    {
        const plVector3 &v = cap.perimeter[i].point;
        float dist = (v.x-mark.x)*(v.x-mark.x) + (v.z-mark.z)*(v.z-mark.z);
        if (dist < minDist) 
        {
            minDist = dist;
            minY = v.y;
        }
    }

    // Draw marker  
    mark.y = minY;

    glColor3f( PL_GRAFT_MARKER_COLOUR );
    glPushMatrix();
    glTranslatef( mark.x, mark.y, mark.z );
    plDrawSphere( 0.5 );
    glPopMatrix();
}


void plGraft::setCaps( const plSeq<plTriangle> &cartilageTriangles, const plSeq<plTriangle> &boneTriangles  )
{
    cartilageCap = findCap( cartilageTriangles, harvestTransform.y);
    boneCap      = findCap( boneTriangles,      harvestTransform.y);
       
    updateCartilageMesh();   
    updateBoneMesh();      
}

bool _comparePointAndAngle( const plPointAndAngle &a, const plPointAndAngle &b )
{
    return (a.angle < b.angle);
}

void plGraft::updateCartilageMesh()
{
    const plVector3 y(0,1,0);		        // y is cylinder axis (pointing upward)

    plSeq<plVector3> interleaved_vertices;
    plSeq<unsigned int> indices;

    // cartilage top
    for (int i = 0; i < cartilageCap.polys.size(); i++)
    {
        PLint base = interleaved_vertices.size()/2;
        plPoly &p = cartilageCap.polys[i];
        for (int j = 0; j < p.vertices.size(); j++)
        {
            plVector3 &v = p.vertices[j];
            interleaved_vertices.add( plVector3( v.x, v.y+0.01f, v.z) );    // position
            interleaved_vertices.add( p.normal );                           // normal
        }
        
        for (int j = 1; j <= p.vertices.size()-2; j++) 
        {
            indices.add(base+0);
            indices.add(base+j); 
            indices.add(base+j+1);   
        }
    }

    // cartilage walls
    if (boneCap.perimeter.size() > 0 && cartilageCap.perimeter.size() > 0) 
    {
        int c = 0;
        int b = 0;

        float cOffset = 0;
        float bOffset = 0;

        int stepsLeft = cartilageCap.perimeter.size() + boneCap.perimeter.size();

        while (stepsLeft > 0) 
        {

            float cAngle = cartilageCap.perimeter[c].angle + cOffset;
            float bAngle = boneCap.perimeter[b].angle + bOffset;
        
            plVector3 n = (cartilageCap.perimeter[c].point).normalize();
    
            indices.add(interleaved_vertices.size()/2);
            interleaved_vertices.add( cartilageCap.perimeter[c].point );    // position
            interleaved_vertices.add( n );                                  // normal
            
            indices.add(interleaved_vertices.size()/2);
            interleaved_vertices.add( boneCap.perimeter[b].point );         // position
            interleaved_vertices.add( n );                                  // normal
        
            if (cAngle < bAngle) 
            {	
                // advance cartilage
                c++;
                if (c == cartilageCap.perimeter.size()) 
                {
                    c = 0;
                    cOffset = 2 * M_PI;
                }
        
                indices.add(interleaved_vertices.size()/2);
                interleaved_vertices.add( cartilageCap.perimeter[c].point );    // position
                interleaved_vertices.add( n );                                  // normal
            }  
            else 
            {			
                // advance bone
                b++;
                if (b == boneCap.perimeter.size()) 
                {
                    b = 0;
                    bOffset = 2 * M_PI;
                }
        
                indices.add(interleaved_vertices.size()/2);
                interleaved_vertices.add( boneCap.perimeter[b].point );         // position
                interleaved_vertices.add( n );                                  // normal
            }
            stepsLeft--;
        }
    }
       
    if (indices.size() > 0)
    {
        cartilageMesh.destroy();
        cartilageMesh = plMesh(interleaved_vertices, indices);  
    }
}


void plGraft::updateBoneMesh()
{        
    const plVector3 y(0,1,0);		        // y is cylinder axis (pointing upward)

    plSeq<plVector3> interleaved_vertices;
    plSeq<unsigned int> indices; 
       
    // bone top (only if no cartilage top)
    if (cartilageCap.polys.size() == 0)
    {
        for (int i = 0; i < boneCap.polys.size(); i++)
        {
            PLint base = interleaved_vertices.size()/2;
            plPoly &p = boneCap.polys[i];
            for (int j = 0; j < p.vertices.size(); j++)
            {
                plVector3 &v = p.vertices[j];
                interleaved_vertices.add( plVector3( v.x, v.y+0.01f, v.z) );        // position
                interleaved_vertices.add( p.normal );                               // normal
            }
            
            for (int j = 1; j <= p.vertices.size()-2; j++) 
            {
                indices.add(base+0);
                indices.add(base+j); 
                indices.add(base+j+1);   
            }
        }
    }

    // bone walls
    plVector3 centreBottom = -(length) * y;
    plVector3 z(0,0,1);
    plVector3 x(1,0,0);

    if (boneCap.perimeter.size() > 0) 
    {
        PLint base = interleaved_vertices.size()/2;

        float theta = boneCap.perimeter[0].angle;
        plVector3 n = (cos(theta) * z + sin(theta) * x).normalize();

        plVector3 prevTop = boneCap.perimeter[0].point;
        plVector3 prevBot = centreBottom + radius * cos(theta) * z + radius * sin(theta) * x;

        // top side
        interleaved_vertices.add( prevTop ); // position
        interleaved_vertices.add( n);        // normal
        // bottom side
        interleaved_vertices.add( prevBot ); // position
        interleaved_vertices.add( n);        // normal
        // bottom bottom
        interleaved_vertices.add( prevBot ); // position 
        interleaved_vertices.add( -y);       // normal
        
        for (int i=0; i<boneCap.perimeter.size(); i++) 
        {
            float theta = boneCap.perimeter[i].angle;
            plVector3 top = boneCap.perimeter[i].point;
            plVector3 bot = centreBottom + radius * cos(theta) * z + radius * sin(theta) * x;

            plVector3 n = (cos(theta) * z + sin(theta) * x).normalize();
            // top side
            interleaved_vertices.add( top ); // position
            interleaved_vertices.add( n);    // normal
            // bottom side    
            interleaved_vertices.add( bot ); // position
            interleaved_vertices.add( n);    // normal
            // bottom bottom
            interleaved_vertices.add( bot ); // position 
            interleaved_vertices.add( -y);   // normal
        }
        // top side
        interleaved_vertices.add( prevTop ); // position
        interleaved_vertices.add( n);        // normal
        // bottom side
        interleaved_vertices.add( prevBot ); // position
        interleaved_vertices.add( n);        // normal
        // bottom bottom
        interleaved_vertices.add( prevBot ); // position 
        interleaved_vertices.add( -y);       // normal
        // bottom centre point
        interleaved_vertices.add( centreBottom );   // position
        interleaved_vertices.add( -y );             // normal  
        
        for (int j = 0; j <= boneCap.perimeter.size()*3; j+=3) 
        {
            // side t1
            indices.add(base+j);
            indices.add(base+j+1);
            indices.add(base+j+3);
            // side t2
            indices.add(base+j+1);
            indices.add(base+j+4);
            indices.add(base+j+3);
            // bottom t3
            indices.add(base+j+2);
            indices.add(interleaved_vertices.size()/2 - 1);
            indices.add(base+j+5);
        }

    }

    boneMesh.destroy();
    boneMesh = plMesh(interleaved_vertices, indices);
}


plCap plGraft::findCap( const plSeq<plTriangle> &triangles, const plVector3 &up )
{   
    plCap cap;

    // Find polygons on top of graft
    for (PLint i=0; i<triangles.size(); i++) 
    {
        plPoly p;
        if (triangles[i].normal * up > 0 && triangleIntersection( triangles[i], p ))
        {
            cap.polys.add( p );
        }
    } 

    // Find vertices of polygons on boundary of graft
    std::vector<plPointAndAngle> angles;

    for (PLint i=0; i<cap.polys.size(); i++) 
    {
        for (PLint j=0; j<cap.polys[i].vertices.size(); j++) 
        {
            plVector3 &v = cap.polys[i].vertices[j];
            
            if ((v.x*v.x + v.z*v.z) > 0.97f * radius* radius)
            {
                angles.push_back( plPointAndAngle( atan2( v.x, v.z ), v ));
            }
        }
    }

    if (angles.size() > 0) 
    {
        std::sort(angles.begin(), angles.end(), _comparePointAndAngle);
     
        cap.perimeter.add( plPointAndAngle( angles[0].angle, angles[0].point ) );
          
        for (PLint i=1; i<angles.size(); i++) 
        {
            if ( (angles[i].point - angles[i-1].point).squaredLength() > 0.01f)
            {
                cap.perimeter.add( angles[i]) ; 
            }
        }
	}

	return cap;
}


// Return the Poly that is the intersection of a triangle with the
// interior of the cylinder.


bool plGraft::triangleIntersection( const plTriangle &tri, plPoly &p ) const
{
    static float min = FLT_MAX;

    float radiusSquared = radius * radius;

    plVector3 point1 = harvestTransform.applyInverse( tri.point1 );
    plVector3 point2 = harvestTransform.applyInverse( tri.point2 );
    plVector3 point3 = harvestTransform.applyInverse( tri.point3 );

    // Compute distance to graft axis
    float dist1 = harvestTransform.squaredDistToAxis( point1 );
    float dist2 = harvestTransform.squaredDistToAxis( point2 );
    float dist3 = harvestTransform.squaredDistToAxis( point3 );

    // If too far from graft axis, reject.  Note that this will miss
    // some slightly-overlapping triangles!

    float minDist = PL_MIN_OF_3( dist1, dist2, dist3 );

    if (minDist > radiusSquared)
        return false;

    // If too far away from graft origin, reject.
    
    float proj1 = harvestTransform.projectedDistOnAxis( point1 );
    float proj2 = harvestTransform.projectedDistOnAxis( point2 );
    float proj3 = harvestTransform.projectedDistOnAxis( point3 );

    float maxProj = PL_MAX_OF_3( proj1, proj2, proj3 );

    if (maxProj < -length)
        return false;

    // At least some of the triangle is inside

    p.normal = harvestTransform.applyNormalInverse(tri.normal);

    // If entirely within the graft, accept the whole triangle (this is cheaper).

    float maxDist = PL_MAX_OF_3( dist1, dist2, dist3 );

    if (maxDist <= radiusSquared) 
    {
        p.vertices.add( point1 );
        p.vertices.add( point2 );
        p.vertices.add( point3 );
        return true;
    }

    // Not entirely within the graft, so find the intersection of the
    // triangle with the cylinder wall

    // Find a first vertex that is inside

    plVector3 vs[3];
    float ds[3];
  
    if (dist1 <= radiusSquared) 
    {
        vs[0] = point1; ds[0] = dist1;
        vs[1] = point2; ds[1] = dist2;
        vs[2] = point3; ds[2] = dist3;
    } 
    else if (dist2 <= radiusSquared)
    {
        vs[0] = point2; ds[0] = dist2;
        vs[1] = point3; ds[1] = dist3;
        vs[2] = point1; ds[2] = dist1;
    } 
    else 
    {    
        vs[0] = point3; ds[0] = dist3;
        vs[1] = point1; ds[1] = dist1;
        vs[2] = point2; ds[2] = dist2;
    }

    bool prevInside = true; // always starts as true (ds[0] <= radiusSquared)

    for (int i=0; i<3; i++) 
    {
        int j = (i+1) % 3;		// vertex at next end of edge

        bool nextInside = (ds[j] <= radiusSquared);

        if (prevInside && nextInside) 
        {
            // Add inside triangle point
            p.vertices.add( vs[j] );
        } 
        else if (prevInside && !nextInside) 
        {
            // Find point on edge of graft
            p.vertices.add( pointOnCircumference(vs[i], vs[j]) );
        } 
        else if (!prevInside && nextInside) 
        {
            // Find entering point and angle 
            p.vertices.add( pointOnCircumference(vs[i], vs[j]) );    
            // Add inside triangle point    
            p.vertices.add( vs[j] );
        }

        prevInside = nextInside;
    }

    return true;
}


plVector3 plGraft::pointOnCircumference( const plVector3 &u, const plVector3 &v ) const
{
    plVector3 uProj = u;		// ignore component along axis
    plVector3 vProj = v;

    uProj.y = 0;
    vProj.y = 0;

    float a = (vProj-uProj)*(vProj-uProj);
    float b = 2*(uProj*(vProj-uProj));
    float c = uProj*uProj-radius*radius;

    float radical = b*b - 4*a*c;
    
    if (radical < 0) 
    {
        std::cout << "radical = " << radical << std::endl;
        return plVector3(0,0,0);	// error
    }

    float root = sqrt(radical);

    float t1 = (-b + root)/(2*a);
    float t2 = (-b - root)/(2*a);

    float t;
    if (t1 >= 0 && t1 <= 1 && (t1 <= t2 || t2 <= 0))
    {
        t = t1;
    }
    else if (t2 >= 0 && t2 <= 1 && (t2 <= t1 || t1 >= 1))
    {
        t = t2;
    }
    else 
    {
        std::cout << "t1 = " << t1 << ", t2 = " << t2 << std::endl;
        return plVector3(0,0,0);	// error
    }

    return u + t*(v-u);
    
}


float plGraft::angleOfPoint( const plVector3 &v ) const
{
    return atan2( v*harvestTransform.x, v*harvestTransform.z );
}


plVector3 plGraft::pointAtAngle( float theta ) const
{
    return cos(theta) * harvestTransform.z + sin(theta) * harvestTransform.x;
}


/////////////////////////////////////////


void plGraftSelectDefect( PLuint graft_id )
{    
    if (plErrorCheckGraftBounds(graft_id, "plGraftSelectDefect"))
        return; 
    
    _plState->selectGraft(graft_id, PL_PICKING_INDEX_GRAFT_DEFECT);        
}

void plGraftSelectDonor( PLuint graft_id )
{
    if (plErrorCheckGraftBounds(graft_id, "plGraftSelectDonor"))
        return;
    
    _plState->selectGraft(graft_id, PL_PICKING_INDEX_GRAFT_DONOR);   
}

void plGraftEnableHandles()
{
    _plState->graftHandlesEnabled = true;
}

void plGraftDisableHandles()
{
    _plState->graftHandlesEnabled = false;
}

PLbool plGraftIsSelected()
{
    return _plState->graftSelectedID != -1;
}

PLint plGraftGetSelectedID()
{
    return _plState->graftSelectedID;
}

void plGraftSetRotateMode()
{
    _plState->graftEditMode = PL_GRAFT_EDIT_MODE_ROTATE;
} 

void plGraftSetTranslateMode()
{
    _plState->graftEditMode = PL_GRAFT_EDIT_MODE_TRANSLATE;
}

void plGraftSetLengthMode()
{
    _plState->graftEditMode = PL_GRAFT_EDIT_MODE_LENGTH;
}






