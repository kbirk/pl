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


void plModel::extractRenderComponents( std::set<plRenderComponent>& renderComponents ) const
{
    if ( !_isVisible )
        return;

    if ( !_isTransparent ) 
    {
        //glDisable( GL_STENCIL_TEST );            // if opaque, allow overwriting pixels during picking
        //plColourStack::load( colour.x, colour.y, colour.z, 1.0f ); 
        
        renderComponents.insert( plRenderComponent( &_vao ) );
    }
    else
    {
        //glEnable( GL_STENCIL_TEST );             // if transparent, prevent overwriting pixels during picking
        //plColourStack::load( colour.x, colour.y, colour.z, 0.2f );
        plVector4 tC( PL_MODEL_BONE_COLOUR );
        plColourStack::load( plVector4( tC.x, tC.y, tC.z, 0.4) ); 
        renderComponents.insert( plRenderComponent( &_vao ) );
        /*
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
        */
        //glDisable( GL_STENCIL_TEST ); 
    } 

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

    std::vector<PLuint> attributeTypes;  
	attributeTypes.push_back( PL_POSITION_ATTRIBUTE ); 
	attributeTypes.push_back( PL_NORMAL_ATTRIBUTE   ); 

    _vao.set( vertices, attributeTypes, indices );
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



