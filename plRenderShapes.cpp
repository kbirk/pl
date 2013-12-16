#include "plRenderShapes.h"


namespace plRenderShapes
{

    plVAO sphereVAO( float radius, int slices, int stacks )
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


    plVAO cylinderVAO( float baseRadius, float topRadius, float height, int slices, int stacks )
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


    plVAO diskVAO( float innerRadius, float outerRadius, int slices, int loops, bool up )
    {
        plVector3 normal = ( up ) ? plVector3( 0.0f, 0.0f, 1.0f ) : plVector3( 0.0f, 0.0f, -1.0f );

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
    

    plVAO coneVAO( float baseRadius, float topRadius, float height, int slices, int stacks )
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
    
    plVAO quadVAO()
    {
        std::vector<plVector3> vertices;        vertices.reserve( 8 );
        std::vector<PLuint>    indices;         indices.reserve( 6 );

        // position                                     // normals       
        vertices.push_back( plVector3( -1, -1, 0 ) );   vertices.push_back( plVector3( 0,0,1) );   
        vertices.push_back( plVector3(  1, -1, 0 ) );   vertices.push_back( plVector3( 0,0,1) );   
        vertices.push_back( plVector3(  1,  1, 0 ) );   vertices.push_back( plVector3( 0,0,1) );    
        vertices.push_back( plVector3( -1,  1, 0 ) );   vertices.push_back( plVector3( 0,0,1) );

        // indices
        indices.push_back( 0 );
        indices.push_back( 1 );
        indices.push_back( 2 );
        
        indices.push_back( 0 );
        indices.push_back( 2 );
        indices.push_back( 3 );
        
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
    
    
    
}
