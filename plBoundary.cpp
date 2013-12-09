#include "plBoundary.h"


plBoundary::plBoundary()
    : _type( -1 )
{
}


plBoundary::plBoundary( PLuint type, const plMesh& mesh )
    : _type( type ), plMeshSpecific( mesh )
{

}

   
plBoundary::plBoundary( PLuint type, const plMesh& mesh, const std::vector<plString> &row )
    : _type( type ), plMeshSpecific( mesh )
{
    // assumes points are counter-clockwise
    for ( PLuint j = 1; j < row.size(); j+=2)
    {               
        _points.push_back ( plVector3( row[j]   ) );                  
        _normals.push_back( plVector3( row[j+1] ) );
    } 
    // construct mesh
    _generateVAO(); 
}


void plBoundary::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    if ( !_isVisible )
        return;

    plPickingStack::loadRed( _type );
    plColourStack::load( _getColour() );

    // draw walls
    if ( _points.size() > 1 )
    {
        plPickingStack::loadBlue( -1 ); // draw walls with index of -1
        
        // create render component
        plRenderComponent component( _vao );
        // attached uniforms
        component.attach( plUniform( PL_MODEL_MATRIX_UNIFORM,      plMatrix44()             ) );
        component.attach( plUniform( PL_VIEW_MATRIX_UNIFORM,       plCameraStack::top()     ) );
        component.attach( plUniform( PL_PROJECTION_MATRIX_UNIFORM, plProjectionStack::top() ) );
        component.attach( plUniform( PL_COLOUR_UNIFORM,            plColourStack::top()     ) ); 
        component.attach( plUniform( PL_PICKING_UNIFORM,           plPickingStack::top()    ) );
        component.attach( plUniform( PL_LIGHT_POSITION_UNIFORM,    plVector3( PL_LIGHT_POSITION ) ) );
        // insert into render map
        renderMap[ technique ].insert( component );            
    }
        
    // draw points
    _extractPointRenderComponents( renderMap, technique );
}


void plBoundary::extractRenderComponents( plRenderMap& renderMap ) const
{
    extractRenderComponents( renderMap, PL_PLAN_TECHNIQUE );
}


void plBoundary::_extractPointRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    // draw points
    for (PLuint i=0; i<_points.size(); i++)
    {
        plPickingStack::loadBlue( i );    
             
        if ( _isSelected && _selectedValue == i )   // is the current point selected?
        {
            // scale larger  
            plRenderer::queueSphere( technique, _points[i], PL_SELECTED_BOUNDARY_POINT_RADIUS );         
        }
        else
        {
            // regular size
            plRenderer::queueSphere( technique, _points[i], PL_BOUNDARY_POINT_RADIUS );  
        }
    } 
}


plVector3 plBoundary::getAverageNormal() const
{
    plVector3 n(0,0,0);
    for (PLuint i=0; i < _normals.size(); i++)
    {
        n = n + _normals[i];        
    }
    return n.normalize();
}


PLuint plBoundary::addPointAndNormal(const plVector3 &point, const plVector3 &normal)
{
    if (_points.size() < 2) 
    {
        // 0 or 1 _points, doesnt matter, just add
        _points.push_back( point );
        _normals.push_back( normal );
        _generateVAO();
        return _points.size()-1;
    }
    else if (_points.size() == 2) 
    {
        // 2 _points, ensure third is counter clockwise
        plVector3 n = 0.5f * (_normals[1] + _normals[0]);
        
        if ( ((_points[1] - _points[0]) ^ (point - _points[0])) * n > 0)
        {
            // already counter clockwise
            _points.push_back( point );
            _normals.push_back( normal );
            
            _generateVAO(); 
            return 2;
        }
        else
        {
            // clock-wise, add new point between existing two
            _points.insert ( _points.begin()+1, point );
            _normals.insert( _normals.begin()+1, normal );

            _generateVAO(); 
            return 1;
        }
    } 
    else 
    {    
        // Find the closest edge to the new point        
        PLfloat minDist = FLT_MAX;
        PLint shift_i = 0;

        const PLfloat EPSILON = 0.000001f;
               
        for (PLuint i = 0; i < _points.size(); i++)
        {
            PLint j = (i+1) % _points.size();   // next point
            // get closest point on current edge
            plVector3 closest = plMath::closestPointOnSegment(point, _points[i], _points[j]);
            // compare distance to current minimum distance
            PLfloat distSquared = (closest - point).squaredLength();
            
            // check if same point as previous edge, occurs when edges are at acute angles, making a pointy "v"
            if ( fabs(distSquared - minDist) < EPSILON )
            {
                // same point as previous edge,                
                int h = (i == 0) ? _points.size()-1 : i-1; // previous index                      
                // get current and previous wall segments        
                plVector3 previous_segment = (_points[i] - _points[h]); 
                plVector3 current_segment  = (_points[j] - _points[i]);     
                // get current wall tangent                        
                plVector3 current_tangent  = current_segment ^ (0.5f * (_normals[j] + _normals[i]));
                 
                // check if current "v" shape is pointing inwards or outwards 
                if ( (current_segment ^ previous_segment) * _normals[i] < 0)
                {
                    // outward pointing "v"
                    if ((point - closest) * current_tangent  > 0 )
                    {
                        // not behind previous edge
                        minDist = distSquared;
                        shift_i = j;
                    } 
                }
                else
                {
                    // inward pointing "v"
                    if ((point - closest) * current_tangent  < 0 )
                    {
                        // in front of current edge
                        minDist = distSquared;
                        shift_i = j;
                    }
                }
            }
            else if (distSquared < minDist)
            {
                // new minimum, store index
                minDist = distSquared;
                shift_i = j;
            }
        }

        _points.insert ( _points.begin()+shift_i, point );
        _normals.insert( _normals.begin()+shift_i, normal );

        _generateVAO(); 
        return shift_i;  
    }
}


void plBoundary::movePointAndNormal( PLuint index, const plVector3 &point, const plVector3 &normal)
{
    _points[index] = point;    
    _normals[index] = normal;
    _generateVAO(); 
}


void plBoundary::removePointAndNormal( PLuint index )
{

    _points.erase ( _points.begin()+index );
    _normals.erase( _normals.begin()+index );
    _generateVAO(); 
}


void plBoundary::clear()
{
    _points.clear();
    _normals.clear();
    _generateVAO(); 
}


plVector4 plBoundary::_getColour() const
{
    // not selected
    switch ( _type )
    {
        case PL_PICKING_TYPE_DEFECT_CORNERS:
        case PL_PICKING_TYPE_DEFECT_SPLINE:
            // defect spline / corners 
            return plVector4( PL_BOUNDARY_DEFECT_CORNER_COLOUR ); 

        case PL_PICKING_TYPE_DEFECT_BOUNDARY:
            // defect boundary
            return plVector4( PL_BOUNDARY_DEFECT_BOUNDARY_COLOUR ); 

        case PL_PICKING_TYPE_DONOR_BOUNDARY:
            // donor boundary
            return plVector4( PL_BOUNDARY_DONOR_COLOUR );

        case PL_PICKING_TYPE_IGUIDE_BOUNDARY: 
        default:
            // iguide boundary
            return plVector4( PL_BOUNDARY_IGUIDE_COLOUR );  
    }

}


void plBoundary::_generateVAO()
{
    if (_points.size() < 2)
        return;

    plVector3 n = getAverageNormal();

    /*
    std::vector< plVector3 > incrementalPoints;
    const float INCREMENT_LENGTH = 0.25f;
    for (PLuint i = 0; i < _points.size(); i++)
    {        
        int j = (i+1) % _points.size();            // next index

        incrementalPoints.push_back( _points[i] );

        plVector3 edge = _points[j] - _points[i];
        PLfloat sqrLength = edge.squaredLength();
        edge = edge.normalize();
        
        while ( sqrLength > INCREMENT_LENGTH )
        {
            plIntersection intersection = _model->combined.mesh().rayIntersect( incrementalPoints.back() + edge * INCREMENT_LENGTH, -n );
            
            if ( intersection.exists )
                incrementalPoints.push_back( intersection.point );

            sqrLength -= INCREMENT_LENGTH;
        }
    }
    */

    std::vector<plVector3> vertices;    vertices.reserve( _points.size() * 10 );
    std::vector<PLuint>    indices;     indices.reserve ( _points.size() * 6 * 4 );

    for (PLuint i = 0; i < _points.size(); i++)
    {        
        int j = (i+1) % _points.size();            // next index
        int k = (i+2) % _points.size();            // next next index
        int l = (i == 0) ? _points.size()-1 : i-1; // previous index

        // tangent vectors of previous, current, and next segments
        plVector3 t0 = ((_points[i] - _points[l]) ^ n).normalize(); // previous tangent 
        plVector3 t1 = ((_points[j] - _points[i]) ^ n).normalize(); // current tangent            
        plVector3 t2 = ((_points[k] - _points[j]) ^ n).normalize(); // next tangent

        plVector3 x0, x1;
        if (_points.size() == 2)
        {
            // single wall, only use 
            x0 = t1;
            x1 = t1;
        }
        else
        {
            // average tangent vectors of current segment end points
            x0 = (t0 + t1).normalize();
            x1 = (t1 + t2).normalize();
        }

        plVector3 a = _points[i] + PL_BOUNDARY_MESH_WIDTH_HALF * x0;
        plVector3 b = _points[j] + PL_BOUNDARY_MESH_WIDTH_HALF * x1;
        plVector3 c = b + PL_BOUNDARY_MESH_HEIGHT * n;
        plVector3 d = a + PL_BOUNDARY_MESH_HEIGHT * n;

        plVector3 e = _points[i] - PL_BOUNDARY_MESH_WIDTH_HALF * x0;
        plVector3 f = _points[j] - PL_BOUNDARY_MESH_WIDTH_HALF * x1;
        plVector3 g = f + PL_BOUNDARY_MESH_HEIGHT * n;
        plVector3 h = e + PL_BOUNDARY_MESH_HEIGHT * n;

        PLuint base = vertices.size()/2;

        // "right" quad
        vertices.push_back(a);    // position
        vertices.push_back(t1);   // normal
        
        vertices.push_back(b);    // position
        vertices.push_back(t1);   // normal
        
        vertices.push_back(c);    // position
        vertices.push_back(t1);   // normal
        
        vertices.push_back(d);    // position
        vertices.push_back(t1);   // normal
        
        // top ridge 
        vertices.push_back(0.5 * (d+h) + PL_BOUNDARY_MESH_CURVE_HEIGHT * n);    // position
        vertices.push_back(n);    // normal
        
        vertices.push_back(0.5 * (c+g) + PL_BOUNDARY_MESH_CURVE_HEIGHT * n);    // position
        vertices.push_back(n);    // normal       
         
        // "left" quad
        vertices.push_back(e);    // position
        vertices.push_back(-t1);  // normal
        
        vertices.push_back(f);    // position
        vertices.push_back(-t1);  // normal
        
        vertices.push_back(g);    // position
        vertices.push_back(-t1);  // normal
        
        vertices.push_back(h);    // position
        vertices.push_back(-t1);  // normal

        // triangle indices
        indices.push_back(base+0);    indices.push_back(base+1);    indices.push_back(base+2);
        indices.push_back(base+0);    indices.push_back(base+2);    indices.push_back(base+3);
        
        indices.push_back(base+3);    indices.push_back(base+2);    indices.push_back(base+5);
        indices.push_back(base+3);    indices.push_back(base+5);    indices.push_back(base+4);
        
        indices.push_back(base+4);    indices.push_back(base+5);    indices.push_back(base+8);
        indices.push_back(base+4);    indices.push_back(base+8);    indices.push_back(base+9);
        
        indices.push_back(base+7);    indices.push_back(base+6);    indices.push_back(base+9);
        indices.push_back(base+7);    indices.push_back(base+9);    indices.push_back(base+8);  

        if (_points.size() == 2 && i == 0 )
        {
            // only 2 points, add faces on ends
            plVector3 backNormal = (_points[i] - _points[l]).normalize();
            plVector3 frontNormal = -backNormal;

            // "back" wall ridge
            vertices.push_back(0.5f * (a+e) + PL_BOUNDARY_MESH_CURVE_HEIGHT * backNormal);    // position
            vertices.push_back(backNormal);   // normal
        
            vertices.push_back(0.5f * (d+h) + PL_BOUNDARY_MESH_CURVE_HEIGHT * backNormal);    // position
            vertices.push_back(backNormal);   // normal

            // "front" wall ridge
            vertices.push_back(0.5f * (b+f) + PL_BOUNDARY_MESH_CURVE_HEIGHT * frontNormal);    // position
            vertices.push_back(frontNormal);   // normal
        
            vertices.push_back(0.5f * (c+g) + PL_BOUNDARY_MESH_CURVE_HEIGHT * frontNormal);    // position
            vertices.push_back(frontNormal);   // normal

            // "back" indices
            indices.push_back(base+6);    indices.push_back(base+10);    indices.push_back(base+11);
            indices.push_back(base+6);    indices.push_back(base+11);    indices.push_back(base+9);

            indices.push_back(base+10);   indices.push_back(base+0);    indices.push_back(base+3);
            indices.push_back(base+10);   indices.push_back(base+3);    indices.push_back(base+11);

            indices.push_back(base+9);    indices.push_back(base+11);    indices.push_back(base+4);
            indices.push_back(base+11);   indices.push_back(base+3);    indices.push_back(base+4);

            // "front" indices
            indices.push_back(base+1);    indices.push_back(base+12);   indices.push_back(base+13);
            indices.push_back(base+1);    indices.push_back(base+13);   indices.push_back(base+2);

            indices.push_back(base+12);   indices.push_back(base+7);    indices.push_back(base+8);
            indices.push_back(base+12);   indices.push_back(base+8);    indices.push_back(base+13);

            indices.push_back(base+2);    indices.push_back(base+13);   indices.push_back(base+5);
            indices.push_back(base+13);   indices.push_back(base+8);    indices.push_back(base+5);

            break; // break out of loop, a 2 point wall mesh is complete from the first iteration
        }
    }

    // set vbo and attach attribute pointers
    std::shared_ptr< plVBO > vbo = std::make_shared< plVBO >();
    vbo->set( vertices );
    vbo->set( plVertexAttributePointer( PL_POSITION_ATTRIBUTE, 0  ) );
    vbo->set( plVertexAttributePointer( PL_NORMAL_ATTRIBUTE,   16 ) ); // FOUR COMPONENT FLOATING POINT OFFSET
    // set eabo
    std::shared_ptr<plEABO> eabo = std::make_shared< plEABO >();    
    eabo->set( indices );
    // attach to vao
    _vao = std::make_shared< plVAO >();
    _vao->attach( vbo );
    _vao->attach( eabo );
    // upload to gpu
    _vao->upload(); 
}


std::ostream& operator << ( std::ostream& out, const plBoundary &b )
{
    for (PLuint j=0; j<b.size(); j++)
    {
        if (j != 0) out << ","; 
        out << b.points(j) << "," << b.normals(j);
    }
    return out;
}


