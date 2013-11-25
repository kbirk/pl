#include "plModel.h"

plModel::plModel( const std::vector<plTriangle> &triangles, const plString &file, PLuint octreeDepth )
    : _mesh( triangles, octreeDepth ), filename( file )
{
    _generateVAO();
}


plModel::plModel( const plString &file, PLuint octreeDepth )
    : filename( file )
{

    std::vector< plTriangle > triangles;
    // import triangles from STL file
    if ( !plSTL::importFile( triangles, filename ) )
        return;
     
    _mesh = plOctreeMesh( std::move( triangles ), octreeDepth );
    
    _generateVAO();
}


void plModel::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    if ( !_isVisible )
        return;

    // create render component
    plRenderComponent component( _vao );
    // attached uniforms
    component.attach( plUniform( PL_MODEL_MATRIX_UNIFORM,      plModelStack::top()      ) );
    component.attach( plUniform( PL_VIEW_MATRIX_UNIFORM,       plCameraStack::top()     ) );
    component.attach( plUniform( PL_PROJECTION_MATRIX_UNIFORM, plProjectionStack::top() ) );
    component.attach( plUniform( PL_LIGHT_POSITION_UNIFORM,    plVector3( PL_LIGHT_POSITION ) ) );    

    if ( !_isTransparent ) 
    {
        component.attach( plUniform( PL_PICKING_UNIFORM, plPickingStack::top() ) );
        component.attach( plUniform( PL_COLOUR_UNIFORM,  plColourStack::top()  ) ); 
        // insert into render map   
        renderMap[ technique ].insert( component );        
    }
    else
    {
        plVector4 currentColour = plColourStack::top();
        component.attach( plUniform( PL_COLOUR_UNIFORM,  plVector4( currentColour.x, currentColour.y, currentColour.z, 0.7)  ) ); 
        // insert into render map   
        renderMap[ PL_MODEL_TECHNIQUE ].insert( component );        
        
        // Sort by distance
        plVector3 viewDir = plCameraStack::direction();

        std::vector<plOrderPair> order;     order.reserve( _mesh.triangles().size() );
        PLuint index = 0;
        for ( const plTriangle& triangle : _mesh.triangles() )
        {
            order.emplace_back( plOrderPair( index++, triangle.centroid() * viewDir) );
        }
        std::sort( order.begin(), order.end() );

        std::vector<PLuint> indices;    indices.reserve( _mesh.triangles().size()*3 );
        for (PLuint i = 0; i < order.size(); i++)
        {
            indices.push_back( order[i].index*3 );
            indices.push_back( order[i].index*3+1 );
            indices.push_back( order[i].index*3+2 );
        }             
        const_cast< std::shared_ptr< plVAO >& >( _vao )->eabo()->set( indices );
        const_cast< std::shared_ptr< plVAO >& >( _vao )->upload();
    }    
}
        


void plModel::extractRenderComponents( plRenderMap& renderMap ) const
{
    extractRenderComponents( renderMap, PL_PLAN_TECHNIQUE );
}


void plModel::_generateVAO()
{			
	// convert to interleaved format
	std::vector<plVector3> vertices;    vertices.reserve( _mesh.triangles().size() * 3 * 2 );
	std::vector<PLuint>    indices;     indices.reserve ( _mesh.triangles().size() * 3);

    int indexCount = 0;
    for ( const plTriangle& triangle : _mesh.triangles() )
    {  
        // p1
	    vertices.emplace_back( triangle.point0() );    // position
	    vertices.emplace_back( triangle.normal() );    // normal
	    indices.emplace_back( indexCount++ );
	    // p2
	    vertices.emplace_back( triangle.point1() );
	    vertices.emplace_back( triangle.normal() );
	    indices.emplace_back( indexCount++ );
	    // p3
	    vertices.emplace_back( triangle.point2() );
	    vertices.emplace_back( triangle.normal() );
	    indices.emplace_back( indexCount++ );	    
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

/*
void plModel::draw( const plVector3 &colour ) const
{
    if ( !_isVisible )
        return;

    if ( !_isTransparent ) 
    {
        glDisable( GL_STENCIL_TEST );            // if opaque, allow overwriting pixels during picking
        plColourStack::load( colour.x, colour.y, colour.z, 1.0f ); 
        _vao.draw();
    }
    else
    {
        glEnable( GL_STENCIL_TEST );             // if transparent, prevent overwriting pixels during picking
        plColourStack::load( colour.x, colour.y, colour.z, 0.2f );

        // Sort by distance
        plVector3 viewDir = plCameraStack::direction();

        std::vector<plOrderPair> order;     order.reserve( _mesh.triangles().size() );
        PLuint index = 0;
        for ( const plTriangle& triangle : _mesh.triangles() )
        {
            order.emplace_back( plOrderPair( index++, triangle.centroid() * viewDir) );
        }
        std::sort( order.begin(), order.end() );

        std::vector<PLuint> indices;    indices.reserve( _mesh.triangles().size()*3 );
        for (PLuint i = 0; i < order.size(); i++)
        {
            indices.push_back( order[i].index*3 );
            indices.push_back( order[i].index*3+1 );
            indices.push_back( order[i].index*3+2 );
        }             
        _vao.draw( indices );

        glDisable( GL_STENCIL_TEST ); 
    } 
}
*/


std::ostream& operator << ( std::ostream& out, const plModel &m )
{
    out << m.filename;
    return out;
}



