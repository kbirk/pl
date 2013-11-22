#include "plSphere.h"


plSphere::plSphere( PLuint techniqueEnum, const plVector3& position, PLfloat radius )
    :   _techniqueEnum( techniqueEnum ),
        _position( position ), 
        _radius( radius )
{
}


void plSphere::position( const plVector3 position )
{
    _position = position;
}

void plSphere::radius  ( PLfloat radius  )
{
    _radius = radius;
}

void plSphere::extractRenderComponents( plRenderMap& renderMap ) const
{
    static plVAO vao = _generateVAO( 1.0f, 20, 20 );

    plModelStack::push();
    plModelStack::translate( _position );   // transform
    plModelStack::scale( _radius );         // scale

    plRenderComponent component( std::make_shared<plVAO>( vao ) );
    
    component.attach( plUniform( PL_MODEL_MATRIX_UNIFORM,      plModelStack::top()      ) );
    component.attach( plUniform( PL_VIEW_MATRIX_UNIFORM,       plCameraStack::top()     ) );
    component.attach( plUniform( PL_PROJECTION_MATRIX_UNIFORM, plProjectionStack::top() ) );
    component.attach( plUniform( PL_COLOUR_UNIFORM,            plColourStack::top()     ) ); 
    component.attach( plUniform( PL_PICKING_UNIFORM,           plPickingStack::top()    ) );
    component.attach( plUniform( PL_LIGHT_POSITION_UNIFORM,    plVector3( PL_LIGHT_POSITION ) ) ); 
    
    renderMap[ _techniqueEnum ].insert( component );  

    plModelStack::pop();         
}


plVAO plSphere::_generateVAO( float radius, int slices, int stacks ) const
{
    float rho, theta;
    float x, y, z;
    PLint base;

    float drho = PL_PI / stacks;
    float dtheta = 2.0f * PL_PI / slices;
    
    std::vector<plVector3> vertices; 
    std::vector<PLuint>    indices;

    // draw +Z end as a triangle fan
    // centre of triangle fan
    vertices.push_back( plVector3(0.0f, 0.0f, radius));   // position
    vertices.push_back( plVector3(0.0f, 0.0f, 1.0f));     // normal    
    for (int j = 0; j <= slices; j++) 
    {
        theta = (j == slices) ? 0.0f : j * dtheta;
        x = -sin(theta) * sin(drho);
        y = cos(theta) * sin(drho);
        z = cos(drho);
    
        vertices.push_back( plVector3(x * radius, y * radius, z * radius)); // position
        vertices.push_back( plVector3(x, y, z) );                           // normal
    }

    for (int j = 1; j <= slices; j++) 
    {
        indices.push_back(0);
        indices.push_back(j);
        indices.push_back(j+1);
    }
     
    int imin = 1;
    int imax = stacks - 1;

    // draw intermediate stacks as quad strips
    for (int i = imin; i < imax; i++) 
    {
        base = vertices.size()/2;

        rho = i * drho;

        for (int j = 0; j <= slices; j++) 
        {
            theta = (j == slices) ? 0.0f : j * dtheta;
            
            x = -sin(theta) * sin(rho);
            y = cos(theta) * sin(rho);
            z = cos(rho);

            vertices.push_back( plVector3(x * radius, y * radius, z * radius)); // position
            vertices.push_back( plVector3(x, y, z));                            // normal

            x = -sin(theta) * sin(rho + drho);
            y = cos(theta) * sin(rho + drho);
            z = cos(rho + drho);

            vertices.push_back( plVector3(x * radius, y * radius, z * radius)); // position
            vertices.push_back( plVector3(x, y, z));                            // normal
        }

        for (int j = 0; j < slices*2; j+=2) 
        {
            indices.push_back(base+j);
            indices.push_back(base+j+1);
            indices.push_back(base+j+2);

            indices.push_back(base+j+1);
            indices.push_back(base+j+3);
            indices.push_back(base+j+2);
        }
    }

    // draw -Z end as a triangle fan
    base = vertices.size()/2;

    vertices.push_back( plVector3(0.0f, 0.0f, -radius));   // position
    vertices.push_back( plVector3(0.0f, 0.0f, -1.0f));     // normal

    rho = PL_PI - drho;

    for (int j = slices; j >= 0; j--) 
    {
        theta = (j == slices) ? 0.0f : j * dtheta;
        x = -sin(theta) * sin(rho);
        y = cos(theta) * sin(rho);
        z = cos(rho);

        vertices.push_back( plVector3(x * radius, y * radius, z * radius)); // position
        vertices.push_back( plVector3(x, y, z));                            // normal
    }

    for (int j = 1; j <= slices; j++) 
    {
        indices.push_back(base+0);
        indices.push_back(base+j);
        indices.push_back(base+j+1);
    }

    // set vbo and attach attribute pointers
    std::shared_ptr<plVBO> vbo( new plVBO() );
    vbo->set( vertices );
    vbo->set( plVertexAttributePointer( PL_POSITION_ATTRIBUTE, 0  ) );
    vbo->set( plVertexAttributePointer( PL_NORMAL_ATTRIBUTE,   16 ) );
    // set eabo
    std::shared_ptr<plEABO> eabo( new plEABO() );    
    eabo->set( indices );
    // create and attach to vao
    plVAO vao;
    vao.attach( vbo );
    vao.attach( eabo );
    // upload to gpu
    vao.upload(); 

    return vao;
}



