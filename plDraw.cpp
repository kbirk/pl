#include "plDraw.h"
 
/*
namespace plDraw
{

    void arrow( const plVector3 &origin, const plVector3 &direction, PLfloat length, PLfloat scale)
    {
        plModelStack::push(); 
        {
            // set to origin
            plModelStack::translate( origin.x, origin.y, origin.z );
            arrow( direction, length, scale);
        }
        plModelStack::pop(); 
    }


    void arrow( const plVector3 &direction, PLfloat length, PLfloat scale)
    {      
        static plVAO cone( generateCylinderVAO( PL_HEAD_RADIUS, 0.0f, PL_ARROW_LENGTH, 30, 1 ) );

        plMatrix44 rot; rot.setRotation( plVector3(0,0,1), direction.normalize());

        plModelStack::push(); 
        {               
            // set to direction
            plModelStack::mult( rot );  
            plModelStack::scale(scale, scale, scale); 
            cylinder(scale*PL_HANDLE_RADIUS, length/scale);
            plModelStack::translate( 0, 0, length/scale );
            cone.draw();
            disk(PL_HEAD_RADIUS, true );
        } 
        plModelStack::pop();   
    }


    void knob( const plVector3 &origin, const plVector3 &direction, PLfloat length, PLfloat scale)
    {
        plModelStack::push(); 
        {
            // set to origin
            plModelStack::translate( origin.x, origin.y, origin.z );
            knob( direction, length, scale);
        }
        plModelStack::pop();    
    }


    void knob(const plVector3 &direction, PLfloat length, PLfloat scale)
    {
        plMatrix44 rot; rot.setRotation( plVector3(0,0,1), direction.normalize());
        
        plModelStack::push(); 
        {
            // set to direction
            plModelStack::mult( rot );
            cylinder(scale*PL_HANDLE_RADIUS, length);
            plModelStack::translate( 0, 0, length/scale );
            cylinder(scale*PL_HEAD_RADIUS, PL_CIRCLE_LENGTH);
            disk(scale*PL_HEAD_RADIUS, true);
            plModelStack::translate( 0, 0, PL_CIRCLE_LENGTH );
            disk(scale*PL_HEAD_RADIUS);
        } 
        plModelStack::pop();     
    }


    void sphere(float radius)
    {
        static plVAO sphere( generateSphereVAO(1, 20, 20) );

        plModelStack::push();    
        {
            plModelStack::scale(radius, radius, radius);
            sphere.draw();
        }
        plModelStack::pop();
    }


    void sphere(const plVector3 &origin, float radius)
    {
        plModelStack::push();  
        {
            plModelStack::translate( origin.x, origin.y, origin.z );
            sphere( radius );
        }
        plModelStack::pop();  
    }


    void cylinder(float radius, float height)
    {
        static plVAO cylinder( generateCylinderVAO( 1.0f, 1.0f, 1.0f, 30, 1 ) );    

        plModelStack::push();
        {
            plModelStack::scale(radius, radius, height);        
            cylinder.draw();
        }
        plModelStack::pop();
    }


    void cylinder(const plVector3 &origin, const plVector3 &direction, float radius, float height)
    {
        plMatrix44 rot; rot.setRotation( plVector3(0,0,1), direction.normalize());

        plModelStack::push();
        {
            plModelStack::translate( origin.x, origin.y, origin.z );
            plModelStack::mult( rot ); 
            plModelStack::translate( 0, 0, -height );                  
            cylinder( radius, height );  
        }
        plModelStack::pop();
    }


    void disk( const plVector3 &origin, const plVector3 &direction, float radius, PLbool flip )
    {
        plMatrix44 rot; rot.setRotation( plVector3(0,0,1), direction.normalize());

        plModelStack::push();
        {
            plModelStack::translate( origin.x, origin.y, origin.z );
            plModelStack::mult( rot );
            disk( radius, flip );       
        }
        plModelStack::pop();
    }


    void disk(const plVector3 &origin, float radius, PLbool flip)
    {
        plModelStack::push();
        {
            plModelStack::translate( origin.x, origin.y, origin.z );
            disk( radius, flip );       
        }
        plModelStack::pop();
    }

    void disk(float radius, PLbool flip)
    {
        static plVAO disk( generateDiskVAO( 0.0f, 1.0f, 20, 20 ) );   

        plModelStack::push();
        {
            plModelStack::scale( radius, radius, radius ); 
            if (flip ) 
                plModelStack::rotate( 180, plVector3(1,0,0) );   
            disk.draw();
        }
        plModelStack::pop();
    }


    void cube(float halfwidth)
    {
        static plVAO cube( generateCubeVAO( 1.0f ) );

        plModelStack::push();
        {
            plModelStack::scale(halfwidth, halfwidth, halfwidth);
            cube.draw();
        }
        plModelStack::pop();
    }


    void cube(const plVector3 &origin, float halfwidth)
    {
        plModelStack::push();
        {
            plModelStack::translate( origin.x, origin.y, origin.z );
            cube( halfwidth );
        }
        plModelStack::pop();
    }


    void laserLine(const plVector3 &origin, const plVector3 &direction, float length)
    {
        plModelStack::push();
        {
            cylinder( origin, direction, 0.1f, length);
        }
        plModelStack::pop();
    }


    plVAO generateCubeVAO( float halfWidth )
    {
        std::vector<plVector3> vertices;    vertices.reserve( 8  );
        std::vector<PLuint>    indices;     indices.reserve ( 8*3 ); //12 );
        
        // +ve x face
        vertices.push_back( plVector3(  halfWidth, -halfWidth, -halfWidth ) ); // point00
        vertices.push_back( plVector3(        1.f,        0.f,        0.f ) ); // normal
        vertices.push_back( plVector3(  halfWidth, -halfWidth,  halfWidth ) ); // point01
        vertices.push_back( plVector3(        1.f,        0.f,        0.f ) ); // normal
        vertices.push_back( plVector3(  halfWidth,  halfWidth,  halfWidth ) ); // point02
        vertices.push_back( plVector3(        1.f,        0.f,        0.f ) ); // normal
        vertices.push_back( plVector3(  halfWidth,  halfWidth, -halfWidth ) ); // point03
        vertices.push_back( plVector3(        1.f,        0.f,        0.f ) ); // normal
        
        // -ve x face
        vertices.push_back( plVector3( -halfWidth, -halfWidth, -halfWidth ) ); // point04
        vertices.push_back( plVector3(       -1.f,        0.f,        0.f ) ); // normal
        vertices.push_back( plVector3( -halfWidth, -halfWidth,  halfWidth ) ); // point05
        vertices.push_back( plVector3(       -1.f,        0.f,        0.f ) ); // normal
        vertices.push_back( plVector3( -halfWidth,  halfWidth,  halfWidth ) ); // point06
        vertices.push_back( plVector3(       -1.f,        0.f,        0.f ) ); // normal
        vertices.push_back( plVector3( -halfWidth,  halfWidth, -halfWidth ) ); // point07
        vertices.push_back( plVector3(       -1.f,        0.f,        0.f ) ); // normal

        // +ve y face
        vertices.push_back( plVector3( -halfWidth,  halfWidth, -halfWidth ) ); // point08
        vertices.push_back( plVector3(        0.f,        1.f,        0.f ) ); // normal
        vertices.push_back( plVector3( -halfWidth,  halfWidth,  halfWidth ) ); // point09
        vertices.push_back( plVector3(        0.f,        1.f,        0.f ) ); // normal
        vertices.push_back( plVector3(  halfWidth,  halfWidth,  halfWidth ) ); // point10
        vertices.push_back( plVector3(        0.f,        1.f,        0.f ) ); // normal
        vertices.push_back( plVector3(  halfWidth,  halfWidth, -halfWidth ) ); // point11
        vertices.push_back( plVector3(        0.f,        1.f,        0.f ) ); // normal

        // -ve y face
        vertices.push_back( plVector3( -halfWidth, -halfWidth, -halfWidth ) ); // point12
        vertices.push_back( plVector3(        0.f,       -1.f,        0.f ) ); // normal
        vertices.push_back( plVector3( -halfWidth, -halfWidth,  halfWidth ) ); // point13
        vertices.push_back( plVector3(        0.f,       -1.f,        0.f ) ); // normal
        vertices.push_back( plVector3(  halfWidth, -halfWidth,  halfWidth ) ); // point14
        vertices.push_back( plVector3(        0.f,       -1.f,        0.f ) ); // normal
        vertices.push_back( plVector3(  halfWidth, -halfWidth, -halfWidth ) ); // point15
        vertices.push_back( plVector3(        0.f,       -1.f,        0.f ) ); // normal

        // +ve z face
        vertices.push_back( plVector3( -halfWidth, -halfWidth,  halfWidth ) ); // point16
        vertices.push_back( plVector3(        0.f,        0.f,        1.f ) ); // normal
        vertices.push_back( plVector3( -halfWidth,  halfWidth,  halfWidth ) ); // point17
        vertices.push_back( plVector3(        0.f,        0.f,        1.f ) ); // normal
        vertices.push_back( plVector3(  halfWidth,  halfWidth,  halfWidth ) ); // point18
        vertices.push_back( plVector3(        0.f,        0.f,        1.f ) ); // normal
        vertices.push_back( plVector3(  halfWidth, -halfWidth,  halfWidth ) ); // point19
        vertices.push_back( plVector3(        0.f,        0.f,        1.f ) ); // normal

        // -ve z face
        vertices.push_back( plVector3( -halfWidth, -halfWidth, -halfWidth ) ); // point20
        vertices.push_back( plVector3(        0.f,        0.f,       -1.f ) ); // normal
        vertices.push_back( plVector3( -halfWidth,  halfWidth, -halfWidth ) ); // point21
        vertices.push_back( plVector3(        0.f,        0.f,       -1.f ) ); // normal
        vertices.push_back( plVector3(  halfWidth,  halfWidth, -halfWidth ) ); // point22
        vertices.push_back( plVector3(        0.f,        0.f,       -1.f ) ); // normal
        vertices.push_back( plVector3(  halfWidth, -halfWidth, -halfWidth ) ); // point23
        vertices.push_back( plVector3(        0.f,        0.f,       -1.f ) ); // normal

        // +ve x
        indices.push_back( 0  );   indices.push_back( 2  );   indices.push_back( 1  );
        indices.push_back( 0  );   indices.push_back( 3  );   indices.push_back( 2  );
        
        // -ve x
        indices.push_back( 4  );   indices.push_back( 5  );   indices.push_back( 6  );
        indices.push_back( 4  );   indices.push_back( 6  );   indices.push_back( 7  );

        // +ve y
        indices.push_back( 8  );   indices.push_back( 9  );   indices.push_back( 10 );
        indices.push_back( 8  );   indices.push_back( 10 );   indices.push_back( 11 );

        // -ve y
        indices.push_back( 12 );   indices.push_back( 14 );   indices.push_back( 13 );
        indices.push_back( 12 );   indices.push_back( 15 );   indices.push_back( 14 );

        // +ve z
        indices.push_back( 16 );   indices.push_back( 18 );   indices.push_back( 17 );
        indices.push_back( 16 );   indices.push_back( 19 );   indices.push_back( 18 );

        // -ve z
        indices.push_back( 20 );   indices.push_back( 21 );   indices.push_back( 22 );
        indices.push_back( 20 );   indices.push_back( 22 );   indices.push_back( 23 );

        std::vector< PLuint > attributeTypes;    
        attributeTypes.push_back( PL_POSITION_ATTRIBUTE );
        attributeTypes.push_back( PL_NORMAL_ATTRIBUTE );

        return plVAO( vertices, attributeTypes, indices );
    }


    plVAO generateCubeLineVAO( PLfloat halfWidth )
    {
        std::vector<plVector3> vertices;    vertices.reserve( 8 );
        std::vector<PLuint>    indices;     indices.reserve( 8*3 );   
    
        // front face
        vertices.emplace_back( plVector3( -halfWidth, -halfWidth, halfWidth ) );
        vertices.emplace_back( plVector3(  halfWidth, -halfWidth, halfWidth ) );
        vertices.emplace_back( plVector3(  halfWidth, halfWidth, halfWidth ) );
        vertices.emplace_back( plVector3( -halfWidth, halfWidth, halfWidth ) );
    
        // back face
        vertices.emplace_back( plVector3( -halfWidth, -halfWidth, -halfWidth ) );
        vertices.emplace_back( plVector3(  halfWidth, -halfWidth, -halfWidth ) );
        vertices.emplace_back( plVector3(  halfWidth,  halfWidth, -halfWidth ) );
        vertices.emplace_back( plVector3( -halfWidth,  halfWidth, -halfWidth ) );
    
        // front
        indices.push_back( 0 );   indices.push_back( 1 );   
        indices.push_back( 1 );   indices.push_back( 2 );   
        indices.push_back( 2 );   indices.push_back( 3 );   
        indices.push_back( 3 );   indices.push_back( 0 );   

        // sides 
        indices.push_back( 0 );   indices.push_back( 4 );   
        indices.push_back( 1 );   indices.push_back( 5 );   
        indices.push_back( 2 );   indices.push_back( 6 );   
        indices.push_back( 3 );   indices.push_back( 7 ); 

        // back
        indices.push_back( 4 );   indices.push_back( 5 );   
        indices.push_back( 5 );   indices.push_back( 6 );   
        indices.push_back( 6 );   indices.push_back( 7 );   
        indices.push_back( 7 );   indices.push_back( 4 );

        std::vector<PLuint> attributeTypes; 
        attributeTypes.push_back( PL_POSITION_ATTRIBUTE );

        return plVAO( vertices, attributeTypes, indices, GL_LINES );
    }


    plVAO generateSphereVAO( float radius, int slices, int stacks ) 
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

        std::vector< PLuint > attributeTypes;    
        attributeTypes.push_back( PL_POSITION_ATTRIBUTE );
        attributeTypes.push_back( PL_NORMAL_ATTRIBUTE );

        return plVAO( vertices, attributeTypes, indices );
    }


    plVAO generateCylinderVAO( float baseRadius, float topRadius, float height, int slices, int stacks ) 
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
        
        std::vector< PLuint > attributeTypes;    
        attributeTypes.push_back( PL_POSITION_ATTRIBUTE );
        attributeTypes.push_back( PL_NORMAL_ATTRIBUTE );

        return plVAO( vertices, attributeTypes, indices );
    }       


    plVAO generateDiskVAO( float innerRadius, float outerRadius, int slices, int loops, bool up )
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
        
        std::vector< PLuint > attributeTypes;    
        attributeTypes.push_back( PL_POSITION_ATTRIBUTE );
        attributeTypes.push_back( PL_NORMAL_ATTRIBUTE );

        return plVAO( vertices, attributeTypes, indices );
    }


}
*/

