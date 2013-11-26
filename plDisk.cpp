#include "plDisk.h"


plDisk::plDisk( PLuint techniqueEnum, const plVector3& position, PLfloat radius, PLbool flip )
    :   _position( position ), _radius( radius ), _flip( flip ), _techniqueEnum( techniqueEnum )
{
}


void plDisk::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    static std::shared_ptr< plVAO > vao = std::make_shared< plVAO >( _generateVAO( 0.0f, 1.0f, 20, 20 ) );

    plModelStack::push();
    plModelStack::translate( _position );   // transform
    plModelStack::scale( _radius );         // scale

    if ( _flip ) 
        plModelStack::rotate( 180.0f, plVector3(1,0,0) );

    // create render component
    plRenderComponent component( vao );
    // attached uniforms
    component.attach( plUniform( PL_MODEL_MATRIX_UNIFORM,      plModelStack::top()      ) );
    component.attach( plUniform( PL_VIEW_MATRIX_UNIFORM,       plCameraStack::top()     ) );
    component.attach( plUniform( PL_PROJECTION_MATRIX_UNIFORM, plProjectionStack::top() ) );
    component.attach( plUniform( PL_COLOUR_UNIFORM,            plColourStack::top()     ) ); 
    component.attach( plUniform( PL_PICKING_UNIFORM,           plPickingStack::top()    ) );
    component.attach( plUniform( PL_LIGHT_POSITION_UNIFORM,    plVector3( PL_LIGHT_POSITION ) ) ); 
    // insert into render map
    renderMap[ technique ].insert( component );  
    
    plModelStack::pop();         
}

void plDisk::extractRenderComponents( plRenderMap& renderMap ) const
{
    extractRenderComponents( renderMap, _techniqueEnum );
}

plVAO plDisk::_generateVAO( float innerRadius, float outerRadius, int slices, int loops, bool up ) const
{
    plVector3 normal = (up) ? plVector3(0.0f, 0.0f, 1.0f) : plVector3(0.0f, 0.0f, -1.0f);

    float da = 2.0f * PL_PI / slices;
    float dr = (outerRadius - innerRadius) /  loops;

    float sa, ca;
    float r1 = innerRadius;

    std::vector<plVector3> vertices; 
    std::vector<PLuint>    indices;

    for (int l = 0; l < loops; l++) 
    {
        float r2 = r1 + dr;
            
        PLint base = vertices.size()/2;

        if (up) 
        {
            for (int s = 0; s <= slices; s++) 
            {
                float a = (s == slices) ? 0.0f : s * da;
                    
                sa = sin(a);
                ca = cos(a);
                    
                vertices.push_back( plVector3(r2 * sa, r2 * ca, 0.0f) );  // position
                vertices.push_back( normal );                             // normal
                    
                vertices.push_back( plVector3(r1 * sa, r1 * ca, 0.0f) );  // position
                vertices.push_back( normal );                             // normal
            }
        }
        else 
        {
            for (int s = slices; s >= 0; s--) 
            {
                float a = (s == slices) ? 0.0f : s * da;
                    
                sa = sin(a);
                ca = cos(a);
                    
                vertices.push_back( plVector3(r2 * sa, r2 * ca, 0.0f) );  // position
                vertices.push_back( normal );                             // normal
                    
                vertices.push_back( plVector3(r1 * sa, r1 * ca, 0.0f) );  // position
                vertices.push_back( normal );                             // normal
            }
        }
            
        r1 = r2;
            
        for (int i = 0; i < slices*2; i+=2) 
        {
            indices.push_back(base+i);
            indices.push_back(base+i+1);
            indices.push_back(base+i+2);

            indices.push_back(base+i+1);
            indices.push_back(base+i+3);
            indices.push_back(base+i+2);
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
    // create and attach to vao
    plVAO vao;
    vao.attach( vbo );
    vao.attach( eabo );
    // upload to gpu
    vao.upload(); 

    return vao;
}
