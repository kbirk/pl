#include "plGraftCap.h"


plGraftCap::plGraftCap()
{
}


void plGraftCap::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    plPickingStack::loadRed( PL_PICKING_TYPE_GRAFT ); 
    plColourStack::load( _getColour() );

    // if empty, do not draw
    if ( triangles.empty() )
        return;

    // create render component
    plRenderComponent component( _vao );
    // attached uniforms
    component.attach( plUniform( PL_MODEL_MATRIX_UNIFORM,      plModelStack::top()      ) );
    component.attach( plUniform( PL_VIEW_MATRIX_UNIFORM,       plCameraStack::top()     ) );
    component.attach( plUniform( PL_PROJECTION_MATRIX_UNIFORM, plProjectionStack::top() ) );
    component.attach( plUniform( PL_COLOUR_UNIFORM,            plColourStack::top()     ) ); 
    component.attach( plUniform( PL_PICKING_UNIFORM,           plPickingStack::top()    ) );
    component.attach( plUniform( PL_LIGHT_POSITION_UNIFORM,    plVector3( PL_LIGHT_POSITION ) ) ); 
    // insert into render map     
    renderMap[ technique ].insert( component );   
}


void plGraftCap::extractRenderComponents( plRenderMap& renderMap ) const
{
    extractRenderComponents( renderMap, PL_PLAN_TECHNIQUE );
}

/*
void plGraftCap::extractEditorRenderComponents( plRenderMap& renderMap ) const
{
    plPickingStack::loadRed( PL_PICKING_TYPE_GRAFT ); 
    
    // if empty, do not draw
    if ( triangles.empty() )
        return;

    // create render component
    plRenderComponent component( _vao );
    // attached uniforms
    component.attach( plUniform( PL_MODEL_MATRIX_UNIFORM,      plModelStack::top()      ) );
    component.attach( plUniform( PL_VIEW_MATRIX_UNIFORM,       plCameraStack::top()     ) );
    component.attach( plUniform( PL_PROJECTION_MATRIX_UNIFORM, plProjectionStack::top() ) );       
    component.attach( plUniform( PL_OUTLINE_UNIFORM,           plPickingStack::top()    ) );

    // insert into render map     
    renderMap[ PL_OUTLINE_TECHNIQUE ].insert( component );   
}
*/

void plGraftCap::generateCap( const plModel& model, const plTransform& transform, const PLfloat radius  )
{   
    // clear previous cap incase
    triangles.clear();
    perimeter.clear();

    plSet<const plTriangle*> potentialTriangles;
    model.mesh().octree().rayIntersect( potentialTriangles, transform.origin(), transform.y(), radius );
    
    // reserve for max number of triangles
    triangles.reserve( triangles.size() );
    
    // iterate through and add triangles
    for ( const plTriangle* tri : potentialTriangles )
    {
        // check if triangle is on cap
        _triangleIntersection( *tri, transform, radius );
    } 

    // calc just under radius
    PLfloat subSquaredRadius = 0.999f * radius * radius;

    // find vertices of polygons along perimeter of graft, put in set to ignore duplicates
    plSet<plPointAndAngle> angles;
  
    for (PLuint i=0; i<triangles.size(); i++) 
    {
        const plVector3 &p0 = triangles[i].point0();
        const plVector3 &p1 = triangles[i].point1();
        const plVector3 &p2 = triangles[i].point2();
        
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
        perimeter.reserve( angles.size() );  // reserve size
        
        for ( const plPointAndAngle& angle : angles )
        {
            perimeter.push_back( angle );
        }
    }


}


bool plGraftCap::_isBeyondHeightThresholds( const plVector3 &p0, const plVector3 &p1, const plVector3 &p2, const plTransform& transform ) const
{
    const PLfloat VERTICAL_THRESHOLD = 8.0f;
    float proj0 = transform.projectedDistOnAxis( p0 );
    float proj1 = transform.projectedDistOnAxis( p1 );
    float proj2 = transform.projectedDistOnAxis( p2 );

    float maxProj = PL_MAX_OF_3( proj0, proj1, proj2 );
    float minProj = PL_MIN_OF_3( proj0, proj1, proj2 );
      
    return (minProj > VERTICAL_THRESHOLD || maxProj < -VERTICAL_THRESHOLD);  
}


std::vector<plVector3> plGraftCap::_pointsOutsideTriangles( plVector3 verts[3], const plTransform& transform, PLfloat radius ) const
{
    float radiusSquared = radius * radius;

    // closest points on each segment
    plVector3 e[3];
    e[0] = plMath::closestPointOnSegment( plVector3(0, 0, 0), verts[0], verts[1] );
    e[1] = plMath::closestPointOnSegment( plVector3(0, 0, 0), verts[1], verts[2] );
    e[2] = plMath::closestPointOnSegment( plVector3(0, 0, 0), verts[2], verts[0] );
  
    // distances from each point to centre
    float d[3];
    d[0] = transform.squaredDistToAxis( e[0] ); 
    d[1] = transform.squaredDistToAxis( e[1] ); 
    d[2] = transform.squaredDistToAxis( e[2] ); 
    
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

            points.emplace_back( _pointOnCircumference( m0, m0p1, radius ) );
            points.emplace_back( _pointOnCircumference( m1, m0,   radius ) );
            points.emplace_back( _pointOnCircumference( m2, m0,   radius ) );
            points.emplace_back( _pointOnCircumference( m0p0, m0, radius ) );
            
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
            
            points.emplace_back( _pointOnCircumference( m0, m0p1, radius ) );
            points.emplace_back( _pointOnCircumference( m1p0, m1, radius ) );
            points.emplace_back( _pointOnCircumference( m1, m1p1, radius ) );
            points.emplace_back( _pointOnCircumference( m0p0, m0, radius ) ); 
            
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

std::vector<plVector3> plGraftCap::_pointsInsideTriangles( plVector3 verts[3], PLfloat dist[3], const plTransform& transform, PLfloat radius ) const
{
    float radiusSquared = radius * radius;

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
            points.emplace_back( _pointOnCircumference( verts[i], verts[j], radius ) );
        } 
        else if ( !prevInside && currentInside ) 
        {
            // Find entering point and angle 
            points.emplace_back( _pointOnCircumference( verts[i], verts[j], radius ) );
            // Add inside triangle point    
            points.push_back( verts[j] );
        }
        else if ( !prevInside && !currentInside )
        {
            // check closest point on edge to see if it crosses into graft
            plVector3 m = plMath::closestPointOnSegment ( plVector3(0, 0, 0), verts[i], verts[j] );
            float     d = transform.squaredDistToAxis( m ); 
            if ( d < radiusSquared )
            {
                // inside
                points.emplace_back( _pointOnCircumference( verts[i], m, radius ) );
                points.emplace_back( _pointOnCircumference( m, verts[j], radius ) );
            }
        }

        prevInside = currentInside; // update prev with current
    }
    
    return points;
}

bool plGraftCap::_triangleIntersection( const plTriangle &triangle, const plTransform& transform, PLfloat radius )
{
    // if triangle is overlapping cap, cut it (if necessary) and add it to cap triangle list
    if (triangle.normal() * transform.y() < 0)
        return false;

    // get squared radius
    float radiusSquared = radius * radius;

    // get triangle verts relative to graft local coordinate system
    plVector3 verts[3];
    verts[0] = transform.applyInverse( triangle.point0() );
    verts[1] = transform.applyInverse( triangle.point1() );
    verts[2] = transform.applyInverse( triangle.point2() );
    
    // if too far above or below graft origin, reject.
    if ( _isBeyondHeightThresholds( verts[0], verts[1], verts[2], transform ) ) 
        return false;
    
    // Compute distance to graft axis
    float dist[3];
    dist[0] = transform.squaredDistToAxis( verts[0] );
    dist[1] = transform.squaredDistToAxis( verts[1] );
    dist[2] = transform.squaredDistToAxis( verts[2] );
          
    float minDist = PL_MIN_OF_3( dist[0], dist[1], dist[2] );
    float maxDist = PL_MAX_OF_3( dist[0], dist[1], dist[2] );
       
    // At least some of the triangle is inside
    plVector3 normal = transform.applyNormalInverse( triangle.normal() );   
       
    // if all points of triangle are withing radius, accept whole triangle, exit early
    if ( maxDist <= radiusSquared ) 
    {
        triangles.push_back( plTriangle( normal, verts[0], verts[1], verts[2] ) );
        return true;
    }  
    
    std::vector<plVector3> points;      points.reserve(6);
    
    if (minDist > radiusSquared)
    {  
        // all points outside of triangle, triangle may overlap  
        points = _pointsOutsideTriangles( verts, transform, radius ); 
    }
    else
    {
        // at least one point is inside triangle
        points = _pointsInsideTriangles( verts, dist, transform, radius );    
    }
    
    if ( points.size() == 0 )
        return false;

    // add first triangle
    triangles.emplace_back( plTriangle( normal, points[0], points[1], points[2] ) );
    
    if ( points.size() >= 4 )
    {
        // if there are 4 points, create a second triangle
        triangles.emplace_back( plTriangle( normal, points[0], points[2], points[3] ) );
    }
    if ( points.size() == 5 )
    {
        // if there are 5 points, create a third triangle
        triangles.emplace_back( plTriangle( normal, points[0], points[3], points[4] ) );
    }
    return true;
}


plVector3 plGraftCap::_pointOnCircumference( const plVector3 &u, const plVector3 &v, PLfloat radius ) const
{
    plVector3 uProj(u.x, 0.0f, u.z ); // ignore component along y axis
    plVector3 vProj(v.x, 0.0f, v.z );

    plVector3 uvProj = vProj-uProj;

    float a = uvProj * uvProj;
    float b = 2*( uProj * uvProj );
    float c = (uProj * uProj)-(radius * radius);

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


plCartilageCap::plCartilageCap()
{
}


plVector4 plCartilageCap::_getColour() const
{
    if ( plPickingStack::topBlue() == PL_PICKING_INDEX_GRAFT_DEFECT )
    {
        return plVector4( PL_GRAFT_DEFECT_CARTILAGE_COLOUR );
    }
    else
    {
        return plVector4( PL_GRAFT_DONOR_CARTILAGE_COLOUR );
    }    
}

/*
void plCartilageCap::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    plPickingStack::loadRed( PL_PICKING_TYPE_GRAFT ); 
    plColourStack::load( _getColour() );

    // if empty, do not draw
    if ( triangles.empty() )
        return;

    // create render component
    plRenderComponent component( _projectionVAO );
    // attached uniforms
    component.attach( plUniform( PL_MODEL_MATRIX_UNIFORM,      plModelStack::top()      ) );
    component.attach( plUniform( PL_VIEW_MATRIX_UNIFORM,       plCameraStack::top()     ) );
    component.attach( plUniform( PL_PROJECTION_MATRIX_UNIFORM, plProjectionStack::top() ) );
    component.attach( plUniform( PL_COLOUR_UNIFORM,            plColourStack::top()     ) ); 
    component.attach( plUniform( PL_PICKING_UNIFORM,           plPickingStack::top()    ) );
    component.attach( plUniform( PL_LIGHT_POSITION_UNIFORM,    plVector3( PL_LIGHT_POSITION ) ) ); 
    // insert into render map     
    renderMap[ technique ].insert( component );   
}
*/

void plCartilageCap::generateVAO( PLfloat radius, PLfloat length, const std::vector<plPointAndAngle>& bonePerimeter )
{
    const plVector3 y(0,1,0);		        // y is cylinder axis (pointing upward)

    std::vector<plVector3> vertices;     vertices.reserve( triangles.size()*6 + ( perimeter.size() + bonePerimeter.size() )*6 );
    std::vector<PLuint>    indices;      indices.reserve( triangles.size()*3 + ( perimeter.size() + bonePerimeter.size() )*3 );
    
    for (PLuint i = 0; i < triangles.size(); i++)
    {
        PLint base = vertices.size()/2;
        
        const plVector3 &p0 = triangles[i].point0();
        const plVector3 &p1 = triangles[i].point1();
        const plVector3 &p2 = triangles[i].point2();
        const plVector3 &n  = triangles[i].normal();
        
        vertices.push_back( plVector3( p0.x, p0.y+0.05f, p0.z) );    // position
        vertices.push_back( n );                                     // normal

        vertices.push_back( plVector3( p1.x, p1.y+0.05f, p1.z) );    // position
        vertices.push_back( n );                                     // normal
        
        vertices.push_back( plVector3( p2.x, p2.y+0.05f, p2.z) );    // position
        vertices.push_back( n );                                     // normal

        indices.push_back(base+0);
        indices.push_back(base+1); 
        indices.push_back(base+2);   
    }
    
    // cartilage walls
    if ( bonePerimeter.size() > 0 && perimeter.size() > 0 ) 
    {
        int c = 0;
        int b = 0;

        float cOffset = 0;
        float bOffset = 0;

        int stepsLeft = perimeter.size() + bonePerimeter.size();

        while (stepsLeft > 0) 
        {
            float cAngle = perimeter[c].angle + cOffset;
            float bAngle = bonePerimeter[b].angle + bOffset;
        
            plVector3 n = (perimeter[c].point).normalize();
    
            indices.push_back(vertices.size()/2);
            vertices.push_back( perimeter[c].point );   // position
            vertices.push_back( n );                    // normal
            
            indices.push_back(vertices.size()/2);
            vertices.push_back( bonePerimeter[b].point );        // position
            vertices.push_back( n );                             // normal
        
            if (cAngle < bAngle) 
            {	
                // advance cartilage
                c++;
                if (c == perimeter.size()) 
                {
                    c = 0;
                    cOffset = 2 * PL_PI;
                }
        
                indices.push_back(vertices.size()/2);
                vertices.push_back( perimeter[c].point );    // position
                vertices.push_back( n );                                  // normal
            }  
            else 
            {			
                // advance bone
                b++;
                if (b == bonePerimeter.size()) 
                {
                    b = 0;
                    bOffset = 2 * PL_PI;
                }
        
                indices.push_back(vertices.size()/2);
                vertices.push_back( bonePerimeter[b].point );        // position
                vertices.push_back( n );                                  // normal
            }
            stepsLeft--;
        }
    }

    if ( indices.size() > 0 )
    {   
        // set vbo and attach attribute pointers
        std::shared_ptr< plVBO > vbo = std::make_shared< plVBO >();
        vbo->set( vertices );
        vbo->set( plVertexAttributePointer( PL_POSITION_ATTRIBUTE, 0  ) );
        vbo->set( plVertexAttributePointer( PL_NORMAL_ATTRIBUTE,   16 ) );
        // set eabo
        std::shared_ptr<plEABO> eabo = std::make_shared< plEABO >();   
        eabo->set( indices );
        // create vao, attach eabo and vbo, upload to gpu
        _vao = std::make_shared< plVAO >();
        _vao->attach( vbo );
        _vao->attach( eabo );
        // upload to gpu
        _vao->upload(); 
        
        _generateProjectionVAO( radius, length, bonePerimeter );
    }
}


void plCartilageCap::_generateProjectionVAO( PLfloat radius, PLfloat length, const std::vector<plPointAndAngle>& bonePerimeter )
{

    const plVector3 y(0,1,0);		        // y is cylinder axis (pointing upward)

    std::vector<plVector3> vertices;     vertices.reserve( triangles.size()*6 + ( perimeter.size() + bonePerimeter.size() )*6 );
    std::vector<PLuint>    indices;      indices.reserve( triangles.size()*3 + ( perimeter.size() + bonePerimeter.size() )*3 );
    
    for (PLuint i = 0; i < triangles.size(); i++)
    {
        PLint base = vertices.size()/2;
        
        const plVector3 &p0 = triangles[i].point0();
        const plVector3 &p1 = triangles[i].point1();
        const plVector3 &p2 = triangles[i].point2();
        const plVector3 &n  = triangles[i].normal();
        
        vertices.push_back( plVector3( p0.x, p0.y+0.05f, p0.z) );    // position
        vertices.push_back( n );                                     // normal

        vertices.push_back( plVector3( p1.x, p1.y+0.05f, p1.z) );    // position
        vertices.push_back( n );                                     // normal
        
        vertices.push_back( plVector3( p2.x, p2.y+0.05f, p2.z) );    // position
        vertices.push_back( n );                                     // normal

        indices.push_back(base+0);
        indices.push_back(base+1); 
        indices.push_back(base+2);   
    }
    
    if ( perimeter.size() > 0 ) 
    {
        PLfloat PROJECTION_LENGTH = 10;
    
        // generate cylinder walls
        plVector3 centreBottom = ( PROJECTION_LENGTH ) * y;
        plVector3 z(0,0,1);
        plVector3 x(1,0,0);
    
        PLint base = vertices.size()/2;

        float theta = perimeter[0].angle;
        plVector3 n = (cos(theta) * z + sin(theta) * x).normalize();

        plVector3 prevTop = perimeter[0].point;
        plVector3 prevBot = centreBottom + radius * cos(theta) * z + radius * sin(theta) * x;

        // top side
        vertices.push_back( prevTop ); // position
        vertices.push_back( n);        // normal
        // bottom side
        vertices.push_back( prevBot ); // position
        vertices.push_back( n);        // normal
        
        for ( PLuint i=0; i< perimeter.size(); i++ ) 
        {
            float theta = perimeter[i].angle;
            plVector3 top = perimeter[i].point;
            plVector3 bot = centreBottom + radius * cos(theta) * z + radius * sin(theta) * x;

            plVector3 n = (cos(theta) * z + sin(theta) * x).normalize();
            // top side
            vertices.push_back( top ); // position
            vertices.push_back( n);    // normal
            // bottom side    
            vertices.push_back( bot ); // position
            vertices.push_back( n);    // normal
        }
        
        // top side
        vertices.push_back( prevTop ); // position
        vertices.push_back( n);        // normal
        // bottom side
        vertices.push_back( prevBot ); // position
        vertices.push_back( n);        // normal    
        
        for (PLuint j = 0; j <= perimeter.size()*2; j+=2) 
        {
            // side t1
            indices.push_back(base+j);
            indices.push_back(base+j+1);
            indices.push_back(base+j+2);
            // side t2
            indices.push_back(base+j+2);
            indices.push_back(base+j+1);
            indices.push_back(base+j+3);
        }

    }
    
    // set vbo and attach attribute pointers
    std::shared_ptr< plVBO > vbo = std::make_shared< plVBO >();
    vbo->set( vertices );
    vbo->set( plVertexAttributePointer( PL_POSITION_ATTRIBUTE, 0  ) );
    vbo->set( plVertexAttributePointer( PL_NORMAL_ATTRIBUTE,   16 ) );
    // set eabo
    std::shared_ptr<plEABO> eabo = std::make_shared< plEABO >();     
    eabo->set( indices );
    // create vao, attach eabo and vbo, upload to gpu
    _projectionVAO = std::make_shared< plVAO >();
    _projectionVAO->attach( vbo );
    _projectionVAO->attach( eabo );
    // upload to gpu
    _projectionVAO->upload(); 

}


plBoneCap::plBoneCap()
{
}


plVector4 plBoneCap::_getColour() const
{
    if ( plPickingStack::topBlue() == PL_PICKING_INDEX_GRAFT_DEFECT )
    {
        return plVector4( PL_GRAFT_DEFECT_BONE_COLOUR );
    }
    else
    {
        return plVector4( PL_GRAFT_DONOR_BONE_COLOUR );
    } 
}


void plBoneCap::generateVAO( PLfloat radius, PLfloat length )
{        
    const plVector3 y(0,1,0);		        // y is cylinder axis (pointing upward)

    std::vector<plVector3> vertices;     vertices.reserve( triangles.size()*6 + perimeter.size()*6 + 14 );
    std::vector<PLuint>    indices;      indices.reserve( triangles.size()*3 + perimeter.size() * 9 ); 
    
    // generate surface vertices for cap
    for (PLuint i = 0; i < triangles.size(); i++)
    {
        PLint base = vertices.size()/2;
        
        const plVector3 &p0 = triangles[i].point0();
        const plVector3 &p1 = triangles[i].point1();
        const plVector3 &p2 = triangles[i].point2();
        const plVector3 &n  = triangles[i].normal();
        
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
    plVector3 centreBottom = -( length ) * y;
    plVector3 z(0,0,1);
    plVector3 x(1,0,0);

    if ( perimeter.size() > 0 ) 
    {
        PLint base = vertices.size()/2;

        float theta = perimeter[0].angle;
        plVector3 n = (cos(theta) * z + sin(theta) * x).normalize();

        plVector3 prevTop = perimeter[0].point;
        plVector3 prevBot = centreBottom + radius * cos(theta) * z + radius * sin(theta) * x;

        // top side
        vertices.push_back( prevTop ); // position
        vertices.push_back( n);        // normal
        // bottom side
        vertices.push_back( prevBot ); // position
        vertices.push_back( n);        // normal
        // bottom bottom
        vertices.push_back( prevBot ); // position 
        vertices.push_back( -y);       // normal
        
        for ( PLuint i=0; i< perimeter.size(); i++ ) 
        {
            float theta = perimeter[i].angle;
            plVector3 top = perimeter[i].point;
            plVector3 bot = centreBottom + radius * cos(theta) * z + radius * sin(theta) * x;

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
        
        for (PLuint j = 0; j <= perimeter.size()*3; j+=3) 
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
    std::shared_ptr< plVBO > vbo = std::make_shared< plVBO >();
    vbo->set( vertices );
    vbo->set( plVertexAttributePointer( PL_POSITION_ATTRIBUTE, 0  ) );
    vbo->set( plVertexAttributePointer( PL_NORMAL_ATTRIBUTE,   16 ) );
    // set eabo
    std::shared_ptr<plEABO> eabo = std::make_shared< plEABO >();     
    eabo->set( indices );
    // create vao, attach eabo and vbo, upload to gpu
    _vao = std::make_shared< plVAO >();
    _vao->attach( vbo );
    _vao->attach( eabo );
    // upload to gpu
    _vao->upload(); 
}

