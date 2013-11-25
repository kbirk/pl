#include "plCone.h"


plCone::plCone( PLuint techniqueEnum, const plVector3 &position, const plVector3 &direction, PLfloat bottomRadius, PLfloat topRadius, PLfloat length )
    :   _techniqueEnum( techniqueEnum ),
        _position( position ),
        _bottomRadius( bottomRadius ), 
        _topRadius( topRadius ),
        _direction( direction ),
        _length( length )
{
}


void plCone::extractRenderComponents( plRenderMap& renderMap ) const
{
    // can't use static for cones as normals scale inversely, 
    std::shared_ptr< plVAO > vao = std::make_shared< plVAO >( _generateVAO( _bottomRadius, _topRadius, _length, 30, 1 ) ); 

    plMatrix44 rot; rot.setRotation( plVector3(0,0,1), _direction.normalize() );

    plModelStack::push();
    {
        plModelStack::translate( _position.x, _position.y, _position.z );
        plModelStack::mult( rot );     
       
        plRenderComponent component( vao );
    
        component.attach( plUniform( PL_MODEL_MATRIX_UNIFORM,      plModelStack::top()      ) );
        component.attach( plUniform( PL_VIEW_MATRIX_UNIFORM,       plCameraStack::top()     ) );
        component.attach( plUniform( PL_PROJECTION_MATRIX_UNIFORM, plProjectionStack::top() ) );
        component.attach( plUniform( PL_COLOUR_UNIFORM,            plColourStack::top()     ) ); 
        component.attach( plUniform( PL_PICKING_UNIFORM,           plPickingStack::top()    ) );
        component.attach( plUniform( PL_LIGHT_POSITION_UNIFORM,    plVector3( PL_LIGHT_POSITION ) ) ); 
        
        renderMap[ _techniqueEnum ].insert( component );  
       
    }
    plModelStack::pop();      
}


plVAO plCone::_generateVAO( float baseRadius, float topRadius, float height, int slices, int stacks ) const
{
    float da = 2.0f * PL_PI / slices;
    float dr = (topRadius - baseRadius) / stacks;
    float dz = height / stacks;
    float nz = (baseRadius - topRadius) / height;

    float sa, ca;
    float z = 0.0f;
    float r = baseRadius;
    
    std::vector<plVector3> vertices; 
    std::vector<PLuint>    indices;
    
    for (int j = 0; j < stacks; j++) 
    {
        PLint base = vertices.size()/2;
        
        for (int i = 0; i <= slices; i++) 
        {        
            float a = (i == slices) ? 0.0f : i * da;
                
            sa = sin(a);
            ca = cos(a);
            
            vertices.push_back( plVector3( sa * r, ca * r, z) );  // position
            vertices.push_back( plVector3( sa, ca, nz) );         // normal

            vertices.push_back( plVector3( sa * (r + dr), ca * (r + dr), z + dz) );  // position
            vertices.push_back( plVector3( sa, ca, nz) );                             // normal                        
        } 
        
        for (int i = 0; i < slices*2; i+=2) 
        {
            indices.push_back(base+i);
            indices.push_back(base+i+1);
            indices.push_back(base+i+2);

            indices.push_back(base+i+1);
            indices.push_back(base+i+3);
            indices.push_back(base+i+2);
        }
        r += dr;
        z += dz;
        
    }

    // set vbo and attach attribute pointers
    std::shared_ptr< plVBO > vbo = std::make_shared< plVBO >();
    vbo->set( vertices );
    vbo->set( plVertexAttributePointer( PL_POSITION_ATTRIBUTE, 0  ) );
    vbo->set( plVertexAttributePointer( PL_NORMAL_ATTRIBUTE,   16 ) );
    // set eabo
    std::shared_ptr<plEABO> eabo = std::make_shared<plEABO >();    
    eabo->set( indices );
    // create and attach to vao
    plVAO vao;
    vao.attach( vbo );
    vao.attach( eabo );
    // upload to gpu
    vao.upload(); 

    return vao;
}       

