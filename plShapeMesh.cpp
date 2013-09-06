#include "plShapeMesh.h"

// cube
plShapeMesh::plShapeMesh(float halfWidth)
{
    plSeq<plVector3> vertices( 8  );
    plSeq<PLuint>    indices ( 8*3 ); //12 );
    
    // front face
    vertices.add( plVector3( -halfWidth, -halfWidth, halfWidth) );
    vertices.add( plVector3(  halfWidth, -halfWidth, halfWidth) );
    vertices.add( plVector3(  halfWidth,  halfWidth, halfWidth) );
    vertices.add( plVector3( -halfWidth, halfWidth,  halfWidth) );
    
    // back face
    vertices.add( plVector3( -halfWidth, -halfWidth, -halfWidth) );
    vertices.add( plVector3(  halfWidth, -halfWidth, -halfWidth) );
    vertices.add( plVector3(  halfWidth,  halfWidth, -halfWidth) );
    vertices.add( plVector3( -halfWidth,  halfWidth, -halfWidth) );
    
    // front
    indices.add( 0 );   indices.add( 1 );   indices.add( 2 );
    indices.add( 0 );   indices.add( 2 );   indices.add( 3 );
    
    // right
    indices.add( 1 );   indices.add( 5 );   indices.add( 6 );
    indices.add( 1 );   indices.add( 6 );   indices.add( 2 );
    
    // back
    indices.add( 5 );   indices.add( 4 );   indices.add( 7 );
    indices.add( 5 );   indices.add( 7 );   indices.add( 6 );
    
    // left
    indices.add( 4 );   indices.add( 0 );   indices.add( 3 );
    indices.add( 4 );   indices.add( 3 );   indices.add( 7 );
    
    // top
    indices.add( 3 );   indices.add( 2 );   indices.add( 6 );
    indices.add( 3 );   indices.add( 6 );   indices.add( 7 );
    
    // bottem
    indices.add( 4 );   indices.add( 5 );   indices.add( 1 );
    indices.add( 4 );   indices.add( 1 );   indices.add( 0 );

    setBuffers( vertices, indices );
}

// sphere
plShapeMesh::plShapeMesh(float radius, int slices, int stacks) 
{
    float rho, drho, theta, dtheta;
    float x, y, z;
    int imin, imax;
    PLint base;

    drho = PL_PI / stacks;
    dtheta = 2.0f * PL_PI / slices;
    
    plSeq<plVector3> interleaved_vertices; 
    plSeq<PLuint>    indices;

    // draw +Z end as a triangle fan
    // centre of triangle fan
    interleaved_vertices.add( plVector3(0.0f, 0.0f, radius));   // position
    interleaved_vertices.add( plVector3(0.0f, 0.0f, 1.0f));     // normal    
    for (int j = 0; j <= slices; j++) 
    {
        theta = (j == slices) ? 0.0f : j * dtheta;
        x = -sin(theta) * sin(drho);
        y = cos(theta) * sin(drho);
        z = cos(drho);
    
        interleaved_vertices.add( plVector3(x * radius, y * radius, z * radius)); // position
        interleaved_vertices.add( plVector3(x, y, z) );                           // normal
    }

    for (int j = 1; j <= slices; j++) 
    {
        indices.add(0);
        indices.add(j);
        indices.add(j+1);
    }
     
    imin = 1;
    imax = stacks - 1;

    // draw intermediate stacks as quad strips
    for (int i = imin; i < imax; i++) 
    {
        base = interleaved_vertices.size()/2;

        rho = i * drho;

        for (int j = 0; j <= slices; j++) 
        {
            theta = (j == slices) ? 0.0f : j * dtheta;
            
            x = -sin(theta) * sin(rho);
            y = cos(theta) * sin(rho);
            z = cos(rho);

            interleaved_vertices.add( plVector3(x * radius, y * radius, z * radius)); // position
            interleaved_vertices.add( plVector3(x, y, z));                            // normal

            x = -sin(theta) * sin(rho + drho);
            y = cos(theta) * sin(rho + drho);
            z = cos(rho + drho);

            interleaved_vertices.add( plVector3(x * radius, y * radius, z * radius)); // position
            interleaved_vertices.add( plVector3(x, y, z));                            // normal
        }

        for (int j = 0; j < slices*2; j+=2) 
        {
            indices.add(base+j);
            indices.add(base+j+1);
            indices.add(base+j+2);

            indices.add(base+j+1);
            indices.add(base+j+3);
            indices.add(base+j+2);
        }
    }

    // draw -Z end as a triangle fan
    base = interleaved_vertices.size()/2;

    interleaved_vertices.add( plVector3(0.0f, 0.0f, -radius));   // position
    interleaved_vertices.add( plVector3(0.0f, 0.0f, -1.0f));     // normal

    rho = PL_PI - drho;

    for (int j = slices; j >= 0; j--) 
    {
        theta = (j == slices) ? 0.0f : j * dtheta;
        x = -sin(theta) * sin(rho);
        y = cos(theta) * sin(rho);
        z = cos(rho);

        interleaved_vertices.add( plVector3(x * radius, y * radius, z * radius)); // position
        interleaved_vertices.add( plVector3(x, y, z));                            // normal
    }

    for (int j = 1; j <= slices; j++) 
    {
        indices.add(base+0);
        indices.add(base+j);
        indices.add(base+j+1);
    }

    // set VBOs and VAO
    setBuffers(interleaved_vertices, indices);
}

// cylinder
plShapeMesh::plShapeMesh(float baseRadius, float topRadius, float height, int slices, int stacks) 
{
 
    float da, r, dr, dz;
    float x, y, z, nz;

    da = 2.0f * PL_PI / slices;
    dr = (topRadius - baseRadius) / stacks;
    dz = height / stacks;
    nz = (baseRadius - topRadius) / height;
   
    // Z component of normal vectors

    z = 0.0f;
    r = baseRadius;
    
    plSeq<plVector3> interleaved_vertices; 
    plSeq<PLuint>    indices;
    
    for (int j = 0; j < stacks; j++) 
    {
        PLint base = interleaved_vertices.size()/2;
        
        for (int i = 0; i <= slices; i++) 
        {
            if (i == slices) 
            {
                x = sin(0.0f);
                y = cos(0.0f);
            } else {
                x = sin((i * da));
                y = cos((i * da));
            }
            
            interleaved_vertices.add( plVector3(x * r, y * r, z) );  // position
            interleaved_vertices.add( plVector3(x, y, nz) );         // normal

            interleaved_vertices.add( plVector3( x * (r + dr), y * (r + dr), z + dz) );  // position
            interleaved_vertices.add( plVector3(x, y, nz) );                             // normal                        
        } 
        
        for (int i = 0; i < slices*2; i+=2) 
        {
            indices.add(base+i);
            indices.add(base+i+1);
            indices.add(base+i+2);

            indices.add(base+i+1);
            indices.add(base+i+3);
            indices.add(base+i+2);
        }
        r += dr;
        z += dz;
        
    }
    
    // set VBOs and VAO
    setBuffers(interleaved_vertices, indices);
}       

// disk
plShapeMesh::plShapeMesh(float innerRadius, float outerRadius, int slices, int loops, bool up )
{
    float da, dr;

    plVector3 normal = (up) ? plVector3(0.0f, 0.0f, 1.0f) : plVector3(0.0f, 0.0f, -1.0f);

    da = 2.0f * PL_PI / slices;
    dr = (outerRadius - innerRadius) /  loops;

    float sa, ca;
    float r1 = innerRadius;

    plSeq<plVector3> interleaved_vertices; 
    plSeq<PLuint>    indices;

    for (int l = 0; l < loops; l++) 
    {
        float r2 = r1 + dr;
        
        PLint base = interleaved_vertices.size()/2;

        if (up) 
        {
            for (int s = 0; s <= slices; s++) 
            {
                float a = (s == slices) ? 0.0f : s * da;
                
                sa = sin(a);
                ca = cos(a);
                
                interleaved_vertices.add( plVector3(r2 * sa, r2 * ca, 0.0f) );  // position
                interleaved_vertices.add( normal );                             // normal
                
                interleaved_vertices.add( plVector3(r1 * sa, r1 * ca, 0.0f) );  // position
                interleaved_vertices.add( normal );                             // normal
            }
        }
        else 
        {
            for (int s = slices; s >= 0; s--) 
            {
                float a = (s == slices) ? 0.0f : s * da;
                
                sa = sin(a);
                ca = cos(a);
                
                interleaved_vertices.add( plVector3(r2 * sa, r2 * ca, 0.0f) );  // position
                interleaved_vertices.add( normal );                             // normal
                
                interleaved_vertices.add( plVector3(r1 * sa, r1 * ca, 0.0f) );  // position
                interleaved_vertices.add( normal );                             // normal
            }
        }
        
        r1 = r2;
        
        for (int i = 0; i < slices*2; i+=2) 
        {
            indices.add(base+i);
            indices.add(base+i+1);
            indices.add(base+i+2);

            indices.add(base+i+1);
            indices.add(base+i+3);
            indices.add(base+i+2);
        }
        
    }
    
    // set VBOs and VAO
    setBuffers(interleaved_vertices, indices);
}

