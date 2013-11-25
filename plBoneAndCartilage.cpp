#include "plBoneAndCartilage.h"


plBoneAndCartilage::plBoneAndCartilage( const plString &boneFile, const plString &cartilageFile, const plString &combinedFile)
    :   bone( boneFile ), cartilage( cartilageFile ), combined( combinedFile )
{
}



void plBoneAndCartilage::extractRenderComponents( plRenderMap& renderMap ) const
{
    /*
    if ( bone.isTransparent() && cartilage.isTransparent() )
    {
        // both are transparent, combine triangles and sort to ensure proper rendering
        static std::shared_ptr< plVAO > vao =  std::make_shared<plVAO>( _generateTransparencyVAO() );

        plColourStack::push( PL_COLOUR_MESH_TRANSPARENT_COLOUR );   

        // create render component
        plRenderComponent component( vao );
        // attached uniforms
        component.attach( plUniform( PL_MODEL_MATRIX_UNIFORM,      plModelStack::top()      ) );
        component.attach( plUniform( PL_VIEW_MATRIX_UNIFORM,       plCameraStack::top()     ) );
        component.attach( plUniform( PL_PROJECTION_MATRIX_UNIFORM, plProjectionStack::top() ) );
        component.attach( plUniform( PL_LIGHT_POSITION_UNIFORM,    plVector3( PL_LIGHT_POSITION ) ) );  
        component.attach( plUniform( PL_COLOUR_UNIFORM,  plColourStack::top()  ) ); 
        // insert into render map   
        renderMap[ PL_MODEL_TECHNIQUE ].insert( component );  

        return;
    }
    */
    // draw cartilage
    plPickingStack::loadRed( PL_PICKING_TYPE_CARTILAGE );
    plColourStack::load( PL_MODEL_CARTILAGE_COLOUR );
    cartilage.extractRenderComponents( renderMap ); 

    // draw bone
    plPickingStack::loadRed( PL_PICKING_TYPE_BONE );
    plColourStack::load( PL_MODEL_BONE_COLOUR );       
    bone.extractRenderComponents( renderMap );
}

/*
plVAO plBoneAndCartilage::_generateTransparencyVAO() const
{
    // Sort by distance
    plVector3 viewDir = plCameraStack::direction();

    std::vector<plOrderPair> order;     order.reserve( bone.mesh().triangles().size() + cartilage.mesh().triangles().size() );
    PLuint index = 0;

    // add bone triangles
    for ( const plTriangle& triangle : bone.mesh().triangles() )
        order.emplace_back( plOrderPair( index++, PL_MAX_OF_3( triangle.point0() * viewDir, triangle.point1() * viewDir, triangle.point2() * viewDir ) ) ); //triangle.centroid() * viewDir) );

    // add cartilage triangles
    for ( const plTriangle& triangle : cartilage.mesh().triangles() )
        order.emplace_back( plOrderPair( index++, PL_MAX_OF_3( triangle.point0() * viewDir, triangle.point1() * viewDir, triangle.point2() * viewDir ) ) ); //triangle.centroid() * viewDir) );

    // sort by depth
    std::sort( order.begin(), order.end() );

    // create index array
    std::vector<PLuint> indices;    indices.reserve( bone.mesh().triangles().size() + cartilage.mesh().triangles().size()*3 );
    for (PLuint i = 0; i < order.size(); i++)
    {
        indices.push_back( order[i].index*3 );
        indices.push_back( order[i].index*3+1 );
        indices.push_back( order[i].index*3+2 );
    }     

    std::vector<plVector3> vertices;    vertices.reserve( bone.mesh().triangles().size() + cartilage.mesh().triangles().size() * 6 );

    for ( const plTriangle& triangle : bone.mesh().triangles() )
    {  
        // p1
	    vertices.emplace_back( triangle.point0() );    // position
	    vertices.emplace_back( triangle.normal() );    // normal
        vertices.emplace_back( plVector3( PL_MODEL_BONE_COLOUR ) );
	    // p2
	    vertices.emplace_back( triangle.point1() );
	    vertices.emplace_back( triangle.normal() );
        vertices.emplace_back( plVector3( PL_MODEL_BONE_COLOUR ) );
	    // p3
	    vertices.emplace_back( triangle.point2() );
	    vertices.emplace_back( triangle.normal() );   
        vertices.emplace_back( plVector3( PL_MODEL_BONE_COLOUR ) );
	}

    for ( const plTriangle& triangle : cartilage.mesh().triangles() )
    {  
        // p1
	    vertices.emplace_back( triangle.point0() );    // position
	    vertices.emplace_back( triangle.normal() );    // normal
        vertices.emplace_back( plVector3( PL_MODEL_CARTILAGE_COLOUR ) );
	    // p2
	    vertices.emplace_back( triangle.point1() );
	    vertices.emplace_back( triangle.normal() );
        vertices.emplace_back( plVector3( PL_MODEL_CARTILAGE_COLOUR ) );
	    // p3
	    vertices.emplace_back( triangle.point2() );
	    vertices.emplace_back( triangle.normal() );  
        vertices.emplace_back( plVector3( PL_MODEL_CARTILAGE_COLOUR ) );
	}

    // set vbo and attach attribute pointers
    std::shared_ptr< plVBO > vbo = std::make_shared< plVBO >();
    vbo->set( vertices );
    vbo->set( plVertexAttributePointer( PL_POSITION_ATTRIBUTE, 0  ) );
    vbo->set( plVertexAttributePointer( PL_NORMAL_ATTRIBUTE,   16 ) );
    vbo->set( plVertexAttributePointer( PL_COLOUR_ATTRIBUTE,   32 ) );
    // set eabo
    std::shared_ptr<plEABO> eabo = std::make_shared< plEABO >();    
    eabo->set( indices );
    // create vao, attach eabo and vbo, upload to gpu
    plVAO vao;
    vao.attach( vbo );
    vao.attach( eabo );
    // upload to gpu
    vao.upload(); 

    return vao;
}
*/


plVector3 plBoneAndCartilage::getCentroid() const
{
    plVector3 min, max;
    
    combined.mesh().getMinMax( min, max );

    return 0.5f * (max + min);    
}   



