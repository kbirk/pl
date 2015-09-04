#include "plSpline.h"


plSpline::plSpline()
    : _lastUpdate( 0 )
{
}


plSpline::plSpline( const plMesh &mesh )
    :  _lastUpdate( 0 ), plBoundary( PL_PICKING_TYPE_DEFECT_SPLINE, mesh )
{
}


plSpline::plSpline( const plMesh &mesh, const std::vector<plString> &row )
    :  _lastUpdate( 0 ), plBoundary( PL_PICKING_TYPE_DEFECT_SPLINE, mesh, row )
{
    // construct spline
    if (size() == 4)
    {
        _computeHermite();
    }
}


void plSpline::clear()
{
    plBoundary::clear();
    _surfaceMesh = plMesh();
    _lastUpdate = 0;
}


PLuint plSpline::addPointAndNormal (const plVector3 &point, const plVector3 &normal)
{
    if (size() < 4)
    {
        PLint ret = plBoundary::addPointAndNormal( point, normal );

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
    plBoundary::removePointAndNormal( index );
}


void plSpline::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    if ( !_isVisible )
        return;

    // if not full 4 corners, display walls
    if ( size() < 4 )
    {
        // draw boundary walls
        plBoundary::extractRenderComponents( renderMap, technique );
    }
    else
    {
        plPickingStack::loadRed( PL_PICKING_TYPE_DEFECT_CORNERS );
        plColourStack::load( _getColour() );

        // draw points
        _extractPointRenderComponents( renderMap, technique );

        // draw spline
        plPickingStack::loadBlue( -1 );  // unused

        // set colour flag to use vertex attribute colours
        plColourStack::push( PL_USE_ATTRIBUTE_COLOUR_OPAQUE );

        // create render component
        plRenderComponent component( _surfaceVAO );
        // attached uniforms
        component.attach( plUniform( PL_MODEL_MATRIX_UNIFORM,      plMatrix44()             ) );
        component.attach( plUniform( PL_VIEW_MATRIX_UNIFORM,       plCameraStack::top()     ) );
        component.attach( plUniform( PL_PROJECTION_MATRIX_UNIFORM, plProjectionStack::top() ) );
        component.attach( plUniform( PL_COLOUR_UNIFORM,            plColourStack::top()     ) );
        component.attach( plUniform( PL_PICKING_UNIFORM,           plPickingStack::top()    ) );
        component.attach( plUniform( PL_LIGHT_POSITION_UNIFORM,    plVector3( PL_LIGHT_POSITION ) ) );
        // insert into render map
        renderMap[ technique ].insert( component );

        plColourStack::pop();
    }
}


void plSpline::extractRenderComponents( plRenderMap& renderMap ) const
{
    extractRenderComponents( renderMap, PL_PLAN_TECHNIQUE );
}


PLfloat Q( PLfloat s, PLfloat t, const std::vector<PLfloat> &st, const std::vector<PLfloat> &tt)
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


std::vector<plVector3> plSpline::_averageCornerNormals() const
{
    const PLfloat AVERAGE_RADIUS = 2.0f;

    std::vector<plVector3> cornerNormals;

    // compute averages normals
    cornerNormals.push_back( _mesh->getAverageNormal( AVERAGE_RADIUS, _points[0], _normals[0]) );
    cornerNormals.push_back( _mesh->getAverageNormal( AVERAGE_RADIUS, _points[1], _normals[1]) );
    cornerNormals.push_back( _mesh->getAverageNormal( AVERAGE_RADIUS, _points[2], _normals[2]) );
    cornerNormals.push_back( _mesh->getAverageNormal( AVERAGE_RADIUS, _points[3], _normals[3]) );

    return cornerNormals;
}


void plSpline::_computeTangents( std::vector<PLfloat> &st, std::vector<PLfloat> &tt, const std::vector<plVector3> &p, const std::vector<plVector3> &n ) const
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
    std::vector<plVector3> s, t;

    // normals of planes between each point
    plVector3 sn, tn;

    // p0 plane normals
    sn = ( p01 ^ -n01 ).normalize();
    tn = ( p03 ^ n03 ).normalize();

    s.push_back( plMath::projectVectorOnPlane( p03 ^ n[0], sn).normalize() );
    t.push_back( plMath::projectVectorOnPlane( p01 ^ -n[0], tn).normalize() ) ;

    // p1 plane normals
    //sn = ( p01 ^ -n01 ).normalize(); // redundant
    tn = ( p12 ^ n12 ).normalize();

    s.push_back( plMath::projectVectorOnPlane( p03 ^ n[1], sn).normalize() );
    t.push_back( plMath::projectVectorOnPlane( p01 ^ -n[1], tn).normalize() ) ;

    // p2 plane normals
    sn = ( p32 ^ -n32 ).normalize();
    //tn = ( p12 ^ n12 ).normalize();  // redundant

    s.push_back( plMath::projectVectorOnPlane( p12 ^ n[2], sn).normalize() );
    t.push_back( plMath::projectVectorOnPlane( p32 ^ -n[2], tn).normalize() ) ;

    // p3 plane normals
    //sn = ( p32 ^ -n32 ).normalize(); // redundant
    tn = ( p03 ^ n03 ).normalize();

    s.push_back( plMath::projectVectorOnPlane( p03 ^ n[3], sn).normalize() );
    t.push_back( plMath::projectVectorOnPlane( p32 ^ -n[3], tn).normalize() ) ;

    // find the slope of the line along the plane of the spline boundary wall
    // scale by the length between two points to ensure proper scaling
    st.push_back( (s[0]*n01) / (s[0]*p01) * (p[0]-p[1]).length() );
    tt.push_back( (t[0]*n03) / (t[0]*p03) * (p[0]-p[3]).length() );

    st.push_back( (s[1]*n01) / (s[1]*p01) * (p[0]-p[1]).length() );
    tt.push_back( (t[1]*n12) / (t[1]*p12) * (p[2]-p[1]).length() );

    st.push_back( (s[2]*n32) / (s[2]*p32) * (p[3]-p[2]).length() );
    tt.push_back( (t[2]*n12) / (t[2]*p12) * (p[2]-p[1]).length() );

    st.push_back( (s[3]*n32) / (s[3]*p32) * (p[3]-p[2]).length() );
    tt.push_back( (t[3]*n03) / (t[3]*p03) * (p[0]-p[3]).length() );

}


void plSpline::_computeHermite()
{
    // spline colour map computations are very intensive, to prevent unnecessary updates due to frequency of mouse events, add a limit
    if ( _timeSinceLastUpdate() < 5 )
        return;

    // p and n for cleaner code
    const std::vector<plVector3> &p = _points;
    const std::vector<plVector3> n  = _averageCornerNormals();

    // find tangents in the s and t planes
    std::vector<PLfloat> st, tt; _computeTangents( st, tt, p, n );

    std::vector<plTriangle> triangles;
    triangles.reserve( PL_SPLINE_RESOLUTION*PL_SPLINE_RESOLUTION*2 );

    std::vector<plVector3> points;      points.reserve( (PL_SPLINE_RESOLUTION+1)*(PL_SPLINE_RESOLUTION+1) );
    std::vector<plVector3> colours;     colours.reserve( (PL_SPLINE_RESOLUTION+1)*(PL_SPLINE_RESOLUTION+1) );

    std::vector<plVector3> vertices;    vertices.reserve( PL_SPLINE_RESOLUTION * PL_SPLINE_RESOLUTION * 6 * 3 );
    std::vector<PLuint>    indices;     indices.reserve ( PL_SPLINE_RESOLUTION * PL_SPLINE_RESOLUTION * 6 );

    for (PLuint j=0; j <= PL_SPLINE_RESOLUTION; j++)
    {
        PLfloat v = j/PL_SPLINE_RESOLUTION;

        // interpolate spline corner positions and normals for current v coord
        plVector3 n03 = (1.0f-v)*n[0] + v*n[3];
        plVector3 n12 = (1.0f-v)*n[1] + v*n[2];

        plVector3 p03 = (1.0f-v)*p[0] + v*p[3];
        plVector3 p12 = (1.0f-v)*p[1] + v*p[2];

        for ( PLuint i=0; i <= PL_SPLINE_RESOLUTION; i++ )
        {
            PLfloat u = i/PL_SPLINE_RESOLUTION;

            // first row, don't triangulate
            PLfloat z = Q( u, v, st, tt );

            // interpolate position and normal for current u coord
            plVector3 norm = ( (1.0f-u)*n03 + u*n12 ).normalize();
            plVector3 pos  = (1.0f-u)*p03 + u*p12 + z*norm;          // inflate this point using normal scaled by z value returned by hermite spline

            // intersect surface for distance map
            plIntersection intersection = _mesh->rayIntersect( pos, -norm, false, false, true );

            PLfloat distance = (intersection.point - pos ).squaredLength() / PL_SPLINE_COLOUR_MAP_RANGE;

            // get colour value
            plVector3 colour = ( distance <= PL_SPLINE_COLOUR_MAP_RANGE ) ? plColourMap::map( distance ) : PL_SPLINE_NO_DATA_COLOUR;

            points.push_back( pos );
            colours.push_back( colour );

            if ( j > 0 && i > 0 )
            {
                // once past first row, begin triangulating
                PLuint i0 = (i-1) + (j-1) * (PL_SPLINE_RESOLUTION+1);
                PLuint i1 = i0 + 1;
                PLuint i2 = i + j * (PL_SPLINE_RESOLUTION+1);
                PLuint i3 = i2 - 1;

                plVector3 normal = ( ( points[i2] - points[i1]) ^ (points[i0] - points[i1]) ).normalize();

                triangles.emplace_back( plTriangle( normal, points[i0], points[i1], points[i2] ) );
                triangles.emplace_back( plTriangle( normal, points[i0], points[i2], points[i3] ) );

                PLuint base = vertices.size() / 3;

                vertices.push_back( points[i0] );  vertices.push_back( normal );  vertices.push_back( colours[i0] );  // point 0
                vertices.push_back( points[i1] );  vertices.push_back( normal );  vertices.push_back( colours[i1] );  // point 1
                vertices.push_back( points[i2] );  vertices.push_back( normal );  vertices.push_back( colours[i2] );  // point 2
                vertices.push_back( points[i3] );  vertices.push_back( normal );  vertices.push_back( colours[i3] );  // point 3

                // triangle indices
                indices.push_back( base );   indices.push_back( base+1 );   indices.push_back( base+2 );
                indices.push_back( base );   indices.push_back( base+2 );   indices.push_back( base+3 );
            }
        }

    }

    _surfaceMesh = plMesh( triangles );

    // set vbo and attach attribute pointers
    std::shared_ptr< plVBO > vbo = std::make_shared< plVBO >();
    vbo->set( vertices );
    vbo->set( plVertexAttributePointer( PL_POSITION_ATTRIBUTE, 48, 0  ) );
    vbo->set( plVertexAttributePointer( PL_NORMAL_ATTRIBUTE,   48, 16 ) );
    vbo->set( plVertexAttributePointer( PL_COLOUR_ATTRIBUTE,   48, 32 ) );
    // set eabo
    std::shared_ptr<plEABO> eabo = std::make_shared< plEABO >();
    eabo->set( indices );
    // create vao, attach eabo and vbo, upload to gpu
    _surfaceVAO = std::make_shared< plVAO >();
    _surfaceVAO->attach( vbo );
    _surfaceVAO->attach( eabo );
    // upload to gpu
    _surfaceVAO->upload();

    // update timer to store time of last update
    _lastUpdate = plTimer::now();
}


PLtime plSpline::_timeSinceLastUpdate()
{
    return plTimer::now() - _lastUpdate;
}
