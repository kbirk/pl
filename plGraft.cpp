#include "plGraft.h"

plGraft::plGraft()
{
}


plGraft::plGraft( const plPlug &harvest, const plPlug &recipient, PLfloat radius, PLfloat cartilageThickness, PLfloat heightOffset, PLfloat length, const plVector3 &markDirection  )
    : _recipient( recipient ), _harvest( harvest ), _radius( radius ), _markDirection( markDirection ), _length( length ), _cartilageThickness( cartilageThickness ), _heightOffset( heightOffset ) 
{
    _setCaps();   
}


plVector4 plGraft::_getBoneColour() const
{
    /*
    if (_isSelected)
    {
        // selected
        if ( plPickingStack::topBlue() == PL_PICKING_INDEX_GRAFT_DEFECT )
        {
            return plVector4( PL_GRAFT_DEFECT_BONE_COLOUR_DULL );
        }
        else
        {
            return plVector4( PL_GRAFT_DONOR_BONE_COLOUR_DULL );
        }
    }
    else
    {
    */
        // not selected
        if ( plPickingStack::topBlue() == PL_PICKING_INDEX_GRAFT_DEFECT )
        {
            return plVector4( PL_GRAFT_DEFECT_BONE_COLOUR );
        }
        else
        {
            return plVector4( PL_GRAFT_DONOR_BONE_COLOUR );
        } 
    //}
}


plVector4 plGraft::_getCartilageColour() const
{

    /*
    if (_isSelected)
    {
        // selected
        if ( plPickingStack::topBlue() == PL_PICKING_INDEX_GRAFT_DEFECT )
        {
            return plVector4( PL_GRAFT_DEFECT_CARTILAGE_COLOUR_DULL );
        }
        else
        {
            return plVector4( PL_GRAFT_DONOR_CARTILAGE_COLOUR_DULL );
        }  
    }
    else
    {
    */
    
        // not selected
        if ( plPickingStack::topBlue() == PL_PICKING_INDEX_GRAFT_DEFECT )
        {
            return plVector4( PL_GRAFT_DEFECT_CARTILAGE_COLOUR );
        }
        else
        {
            return plVector4( PL_GRAFT_DONOR_CARTILAGE_COLOUR );
        }
    //}      
}

/*
void plGraft::draw() const
{
    if ( !_isVisible )
        return;

    // Draw at harvest location
    plModelStack::push( _harvest.transform().matrix() );
    {
        plPickingStack::loadBlue( PL_PICKING_INDEX_GRAFT_DONOR );                  
        _drawGraft();
    }
    plModelStack::pop();

    // Draw at recipient location
    plModelStack::push( _recipient.transform().matrix() );
    {
        plModelStack::translate( 0, _heightOffset, 0 );
        plPickingStack::loadBlue( PL_PICKING_INDEX_GRAFT_DEFECT );
        _drawGraft();
    }
    plModelStack::pop();
}
*/


void plGraft::extractRenderComponents( plRenderMap& renderMap ) const
{
    if ( !_isVisible )
        return;

    // Draw at harvest location
    plModelStack::push( _harvest.transform().matrix() );
    {
        plPickingStack::loadBlue( PL_PICKING_INDEX_GRAFT_DONOR );                  
        _extractGraftRenderComponents( renderMap );
    }
    plModelStack::pop();

    // Draw at recipient location
    plModelStack::push( _recipient.transform().matrix() );
    {
        plModelStack::translate( 0, _heightOffset, 0 );
        plPickingStack::loadBlue( PL_PICKING_INDEX_GRAFT_DEFECT );
        _extractGraftRenderComponents( renderMap );
    }
    plModelStack::pop();

}


void plGraft::_extractGraftRenderComponents( plRenderMap& renderMap ) const
{
    plPickingStack::loadRed( PL_PICKING_TYPE_GRAFT ); 

    // draw cartilage cap
    if ( _cartilageCap.triangles.size() > 0 )  // may not always have the cartilage top
    {
        plColourStack::load( _getCartilageColour() );    
          
        // create render component
        plRenderComponent component( &_cartilageVAO );
        // attached uniforms
        component.attach( plUniform( PL_MODEL_MATRIX_UNIFORM,      plModelStack::top()      ) );
        component.attach( plUniform( PL_VIEW_MATRIX_UNIFORM,       plCameraStack::top()     ) );
        component.attach( plUniform( PL_PROJECTION_MATRIX_UNIFORM, plProjectionStack::top() ) );
        component.attach( plUniform( PL_COLOUR_UNIFORM,            plColourStack::top()     ) ); 
        component.attach( plUniform( PL_PICKING_UNIFORM,           plPickingStack::top()    ) );
        component.attach( plUniform( PL_LIGHT_POSITION_UNIFORM,    plVector3( PL_LIGHT_POSITION ) ) ); 
        // insert into render map     
        renderMap[ PL_PLAN_TECHNIQUE ].insert( component );   
    }
    
    // draw bone cap
    plColourStack::load( _getBoneColour() );
       
    // create render component
    plRenderComponent component( &_boneVAO );
    // attached uniforms
    component.attach( plUniform( PL_MODEL_MATRIX_UNIFORM,      plModelStack::top()      ) );
    component.attach( plUniform( PL_VIEW_MATRIX_UNIFORM,       plCameraStack::top()     ) );
    component.attach( plUniform( PL_PROJECTION_MATRIX_UNIFORM, plProjectionStack::top() ) );
    component.attach( plUniform( PL_COLOUR_UNIFORM,            plColourStack::top()     ) ); 
    component.attach( plUniform( PL_PICKING_UNIFORM,           plPickingStack::top()    ) );
    component.attach( plUniform( PL_LIGHT_POSITION_UNIFORM,    plVector3( PL_LIGHT_POSITION ) ) ); 
    // insert into render map   
    renderMap[ PL_PLAN_TECHNIQUE ].insert( component );  
    
    // draw marker   
    plColourStack::load( PL_GRAFT_MARKER_COLOUR );
    plPickingStack::loadRed( PL_PICKING_TYPE_GRAFT_MARKER );
    plRenderer::queue( plSphere( PL_PLAN_TECHNIQUE, _markPosition, 0.5f ) );

}

/*
void plGraft::_drawGraft() const
{
    plPickingStack::loadRed( PL_PICKING_TYPE_GRAFT ); 

    // draw cartilage cap
    if (_cartilageCap.triangles.size() > 0)  // may not always have the cartilage top
    {
        _setCartilageColour();
        _cartilageVAO.draw();
    }
    
    // draw bone cap
    _setBoneColour();
    _boneVAO.draw();
    
    // draw marker   
    plColourStack::load( PL_GRAFT_MARKER_COLOUR );
    plPickingStack::loadRed( PL_PICKING_TYPE_GRAFT_MARKER );
    plDraw::sphere( _markPosition, 0.5 );

}
*/

void plGraft::_setCaps()
{
    // generate cap polygons
    _findCap( _cartilageCap, _harvest.model().cartilage );
    _findCap( _boneCap,      _harvest.model().bone      );
    
    // generate meshes   
    _generateCartilageVAO();
    _generateBoneVAO(); 

    // update values
    _updateMarkPosition();      
    _updateCartilageThickness();
}


void plGraft::_generateCartilageVAO()
{
    const plVector3 y(0,1,0);		        // y is cylinder axis (pointing upward)

    std::vector<plVector3> vertices;     vertices.reserve( _cartilageCap.triangles.size()*6 + ( _cartilageCap.perimeter.size() + _boneCap.perimeter.size() )*6 );
    std::vector<PLuint>    indices;      indices.reserve( _cartilageCap.triangles.size()*3 + ( _cartilageCap.perimeter.size() + _boneCap.perimeter.size() )*3 );
    
    for (PLuint i = 0; i < _cartilageCap.triangles.size(); i++)
    {
        PLint base = vertices.size()/2;
        
        const plVector3 &p0 = _cartilageCap.triangles[i].point0();
        const plVector3 &p1 = _cartilageCap.triangles[i].point1();
        const plVector3 &p2 = _cartilageCap.triangles[i].point2();
        const plVector3 &n  = _cartilageCap.triangles[i].normal();
        
        vertices.push_back( plVector3( p0.x, p0.y+0.01f, p0.z) );    // position
        vertices.push_back( n );                                     // normal

        vertices.push_back( plVector3( p1.x, p1.y+0.01f, p1.z) );    // position
        vertices.push_back( n );                                     // normal
        
        vertices.push_back( plVector3( p2.x, p2.y+0.01f, p2.z) );    // position
        vertices.push_back( n );                                     // normal

        indices.push_back(base+0);
        indices.push_back(base+1); 
        indices.push_back(base+2);   
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
    
            indices.push_back(vertices.size()/2);
            vertices.push_back( _cartilageCap.perimeter[c].point );   // position
            vertices.push_back( n );                                  // normal
            
            indices.push_back(vertices.size()/2);
            vertices.push_back( _boneCap.perimeter[b].point );        // position
            vertices.push_back( n );                                  // normal
        
            if (cAngle < bAngle) 
            {	
                // advance cartilage
                c++;
                if (c == _cartilageCap.perimeter.size()) 
                {
                    c = 0;
                    cOffset = 2 * PL_PI;
                }
        
                indices.push_back(vertices.size()/2);
                vertices.push_back( _cartilageCap.perimeter[c].point );    // position
                vertices.push_back( n );                                  // normal
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
        
                indices.push_back(vertices.size()/2);
                vertices.push_back( _boneCap.perimeter[b].point );        // position
                vertices.push_back( n );                                  // normal
            }
            stepsLeft--;
        }
    }

    if (indices.size() > 0)
    {   
        // set vbo and attach attribute pointers
        std::shared_ptr<plVBO> vbo( new plVBO() );
        vbo->set( vertices );
        vbo->set( plVertexAttributePointer( PL_POSITION_ATTRIBUTE, 0  ) );
        vbo->set( plVertexAttributePointer( PL_NORMAL_ATTRIBUTE,   16 ) );
        // set eabo
        std::shared_ptr<plEABO> eabo( new plEABO() );   
        eabo->set( indices );
        // attach to vao
        _cartilageVAO.attach( vbo );
        _cartilageVAO.attach( eabo );
        // upload to gpu
        _cartilageVAO.upload(); 
    }
}


void plGraft::_generateBoneVAO()
{        
    const plVector3 y(0,1,0);		        // y is cylinder axis (pointing upward)

    std::vector<plVector3> vertices;     vertices.reserve( _boneCap.triangles.size()*6 + _boneCap.perimeter.size()*6 + 14 );
    std::vector<PLuint>    indices;      indices.reserve( _boneCap.triangles.size()*3 + _boneCap.perimeter.size() * 9 ); 
    
    // generate surface vertices for cap
    for (PLuint i = 0; i < _boneCap.triangles.size(); i++)
    {
        PLint base = vertices.size()/2;
        
        const plVector3 &p0 = _boneCap.triangles[i].point0();
        const plVector3 &p1 = _boneCap.triangles[i].point1();
        const plVector3 &p2 = _boneCap.triangles[i].point2();
        const plVector3 &n  = _boneCap.triangles[i].normal();
        
        vertices.emplace_back( plVector3( p0.x, p0.y+0.01f, p0.z) ); // position
        vertices.push_back( n );                                     // normal

        vertices.emplace_back( plVector3( p1.x, p1.y+0.01f, p1.z) ); // position
        vertices.push_back( n );                                     // normal
        
        vertices.emplace_back( plVector3( p2.x, p2.y+0.01f, p2.z) ); // position
        vertices.push_back( n );                                     // normal

        indices.push_back(base+0);
        indices.push_back(base+1); 
        indices.push_back(base+2);   
    }
    
    // generate cylinder walls
    plVector3 centreBottom = -(_length) * y;
    plVector3 z(0,0,1);
    plVector3 x(1,0,0);

    if (_boneCap.perimeter.size() > 0) 
    {
        PLint base = vertices.size()/2;

        float theta = _boneCap.perimeter[0].angle;
        plVector3 n = (cos(theta) * z + sin(theta) * x).normalize();

        plVector3 prevTop = _boneCap.perimeter[0].point;
        plVector3 prevBot = centreBottom + _radius * cos(theta) * z + _radius * sin(theta) * x;

        // top side
        vertices.push_back( prevTop ); // position
        vertices.push_back( n);        // normal
        // bottom side
        vertices.push_back( prevBot ); // position
        vertices.push_back( n);        // normal
        // bottom bottom
        vertices.push_back( prevBot ); // position 
        vertices.push_back( -y);       // normal
        
        for (PLuint i=0; i<_boneCap.perimeter.size(); i++) 
        {
            float theta = _boneCap.perimeter[i].angle;
            plVector3 top = _boneCap.perimeter[i].point;
            plVector3 bot = centreBottom + _radius * cos(theta) * z + _radius * sin(theta) * x;

            plVector3 n = (cos(theta) * z + sin(theta) * x).normalize();
            // top side
            vertices.push_back( top ); // position
            vertices.push_back( n);    // normal
            // bottom side    
            vertices.push_back( bot ); // position
            vertices.push_back( n);    // normal
            // bottom bottom
            vertices.push_back( bot ); // position 
            vertices.push_back( -y);   // normal
        }
        // top side
        vertices.push_back( prevTop ); // position
        vertices.push_back( n);        // normal
        // bottom side
        vertices.push_back( prevBot ); // position
        vertices.push_back( n);        // normal
        // bottom bottom
        vertices.push_back( prevBot ); // position 
        vertices.push_back( -y);       // normal
        // bottom centre point
        vertices.push_back( centreBottom );   // position
        vertices.push_back( -y );             // normal  
        
        for (PLuint j = 0; j <= _boneCap.perimeter.size()*3; j+=3) 
        {
            // side t1
            indices.push_back(base+j);
            indices.push_back(base+j+1);
            indices.push_back(base+j+3);
            // side t2
            indices.push_back(base+j+1);
            indices.push_back(base+j+4);
            indices.push_back(base+j+3);
            // bottom t3
            indices.push_back(base+j+2);
            indices.push_back(vertices.size()/2 - 1);
            indices.push_back(base+j+5);
        }

    }

    // set vbo and attach attribute pointers
    std::shared_ptr<plVBO> vbo( new plVBO() );
    vbo->set( vertices );
    vbo->set( plVertexAttributePointer( PL_POSITION_ATTRIBUTE, 0  ) );
    vbo->set( plVertexAttributePointer( PL_NORMAL_ATTRIBUTE,   16 ) );
    // set eabo
    std::shared_ptr<plEABO> eabo( new plEABO() );     
    eabo->set( indices );
    // attach to vao
    _boneVAO.attach( vbo );
    _boneVAO.attach( eabo );
    // upload to gpu
    _boneVAO.upload(); 
}


void plGraft::_findCap( plCap &cap, const plModel &model )
{   
    // clear previous cap incase
    cap.triangles.clear();
    cap.perimeter.clear();

    plSet<const plTriangle*> triangles;
    model.mesh().octree().rayIntersect( triangles, _harvest.transform().origin(), _harvest.transform().y(), _radius );
    
    // reserve for max number of triangles
    cap.triangles.reserve( triangles.size() );
    
    // iterate through and add triangles
    for ( const plTriangle* tri : triangles )
    {
        // check if triangle is on cap
        _triangleIntersection( cap, *tri );
    } 

    // calc just under radius
    PLfloat subSquaredRadius = 0.999f * _radius * _radius;

    // find vertices of polygons along perimeter of graft, put in set to ignore duplicates
    plSet<plPointAndAngle> angles;
  
    for (PLuint i=0; i<cap.triangles.size(); i++) 
    {
        const plVector3 &p0 = cap.triangles[i].point0();
        const plVector3 &p1 = cap.triangles[i].point1();
        const plVector3 &p2 = cap.triangles[i].point2();
        
        // if point is very close to radius, assume perimeter point
        if ( (p0.x*p0.x + p0.z*p0.z) > subSquaredRadius) 
            angles.insert( plPointAndAngle( atan2( p0.x, p0.z ), p0 ) );
            
        if ( (p1.x*p1.x + p1.z*p1.z) > subSquaredRadius) 
            angles.insert( plPointAndAngle( atan2( p1.x, p1.z ), p1 ) );
            
        if ( (p2.x*p2.x + p2.z*p2.z) > subSquaredRadius) 
            angles.insert( plPointAndAngle( atan2( p2.x, p2.z ), p2 ) );
  
    }   
    
    if (angles.size() > 0)
    {       
        cap.perimeter.reserve( angles.size() );  // reserve size
        
        for ( const plPointAndAngle& angle : angles )
        {
            cap.perimeter.push_back( angle );
        }
    }


}


bool plGraft::_isBeyondHeightThresholds( const plVector3 &p0, const plVector3 &p1, const plVector3 &p2 ) const
{
    const PLfloat VERTICAL_THRESHOLD = 8.0f;
    float proj0 = _harvest.transform().projectedDistOnAxis( p0 );
    float proj1 = _harvest.transform().projectedDistOnAxis( p1 );
    float proj2 = _harvest.transform().projectedDistOnAxis( p2 );

    float maxProj = PL_MAX_OF_3( proj0, proj1, proj2 );
    float minProj = PL_MIN_OF_3( proj0, proj1, proj2 );
      
    return (minProj > VERTICAL_THRESHOLD || maxProj < -VERTICAL_THRESHOLD);  
}


std::vector<plVector3> plGraft::_pointsOutsideTriangles( plVector3 *verts, PLfloat radiusSquared ) const
{
    // closest points on each segment
    plVector3 e[3];
    e[0] = plMath::closestPointOnSegment( plVector3(0, 0, 0), verts[0], verts[1] );
    e[1] = plMath::closestPointOnSegment( plVector3(0, 0, 0), verts[1], verts[2] );
    e[2] = plMath::closestPointOnSegment( plVector3(0, 0, 0), verts[2], verts[0] );
  
    // distances from each point to centre
    float d[3];
    d[0] = _harvest.transform().squaredDistToAxis( e[0] ); 
    d[1] = _harvest.transform().squaredDistToAxis( e[1] ); 
    d[2] = _harvest.transform().squaredDistToAxis( e[2] ); 
    
    // indexs of inside points
    std::vector<PLuint> insideEdges;
    if ( d[0] < radiusSquared ) insideEdges.push_back( 0 );
    if ( d[1] < radiusSquared ) insideEdges.push_back( 1 );
    if ( d[2] < radiusSquared ) insideEdges.push_back( 2 );
    
    std::vector<plVector3> points;      points.reserve(4);
    
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

            points.emplace_back( _pointOnCircumference( m0, m0p1 ) );
            points.emplace_back( _pointOnCircumference( m1, m0 ) );
            points.emplace_back( _pointOnCircumference( m2, m0 ) );
            points.emplace_back( _pointOnCircumference( m0p0, m0 ) );
            
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
            
            points.emplace_back( _pointOnCircumference( m0, m0p1 ) );
            points.emplace_back( _pointOnCircumference( m1p0, m1 ) );
            points.emplace_back( _pointOnCircumference( m1, m1p1 ) );
            points.emplace_back( _pointOnCircumference( m0p0, m0 ) ); 
            
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

std::vector<plVector3> plGraft::_pointsInsideTriangles( plVector3 *verts, PLfloat *dist, PLfloat radiusSquared ) const
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

    std::vector<plVector3> points;      points.reserve(5);
    
    for ( int i=0; i<3; i++ ) 
    {
        int j = (i+1) % 3;		// vertex at next end of edge

        bool currentInside = (dist[j] <= radiusSquared);

        if ( prevInside && currentInside ) 
        {
            // Add inside triangle point
            points.push_back( verts[j] );
        } 
        else if ( prevInside && !currentInside ) 
        {
            // Find point on edge of graft
            points.emplace_back( _pointOnCircumference( verts[i], verts[j] ) );
        } 
        else if ( !prevInside && currentInside ) 
        {
            // Find entering point and angle 
            points.emplace_back( _pointOnCircumference( verts[i], verts[j] ) );
            // Add inside triangle point    
            points.push_back( verts[j] );
        }
        else if ( !prevInside && !currentInside )
        {
            // check closest point on edge to see if it crosses into graft
            plVector3 m = plMath::closestPointOnSegment ( plVector3(0, 0, 0), verts[i], verts[j] );
            float     d = _harvest.transform().squaredDistToAxis( m ); 
            if ( d < radiusSquared )
            {
                // inside
                points.emplace_back( _pointOnCircumference( verts[i], m ) );
                points.emplace_back( _pointOnCircumference( m, verts[j] ) );
            }
        }

        prevInside = currentInside; // update prev with current
    }
    
    return points;
}

bool plGraft::_triangleIntersection( plCap &cap, const plTriangle &triangle ) const
{
    // if triangle is overlapping cap, cut it (if necessary) and add it to cap triangle list
    if (triangle.normal() * _harvest.transform().y() < 0)
        return false;

    // get squared radius
    float radiusSquared = _radius * _radius;

    // get triangle verts relative to graft local coordinate system
    plVector3 verts[3];
    verts[0] = _harvest.transform().applyInverse( triangle.point0() );
    verts[1] = _harvest.transform().applyInverse( triangle.point1() );
    verts[2] = _harvest.transform().applyInverse( triangle.point2() );
    
    // if too far above or below graft origin, reject.
    if ( _isBeyondHeightThresholds( verts[0], verts[1], verts[2] ) ) 
        return false;
    
    // Compute distance to graft axis
    float dist[3];
    dist[0] = _harvest.transform().squaredDistToAxis( verts[0] );
    dist[1] = _harvest.transform().squaredDistToAxis( verts[1] );
    dist[2] = _harvest.transform().squaredDistToAxis( verts[2] );
          
    float minDist = PL_MIN_OF_3( dist[0], dist[1], dist[2] );
    float maxDist = PL_MAX_OF_3( dist[0], dist[1], dist[2] );
       
    // At least some of the triangle is inside
    plVector3 normal = _harvest.transform().applyNormalInverse( triangle.normal() );   
       
    // if all points of triangle are withing radius, accept whole triangle, exit early
    if ( maxDist <= radiusSquared ) 
    {
        cap.triangles.push_back( plTriangle( normal, verts[0], verts[1], verts[2] ) );
        return true;
    }  
    
    std::vector<plVector3> points;      points.reserve(6);
    
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
    cap.triangles.emplace_back( plTriangle( normal, points[0], points[1], points[2] ) );
    
    if ( points.size() >= 4 )
    {
        // if there are 4 points, create a second triangle
        cap.triangles.emplace_back( plTriangle( normal, points[0], points[2], points[3] ) );
    }
    if ( points.size() == 5 )
    {
        // if there are 5 points, create a third triangle
        cap.triangles.emplace_back( plTriangle( normal, points[0], points[3], points[4] ) );
    }
    return true;
}


plVector3 plGraft::_pointOnCircumference( const plVector3 &u, const plVector3 &v ) const
{
    plVector3 uProj(u.x, 0.0f, u.z ); // ignore component along y axis
    plVector3 vProj(v.x, 0.0f, v.z );

    plVector3 uvProj = vProj-uProj;

    float a = uvProj * uvProj;
    float b = 2*( uProj * uvProj );
    float c = (uProj * uProj)-(_radius * _radius);

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


void plGraft::_updateCartilageThickness()
{
    // intersect cartilage and bone
    plIntersection boneIntersection = _harvest.model().bone.mesh().rayIntersect     ( _harvest.transform().origin(), -_harvest.transform().y() );  
    plIntersection cartIntersection = _harvest.model().cartilage.mesh().rayIntersect( _harvest.transform().origin(), -_harvest.transform().y() );  

    if ( boneIntersection.exists )
    {     
        // calculate new cartilage thickness
        _cartilageThickness = ( !cartIntersection.exists ) ? 0.0f : ( boneIntersection.point - cartIntersection.point ).length();        
    } 
}

/*
void plGraft::spinMark( PLfloat degrees )
{
    plVector3 axis(0,1,0);
    plMatrix44 rot; rot.setRotationD( degrees, axis );     
    _markDirection = (rot * _markDirection).normalize();    
    _updateMarkPosition();
}
*/

void plGraft::setMark( const plVector3 &direction )
{   
    _markDirection = direction.normalize();    
    _updateMarkPosition();
}


const plTransform& plGraft::transform( PLuint type ) const
{
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:      return _harvest.transform();        
        case PL_PICKING_INDEX_GRAFT_DEFECT:     return _recipient.transform();
            
        default:
        
            std::cerr << "plGraft transform()() error: invalid type enumeration provided, defaulting to recipient \n";
            return _recipient.transform();   
    } 
}


const plPlug &plGraft::plug( PLuint type ) const
{
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:      return _harvest;        
        case PL_PICKING_INDEX_GRAFT_DEFECT:     return _recipient;
            
        default:
        
            std::cerr << "plGraft plug() error: invalid type enumeration provided, defaulting to recipient \n";
            return _recipient;   
    } 
}

void plGraft::move( PLuint type, const plVector3 &origin, const plVector3 &y )
{
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:
        
            _harvest.move( origin, y );            
            _setCaps();
            break;
        
        case PL_PICKING_INDEX_GRAFT_DEFECT:
        
            _recipient.move( origin, y );
            break;
            
        default:
        
            std::cerr << "plGraft translate() error: invalid type enumeration provided \n";
            break;    
    } 
}


/*
void plGraft::translate( PLuint type, const plVector3 &translation )
{       
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:
        
            _harvest.translate( translation );            
            _setCaps();
            break;
        
        case PL_PICKING_INDEX_GRAFT_DEFECT:
        
            _recipient.translate( translation );
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
        
            _harvest.translateX( distance, planeNormal );
            _setCaps();
            break;
        
        case PL_PICKING_INDEX_GRAFT_DEFECT:
        
            _recipient.translateX( distance, planeNormal );
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
        
            _harvest.translateZ( distance, planeNormal );
            _setCaps();
            break;
        
        case PL_PICKING_INDEX_GRAFT_DEFECT:
        
            _recipient.translateZ( distance, planeNormal );
            break;
            
        default:
        
            std::cerr << "plGraft translateZ() error: invalid type enumeration provided \n";
            break;    
    } 
}
*/

void plGraft::rotate( PLuint type, const plVector3 &axis, PLfloat angleDegrees )
{    
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:
        
            _harvest.rotate( axis, angleDegrees );
            _setCaps();
            break;
        
        case PL_PICKING_INDEX_GRAFT_DEFECT:
        
            _recipient.rotate( axis, angleDegrees );
            break;
            
        default:
        
            std::cerr << "plGraft rotate() error: invalid type enumeration provided \n";
            break;    
    } 
}


