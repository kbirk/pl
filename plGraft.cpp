#include "plGraft.h"

plGraft::plGraft()
{
}

plGraft::plGraft( const plPlug &h, const plPlug &r, PLfloat radius, PLfloat heightOffset, PLfloat length )
    : recipient(r), harvest(h), _radius( radius ), _markDirection( plVector3(1,0,0) ), _length( length ), _heightOffset( heightOffset ) 
{
    _setCaps();   
}

void plGraft::importCSV( const plSeq<plString> &row, const plSeq<plBoneAndCartilage*> &models )
{
    // Fill in the field            
    plString subfield = row[2];

    if (subfield.compareCaseInsensitive( "harvest" ) )
    {
        harvest.importCSV( row, models );        
        // check if transform, if so, set caps
        if (row[3].compareCaseInsensitive( "transform" ) )
        {
            if (harvest.modelID() == -1 || _radius == 0)
            {
                std::cerr << "plGraft importCSV() error: harvest transform read before model ID or radius";
                exit(1);
            }  
            _setCaps();
        }           
    }                  
    else if (subfield.compareCaseInsensitive( "recipient" ) )
        recipient.importCSV( row, models );   

    else if (subfield.compareCaseInsensitive( "height offset" ) )
        _heightOffset = atof( row[3].c_str() );
                       
    else if (subfield.compareCaseInsensitive( "radius" ) )
        _radius = atof( row[3].c_str() );
        
    else if (subfield.compareCaseInsensitive( "length" ) )
        _length = atof( row[3].c_str() );
       
    else if (subfield.compareCaseInsensitive( "mark direction" ) )
        _markDirection = plVector3( row[3] ).normalize();
    else
        std::cerr << "Error in plan, 'graft': Unrecognized word '" << subfield << "' in third column." << std::endl;
}


void plGraft::_setBoneColour() const
{
    if (_isSelected)
    {
        // selected
        if (plPicking::value.index == PL_PICKING_INDEX_GRAFT_DEFECT)
        {
            plColourStack::load( PL_GRAFT_DEFECT_BONE_COLOUR_DULL );
        }
        else
        {
            plColourStack::load( PL_GRAFT_DONOR_BONE_COLOUR_DULL );
        }
    }
    else
    {
        // not selected
        if (plPicking::value.index == PL_PICKING_INDEX_GRAFT_DEFECT)
        {
            plColourStack::load( PL_GRAFT_DEFECT_BONE_COLOUR );
        }
        else
        {
            plColourStack::load( PL_GRAFT_DONOR_BONE_COLOUR );
        } 
    }
}

void plGraft::_setCartilageColour() const
{
    if (_isSelected)
    {
        // selected
        if (plPicking::value.index == PL_PICKING_INDEX_GRAFT_DEFECT)
        {
            plColourStack::load( PL_GRAFT_DEFECT_CARTILAGE_COLOUR_DULL );
        }
        else
        {
            plColourStack::load( PL_GRAFT_DONOR_CARTILAGE_COLOUR_DULL );
        }  
    }
    else
    {
        // not selected
        if (plPicking::value.index == PL_PICKING_INDEX_GRAFT_DEFECT)
        {
            plColourStack::load( PL_GRAFT_DEFECT_CARTILAGE_COLOUR );
        }
        else
        {
            plColourStack::load( PL_GRAFT_DONOR_CARTILAGE_COLOUR );
        }
    }      
}

void plGraft::draw() const
{
    if ( !_isVisible )
        return;

    // Draw at harvest location
    plModelStack::push( harvest.transform.matrix() );
    {
        plPicking::value.index = PL_PICKING_INDEX_GRAFT_DONOR;              
        _drawGraft();
    }
    plModelStack::pop();


    // Draw at recipient location
    plModelStack::push( recipient.transform.matrix() );
    {
        plModelStack::translate( 0, _heightOffset, 0 );
        plPicking::value.index = PL_PICKING_INDEX_GRAFT_DEFECT;
        _drawGraft();
    }
    plModelStack::pop();

}


void plGraft::_drawGraft() const
{
    plPicking::value.type = PL_PICKING_TYPE_GRAFT; 

    // draw cartilage cap
    if (_cartilageCap.triangles.size() > 0)  // may not always have the cartilage top
    {
        _setCartilageColour();
        _cartilageMesh.draw();
    }
    
    // draw bone cap
    _setBoneColour();
    _boneMesh.draw();
    
    // draw marker   
    plColourStack::load( PL_GRAFT_MARKER_COLOUR );
    plDraw::sphere( _markPosition, 0.5 );

}


void plGraft::_setCaps()
{
    // generate cap polygons
    _findCap( _cartilageCap, harvest.model().cartilage );
    _findCap( _boneCap,      harvest.model().bone      );
    
    // generate meshes   
    _updateCartilageMesh();

    _updateBoneMesh(); 

    _updateMarkPosition();  
}


void plGraft::_updateCartilageMesh()
{
    const plVector3 y(0,1,0);		        // y is cylinder axis (pointing upward)

    plSeq<plVector3> interleaved_vertices;
    plSeq<PLuint>    indices;
    
    for (PLuint i = 0; i < _cartilageCap.triangles.size(); i++)
    {
        PLint base = interleaved_vertices.size()/2;
        
        const plVector3 &p0 = _cartilageCap.triangles[i].point0();
        const plVector3 &p1 = _cartilageCap.triangles[i].point1();
        const plVector3 &p2 = _cartilageCap.triangles[i].point2();
        const plVector3 &n  = _cartilageCap.triangles[i].normal();
        
        interleaved_vertices.add( plVector3( p0.x, p0.y+0.01f, p0.z) );    // position
        interleaved_vertices.add( n );                                     // normal

        interleaved_vertices.add( plVector3( p1.x, p1.y+0.01f, p1.z) );    // position
        interleaved_vertices.add( n );                                     // normal
        
        interleaved_vertices.add( plVector3( p2.x, p2.y+0.01f, p2.z) );    // position
        interleaved_vertices.add( n );                                     // normal

        indices.add(base+0);
        indices.add(base+1); 
        indices.add(base+2);   
    }
    
    // cartilage walls
    if (_boneCap.perimeter.size() > 0 && _cartilageCap.perimeter.size() > 0) 
    {
        int c = 0;
        int b = 0;

        float cOffset = 0;
        float bOffset = 0;

        int stepsLeft = _cartilageCap.perimeter.size() + _boneCap.perimeter.size();

        while (stepsLeft > 0) 
        {
            float cAngle = _cartilageCap.perimeter[c].angle + cOffset;
            float bAngle = _boneCap.perimeter[b].angle + bOffset;
        
            plVector3 n = (_cartilageCap.perimeter[c].point).normalize();
    
            indices.add(interleaved_vertices.size()/2);
            interleaved_vertices.add( _cartilageCap.perimeter[c].point );   // position
            interleaved_vertices.add( n );                                  // normal
            
            indices.add(interleaved_vertices.size()/2);
            interleaved_vertices.add( _boneCap.perimeter[b].point );        // position
            interleaved_vertices.add( n );                                  // normal
        
            if (cAngle < bAngle) 
            {	
                // advance cartilage
                c++;
                if (c == _cartilageCap.perimeter.size()) 
                {
                    c = 0;
                    cOffset = 2 * PL_PI;
                }
        
                indices.add(interleaved_vertices.size()/2);
                interleaved_vertices.add( _cartilageCap.perimeter[c].point );    // position
                interleaved_vertices.add( n );                                  // normal
            }  
            else 
            {			
                // advance bone
                b++;
                if (b == _boneCap.perimeter.size()) 
                {
                    b = 0;
                    bOffset = 2 * PL_PI;
                }
        
                indices.add(interleaved_vertices.size()/2);
                interleaved_vertices.add( _boneCap.perimeter[b].point );        // position
                interleaved_vertices.add( n );                                  // normal
            }
            stepsLeft--;
        }
    }

    if (indices.size() > 0)
    {   
        _cartilageMesh.setBuffers(interleaved_vertices, indices);  
    }
}


void plGraft::_updateBoneMesh()
{        
    const plVector3 y(0,1,0);		        // y is cylinder axis (pointing upward)

    plSeq<plVector3> interleaved_vertices;
    plSeq<PLuint>    indices; 
    
    for (PLuint i = 0; i < _boneCap.triangles.size(); i++)
    {
        PLint base = interleaved_vertices.size()/2;
        
        const plVector3 &p0 = _boneCap.triangles[i].point0();
        const plVector3 &p1 = _boneCap.triangles[i].point1();
        const plVector3 &p2 = _boneCap.triangles[i].point2();
        const plVector3 &n  = _boneCap.triangles[i].normal();
        
        interleaved_vertices.add( plVector3( p0.x, p0.y+0.01f, p0.z) );    // position
        interleaved_vertices.add( n );                                     // normal

        interleaved_vertices.add( plVector3( p1.x, p1.y+0.01f, p1.z) );    // position
        interleaved_vertices.add( n );                                     // normal
        
        interleaved_vertices.add( plVector3( p2.x, p2.y+0.01f, p2.z) );    // position
        interleaved_vertices.add( n );                                     // normal

        indices.add(base+0);
        indices.add(base+1); 
        indices.add(base+2);   
    }
    
    // bone walls
    plVector3 centreBottom = -(_length) * y;
    plVector3 z(0,0,1);
    plVector3 x(1,0,0);

    if (_boneCap.perimeter.size() > 0) 
    {
        PLint base = interleaved_vertices.size()/2;

        float theta = _boneCap.perimeter[0].angle;
        plVector3 n = (cos(theta) * z + sin(theta) * x).normalize();

        plVector3 prevTop = _boneCap.perimeter[0].point;
        plVector3 prevBot = centreBottom + _radius * cos(theta) * z + _radius * sin(theta) * x;

        // top side
        interleaved_vertices.add( prevTop ); // position
        interleaved_vertices.add( n);        // normal
        // bottom side
        interleaved_vertices.add( prevBot ); // position
        interleaved_vertices.add( n);        // normal
        // bottom bottom
        interleaved_vertices.add( prevBot ); // position 
        interleaved_vertices.add( -y);       // normal
        
        for (PLuint i=0; i<_boneCap.perimeter.size(); i++) 
        {
            float theta = _boneCap.perimeter[i].angle;
            plVector3 top = _boneCap.perimeter[i].point;
            plVector3 bot = centreBottom + _radius * cos(theta) * z + _radius * sin(theta) * x;

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
        
        for (PLuint j = 0; j <= _boneCap.perimeter.size()*3; j+=3) 
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

    _boneMesh.setBuffers(interleaved_vertices, indices);
}


void plGraft::_findCap( plCap &cap, const plModel &model )
{   
    // clear previous cap incase
    cap.triangles.clear();
    cap.perimeter.clear();

    plSet<const plTriangle*> triangles;

    model.octree().graftIntersect( triangles, harvest.transform, _radius );

    // must use iterators for set
    plSet<const plTriangle*>::iterator tri_itr = triangles.begin();
    plSet<const plTriangle*>::iterator tri_end = triangles.end();
    for ( ; tri_itr != tri_end; ++tri_itr)
    {
        // check if triangle is on cap
        _triangleIntersection( cap, *(*tri_itr) );
    } 

    // calc just under radius
    PLfloat subSquaredRadius = 0.97f * _radius * _radius;

    // Find vertices of polygons along perimeter of graft
    plSet<plPointAndAngle> angles;    // reserve at least as much space needed

    // if point is very close to radius, assume perimeter point
    for (PLuint i=0; i<cap.triangles.size(); i++) 
    {
        const plVector3 &p0 = cap.triangles[i].point0();
        const plVector3 &p1 = cap.triangles[i].point1();
        const plVector3 &p2 = cap.triangles[i].point2();

        if ((p0.x*p0.x + p0.z*p0.z) > subSquaredRadius) 
            angles.insert( plPointAndAngle( atan2( p0.x, p0.z ), p0 ) );
            
        if ((p1.x*p1.x + p1.z*p1.z) > subSquaredRadius) 
            angles.insert( plPointAndAngle( atan2( p1.x, p1.z ), p1 ) );
            
        if ((p2.x*p2.x + p2.z*p2.z) > subSquaredRadius) 
            angles.insert( plPointAndAngle( atan2( p2.x, p2.z ), p2 ) );
  
    }   
    
    if (angles.size() > 0)
    {       
        cap.perimeter.reserve( angles.size() );  // reserve size
        
        plSet<plPointAndAngle>::iterator angle_itr = angles.begin();
        plSet<plPointAndAngle>::iterator angle_end = angles.end();
        for ( ; angle_itr != angle_end; ++angle_itr)
        {
            cap.perimeter.add( *angle_itr );
        }
    }


}


bool plGraft::_isBeyondHeightThresholds( const plVector3 &p0, const plVector3 &p1, const plVector3 &p2 ) const
{
    const PLfloat VERTICAL_THRESHOLD = 8.0f;
    float proj0 = harvest.transform.projectedDistOnAxis( p0 );
    float proj1 = harvest.transform.projectedDistOnAxis( p1 );
    float proj2 = harvest.transform.projectedDistOnAxis( p2 );

    float maxProj = PL_MAX_OF_3( proj0, proj1, proj2 );
    float minProj = PL_MIN_OF_3( proj0, proj1, proj2 );
      
    return (minProj > VERTICAL_THRESHOLD || maxProj < -VERTICAL_THRESHOLD);  
}



/*
bool plGraft::_triangleIntersection( plCap &cap, const plTriangle &triangle ) const
{
    // if triangle is overlapping cap, cut it (if necessary) and add it to cap triangle list
    if (triangle.normal() * harvest.transform.y() < 0)
        return false;

    float radiusSquared = _radius * _radius;

    plVector3 verts[3];
    verts[0] = harvest.transform.applyInverse( triangle.point0() );
    verts[1] = harvest.transform.applyInverse( triangle.point1() );
    verts[2] = harvest.transform.applyInverse( triangle.point2() );
    
    // Compute distance to graft axis
    float dist[3];
    dist[0] = harvest.transform.squaredDistToAxis( verts[0] );
    dist[1] = harvest.transform.squaredDistToAxis( verts[1] );
    dist[2] = harvest.transform.squaredDistToAxis( verts[2] );
        
    float minDist = PL_MIN_OF_3( dist[0], dist[1], dist[2] );
    
    if (minDist > radiusSquared)
    {        
        // no vertices are inside, check if triangle spans across
        plVector3 e[3];
        e[0] = plMath::closestPointOnSegment( plVector3(0, 0, 0), verts[0], verts[1] );
        e[1] = plMath::closestPointOnSegment( plVector3(0, 0, 0), verts[1], verts[2] );
        e[2] = plMath::closestPointOnSegment( plVector3(0, 0, 0), verts[2], verts[0] );
      
        float d[3];
        d[0] = harvest.transform.squaredDistToAxis( e[0] ); 
        d[1] = harvest.transform.squaredDistToAxis( e[1] ); 
        d[2] = harvest.transform.squaredDistToAxis( e[2] ); 
        
        plSeq<PLuint> insideEdges;
        if ( d[0] < radiusSquared ) insideEdges.add( 0 );
        if ( d[1] < radiusSquared ) insideEdges.add( 1 );
        if ( d[2] < radiusSquared ) insideEdges.add( 2 );

        // At least some of the triangle is inside
        plVector3 normal = harvest.transform.applyNormalInverse( triangle.normal() );

        switch ( insideEdges.size() )
        {
            case 0:
            {           
                return false;
            }   
            case 1:
            {
                plVector3 &m0 = e[ insideEdges[0] ];
                plVector3 &m1 = e[ (insideEdges[0]+1) % 3 ];
                plVector3 &m2 = e[ (insideEdges[0]+2) % 3 ];
                
                plVector3 &m0p0 = verts[ insideEdges[0] ];                
                plVector3 &m0p1 = verts[ (insideEdges[0]+1) % 3 ]; 
                plVector3 &m0p2 = verts[ (insideEdges[0]+2) % 3 ];
                
                plSeq<plVector3> points(4);
                points.add( _pointOnCircumference( m0, m0p1 ) );
                points.add( _pointOnCircumference( m1, m0 ) );
                points.add( _pointOnCircumference( m2, m0 ) );
                points.add( _pointOnCircumference( m0p0, m0 ) );
                
                // polygons reach a max of 4 vertices, so if there are 4, create a second triangle
                cap.triangles.add( plTriangle( normal, points[0], points[1], points[2] ) );
                cap.triangles.add( plTriangle( normal, points[0], points[2], points[3] ) );
                return true;
            }    
            case 2:
            {   
                // two edges are inside
                if ( d[ insideEdges[0] ] > d[ insideEdges[1] ] )
                {
                    // swap so edge 0 is closer than edge 2
                    plUtility::swap( insideEdges[0], insideEdges[1] );                 
                }
                
                plVector3 &m0 = e[ insideEdges[0] ];
                plVector3 &m1 = e[ insideEdges[1] ];
                
                // points before closest points on two closest triangle edges
                plVector3 &m0p0 = verts[ insideEdges[0] ];                
                plVector3 &m1p0 = verts[ insideEdges[1] ];
                // points after closest points on two closest triangle egdes
                plVector3 &m0p1 = verts[ (insideEdges[0]+1) % 3 ]; 
                plVector3 &m1p1 = verts[ (insideEdges[1]+1) % 3 ];
                
                plSeq<plVector3> points(4);
                points.add( _pointOnCircumference( m0, m0p1 ) );
                points.add( _pointOnCircumference( m1p0, m1 ) );
                points.add( _pointOnCircumference( m1, m1p1 ) );
                points.add( _pointOnCircumference( m0p0, m0 ) ); 
                
                cap.triangles.add( plTriangle( normal, points[0], points[1], points[2] ) );
                cap.triangles.add( plTriangle( normal, points[0], points[2], points[3] ) );
                
                return true;             
            }
            case 3:
            {
                std::cout << "3 inside" << "\n";
                return false;
            }
        }

    }
    
    // if too far above or below graft origin, reject.
    if ( _isBeyondHeightThresholds( verts[0], verts[1], verts[2] ) ) return false;
    
    // At least some of the triangle is inside
    plVector3 normal = harvest.transform.applyNormalInverse( triangle.normal() );

    // if all points of triangle are withing radius, accept whole triangle, exit early
    if ( PL_MAX_OF_3( dist[0], dist[1], dist[2] ) <= radiusSquared ) 
    {
        cap.triangles.add( plTriangle( normal, verts[0], verts[1], verts[2] ) );
        return true;
    }
    
    // at least one point is within radius, set it as first point
    if (dist[0] <= radiusSquared) 
    {
        // keep ordering as 0, 1, 2
    } 
    else if (dist[1] <= radiusSquared)
    {
        // shift ordering to 1, 2, 0
        plUtility::swap( verts[0], verts[1] );
        plUtility::swap( dist[0],  dist[1]  );
        
        plUtility::swap( verts[1], verts[2] );
        plUtility::swap( dist[1],  dist[2]  );
    } 
    else 
    {    
        // shift ordering to 2, 0, 1
        plUtility::swap( verts[0], verts[1] );
        plUtility::swap( dist[0],  dist[1]  );
        
        plUtility::swap( verts[0], verts[2] );
        plUtility::swap( dist[0],  dist[2]  );
    }

    bool prevInside = true; // always starts as true (ds[0] <= radiusSquared)

    plSeq<plVector3> points(5);
    
    for ( int i=0; i<3; i++ ) 
    {
        int j = (i+1) % 3;		// vertex at next end of edge

        bool currentInside = (dist[j] <= radiusSquared);

        if ( prevInside && currentInside ) 
        {
            // Add inside triangle point
            points.add( verts[j] );
        } 
        else if ( prevInside && !currentInside ) 
        {
            // Find point on edge of graft
            points.add( _pointOnCircumference( verts[i], verts[j] ) );
        } 
        else if ( !prevInside && currentInside ) 
        {
            // Find entering point and angle 
            points.add( _pointOnCircumference( verts[i], verts[j] ) );
            // Add inside triangle point    
            points.add( verts[j] );
        }
        else if ( !prevInside && !currentInside )
        {
            // check closest point on edge to see if it crosses into graft
            plVector3 m = plMath::closestPointOnSegment ( plVector3(0, 0, 0), verts[i], verts[j] );
            float     d = harvest.transform.squaredDistToAxis( m ); 
            if ( d < radiusSquared )
            {
                // inside
                points.add( _pointOnCircumference( verts[i], m ) );
                points.add( _pointOnCircumference( m, verts[j] ) );
            }
        }

        prevInside = currentInside; // update prev with current
    }

    cap.triangles.add( plTriangle( normal, points[0], points[1], points[2] ) );
    if ( points.size() >= 4 )
    {
        // if there are 4 points, create a second triangle
        cap.triangles.add( plTriangle( normal, points[0], points[2], points[3] ) );
    }
    if ( points.size() == 5 )
    {
        // if there are 5 points, create a third triangle
        cap.triangles.add( plTriangle( normal, points[0], points[3], points[4] ) );
    }
    return true;
}
*/

plSeq<plVector3> plGraft::_pointsOutsideTriangles( plVector3 *verts, PLfloat radiusSquared ) const
{
    // closest points on each segment
    plVector3 e[3];
    e[0] = plMath::closestPointOnSegment( plVector3(0, 0, 0), verts[0], verts[1] );
    e[1] = plMath::closestPointOnSegment( plVector3(0, 0, 0), verts[1], verts[2] );
    e[2] = plMath::closestPointOnSegment( plVector3(0, 0, 0), verts[2], verts[0] );
  
    // distances from each point to centre
    float d[3];
    d[0] = harvest.transform.squaredDistToAxis( e[0] ); 
    d[1] = harvest.transform.squaredDistToAxis( e[1] ); 
    d[2] = harvest.transform.squaredDistToAxis( e[2] ); 
    
    // indexs of inside points
    plSeq<PLuint> insideEdges;
    if ( d[0] < radiusSquared ) insideEdges.add( 0 );
    if ( d[1] < radiusSquared ) insideEdges.add( 1 );
    if ( d[2] < radiusSquared ) insideEdges.add( 2 );
    
    plSeq<plVector3> points(4);
    
    switch ( insideEdges.size() )
    {
        case 0:
        {    
            // no edges overlap       
            break;
        }   
        case 1:
        {
            // one edge overlaps
            plVector3 &m0 = e[ insideEdges[0] ];
            plVector3 &m1 = e[ (insideEdges[0]+1) % 3 ];
            plVector3 &m2 = e[ (insideEdges[0]+2) % 3 ];
            
            // points before and after inside closest point
            plVector3 &m0p0 = verts[ insideEdges[0] ];                
            plVector3 &m0p1 = verts[ (insideEdges[0]+1) % 3 ]; 

            points.add( _pointOnCircumference( m0, m0p1 ) );
            points.add( _pointOnCircumference( m1, m0 ) );
            points.add( _pointOnCircumference( m2, m0 ) );
            points.add( _pointOnCircumference( m0p0, m0 ) );
            
            break;
        }    
        case 2:
        {   
            // two edges overlap
            if ( d[ insideEdges[0] ] > d[ insideEdges[1] ] )
            {
                // swap so first index is closer than second
                plUtility::swap( insideEdges[0], insideEdges[1] );                 
            }
            
            plVector3 &m0 = e[ insideEdges[0] ];
            plVector3 &m1 = e[ insideEdges[1] ];
            
            // points before the two closest points
            plVector3 &m0p0 = verts[ insideEdges[0] ];                
            plVector3 &m1p0 = verts[ insideEdges[1] ];
            // points after the two closest points
            plVector3 &m0p1 = verts[ (insideEdges[0]+1) % 3 ]; 
            plVector3 &m1p1 = verts[ (insideEdges[1]+1) % 3 ];
            
            points.add( _pointOnCircumference( m0, m0p1 ) );
            points.add( _pointOnCircumference( m1p0, m1 ) );
            points.add( _pointOnCircumference( m1, m1p1 ) );
            points.add( _pointOnCircumference( m0p0, m0 ) ); 
            
            break;            
        }
        case 3:
        {
            // three edges overlap, results in 6 triangles, TODO
            break;
        }
    }
    
    return points;
}

plSeq<plVector3> plGraft::_pointsInsideTriangles( plVector3 *verts, PLfloat *dist, PLfloat radiusSquared ) const
{
    // at least one point is within radius, set it as first point
    if (dist[0] <= radiusSquared) 
    {
        // keep ordering as 0, 1, 2
    } 
    else if (dist[1] <= radiusSquared)
    {
        // shift ordering to 1, 2, 0
        plUtility::swap( verts[0], verts[1] );
        plUtility::swap( dist[0],  dist[1]  );
        
        plUtility::swap( verts[1], verts[2] );
        plUtility::swap( dist[1],  dist[2]  );
    } 
    else 
    {    
        // shift ordering to 2, 0, 1
        plUtility::swap( verts[0], verts[1] );
        plUtility::swap( dist[0],  dist[1]  );
        
        plUtility::swap( verts[0], verts[2] );
        plUtility::swap( dist[0],  dist[2]  );
    }

    bool prevInside = true; // always starts as true (ds[0] <= radiusSquared)

    plSeq<plVector3> points(5);
    
    for ( int i=0; i<3; i++ ) 
    {
        int j = (i+1) % 3;		// vertex at next end of edge

        bool currentInside = (dist[j] <= radiusSquared);

        if ( prevInside && currentInside ) 
        {
            // Add inside triangle point
            points.add( verts[j] );
        } 
        else if ( prevInside && !currentInside ) 
        {
            // Find point on edge of graft
            points.add( _pointOnCircumference( verts[i], verts[j] ) );
        } 
        else if ( !prevInside && currentInside ) 
        {
            // Find entering point and angle 
            points.add( _pointOnCircumference( verts[i], verts[j] ) );
            // Add inside triangle point    
            points.add( verts[j] );
        }
        else if ( !prevInside && !currentInside )
        {
            // check closest point on edge to see if it crosses into graft
            plVector3 m = plMath::closestPointOnSegment ( plVector3(0, 0, 0), verts[i], verts[j] );
            float     d = harvest.transform.squaredDistToAxis( m ); 
            if ( d < radiusSquared )
            {
                // inside
                points.add( _pointOnCircumference( verts[i], m ) );
                points.add( _pointOnCircumference( m, verts[j] ) );
            }
        }

        prevInside = currentInside; // update prev with current
    }
    
    return points;
}

bool plGraft::_triangleIntersection( plCap &cap, const plTriangle &triangle ) const
{
    // if triangle is overlapping cap, cut it (if necessary) and add it to cap triangle list
    if (triangle.normal() * harvest.transform.y() < 0)
        return false;

    // get squared radius
    float radiusSquared = _radius * _radius;

    // get triangle verts relative to graft local coordinate system
    plVector3 verts[3];
    verts[0] = harvest.transform.applyInverse( triangle.point0() );
    verts[1] = harvest.transform.applyInverse( triangle.point1() );
    verts[2] = harvest.transform.applyInverse( triangle.point2() );
    
    // if too far above or below graft origin, reject.
    if ( _isBeyondHeightThresholds( verts[0], verts[1], verts[2] ) ) 
        return false;
    
    // Compute distance to graft axis
    float dist[3];
    dist[0] = harvest.transform.squaredDistToAxis( verts[0] );
    dist[1] = harvest.transform.squaredDistToAxis( verts[1] );
    dist[2] = harvest.transform.squaredDistToAxis( verts[2] );
          
    float minDist = PL_MIN_OF_3( dist[0], dist[1], dist[2] );
    float maxDist = PL_MAX_OF_3( dist[0], dist[1], dist[2] );
       
    // At least some of the triangle is inside
    plVector3 normal = harvest.transform.applyNormalInverse( triangle.normal() );   
       
    // if all points of triangle are withing radius, accept whole triangle, exit early
    if ( maxDist <= radiusSquared ) 
    {
        cap.triangles.add( plTriangle( normal, verts[0], verts[1], verts[2] ) );
        return true;
    }  
    
    plSeq<plVector3> points(6);
    
    if (minDist > radiusSquared)
    {  
        // all points outside of triangle, triangle may overlap  
        points = _pointsOutsideTriangles( verts, radiusSquared ); 
    }
    else
    {
        // at least one point is inside triangle
        points = _pointsInsideTriangles( verts, dist, radiusSquared );    
    }
    
    if ( points.size() == 0 )
        return false;

    // add first triangle
    cap.triangles.add( plTriangle( normal, points[0], points[1], points[2] ) );
    
    if ( points.size() >= 4 )
    {
        // if there are 4 points, create a second triangle
        cap.triangles.add( plTriangle( normal, points[0], points[2], points[3] ) );
    }
    if ( points.size() == 5 )
    {
        // if there are 5 points, create a third triangle
        cap.triangles.add( plTriangle( normal, points[0], points[3], points[4] ) );
    }
    return true;
}


plVector3 plGraft::_pointOnCircumference( const plVector3 &u, const plVector3 &v ) const
{
    plVector3 uProj(u.x, 0.0f, u.z ); // ignore component along y axis
    plVector3 vProj(v.x, 0.0f, v.z );

    float a = (vProj-uProj)*(vProj-uProj);
    float b = 2*(uProj*(vProj-uProj));
    float c = uProj*uProj-_radius*_radius;

    float radical = b*b - 4*a*c;
    
    if (radical < 0) 
    {
        std::cout << "plGraft::_pointOnCircumference() error: radical = " << radical << std::endl;
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
        std::cout << "plGraft::_pointOnCircumference() error: t1 = " << t1 << ", t2 = " << t2 << std::endl;
        return plVector3(0,0,0);	// error
    }

    return u + t*(v-u);
    
}


void plGraft::spinMark( PLfloat degrees )
{
    plVector3 axis(0,1,0);
    plMatrix44 rot; rot.setRotationD( degrees, axis );     
    _markDirection = (rot * _markDirection).normalize();    
    _updateMarkPosition();
}


void plGraft::_updateMarkPosition()
{
    // Mark at tool alignment direction on cartilage
    _markPosition = _radius * _markDirection;

    // First, find the closest top perimeter point in the mark direction.
    float minDist = FLT_MAX;
    float minY;

    const plCap &cap = (_cartilageCap.triangles.size() == 0) ? _boneCap : _cartilageCap;

    for (PLuint i=0; i<cap.perimeter.size(); i++) 
    {
        const plVector3 &v = cap.perimeter[i].point;
        float dist = (v.x-_markPosition.x)*(v.x-_markPosition.x) + (v.z-_markPosition.z)*(v.z-_markPosition.z);
        if (dist < minDist) 
        {
            minDist = dist;
            minY = v.y;
        }
    }

    // Draw marker  
    _markPosition.y = minY;
}


const plTransform &plGraft::transform( PLuint type ) const
{
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:
        
            return harvest.transform;
        
        case PL_PICKING_INDEX_GRAFT_DEFECT:
        
            return recipient.transform;
            
        default:
        
            std::cerr << "plGraft transform() error: invalid type enumeration provided, defaulting to recipient \n";
            return recipient.transform;   
    } 
}


void plGraft::translate( PLuint type, const plVector3 &translation )
{       
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:
        
            harvest.translate( translation );
            _setCaps();
            break;
        
        case PL_PICKING_INDEX_GRAFT_DEFECT:
        
            recipient.translate( translation );
            break;
            
        default:
        
            std::cerr << "plGraft translate() error: invalid type enumeration provided \n";
            break;    
    } 
}


void plGraft::translateX( PLuint type, PLfloat distance, const plVector3 &planeNormal  )
{    
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:
        
            harvest.translateX( distance, planeNormal );
            _setCaps();
            break;
        
        case PL_PICKING_INDEX_GRAFT_DEFECT:
        
            recipient.translateX( distance, planeNormal );
            break;
            
        default:
        
            std::cerr << "plGraft translateX() error: invalid type enumeration provided \n";
            break;    
    } 
}


void plGraft::translateZ( PLuint type, PLfloat distance, const plVector3 &planeNormal  )
{    
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:
        
            harvest.translateZ( distance, planeNormal );
            _setCaps();
            break;
        
        case PL_PICKING_INDEX_GRAFT_DEFECT:
        
            recipient.translateZ( distance, planeNormal );
            break;
            
        default:
        
            std::cerr << "plGraft translateZ() error: invalid type enumeration provided \n";
            break;    
    } 
}


void plGraft::rotate( PLuint type, const plVector3 &axis, PLfloat angleDegrees )
{    
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:
        
            harvest.rotate( axis, angleDegrees );
            _setCaps();
            break;
        
        case PL_PICKING_INDEX_GRAFT_DEFECT:
        
            recipient.rotate( axis, angleDegrees );
            break;
            
        default:
        
            std::cerr << "plGraft rotate() error: invalid type enumeration provided \n";
            break;    
    } 
}


