#include "plDraw.h"
 

void plDraw::arrow( const plVector3 &origin, const plVector3 &direction, PLfloat length, PLfloat scale)
{
    plModelStack::push(); 
    {
        // set to origin
        plModelStack::translate( origin.x, origin.y, origin.z );
        plDraw::arrow( direction, length, scale);
    }
    plModelStack::pop(); 
}


void plDraw::arrow( const plVector3 &direction, PLfloat length, PLfloat scale)
{      
    static plMesh cone(PL_HEAD_RADIUS, 0.0f, PL_ARROW_LENGTH, 30, 1);

    plMatrix44 rot; rot.setRotation( plVector3(0,0,1), direction.normalize());

    plModelStack::push(); 
    {               
        // set to direction
        plModelStack::mult( rot );  
        plModelStack::scale(scale, scale, scale); 
        plDraw::cylinder(scale*PL_HANDLE_RADIUS, length/scale);
        plModelStack::translate( 0, 0, length/scale );
        cone.draw();
        plDraw::disk(PL_HEAD_RADIUS);
    } 
    plModelStack::pop();   
}


void plDraw::knob( const plVector3 &origin, const plVector3 &direction, PLfloat length, PLfloat scale)
{
    plModelStack::push(); 
    {
        // set to origin
        plModelStack::translate( origin.x, origin.y, origin.z );
        plDraw::knob( direction, length, scale);
    }
    plModelStack::pop();    
}


void plDraw::knob(const plVector3 &direction, PLfloat length, PLfloat scale)
{
    plMatrix44 rot; rot.setRotation( plVector3(0,0,1), direction.normalize());
    
    plModelStack::push(); 
    {
        // set to direction
        plModelStack::mult( rot );
        plDraw::cylinder(scale*PL_HANDLE_RADIUS, length);
        plModelStack::translate( 0, 0, length/scale );
        plDraw::cylinder(scale*PL_HEAD_RADIUS, PL_CIRCLE_LENGTH);
        plDraw::disk(scale*PL_HEAD_RADIUS);
        plModelStack::translate( 0, 0, PL_CIRCLE_LENGTH );
        plDraw::disk(scale*PL_HEAD_RADIUS);
    } 
    plModelStack::pop();     
}


void plDraw::sphere(float radius)
{
    static plMesh sphere(1, 20, 20);

    plModelStack::push();    
    {
        plModelStack::scale(radius, radius, radius);
        sphere.draw();
    }
    plModelStack::pop();
}


void plDraw::sphere(const plVector3 &origin, float radius)
{
    plModelStack::push();  
    {
        plModelStack::translate( origin.x, origin.y, origin.z );
        plDraw::sphere( radius );
    }
    plModelStack::pop();  
}


void plDraw::cylinder(float radius, float height)
{
    static plMesh cylinder(1.0f, 1.0f, 1.0f, 30, 1);    

    plModelStack::push();
    {
        plModelStack::scale(radius, radius, height);        
        cylinder.draw();
    }
    plModelStack::pop();
}

void plDraw::disk(const plVector3 &origin, float radius)
{
    plModelStack::push();
    {
        plModelStack::translate( origin.x, origin.y, origin.z );
        plDraw::disk( radius );       
    }
    plModelStack::pop();
}

void plDraw::disk(float radius)
{
    static plMesh disk(0.0f, 1.0f, 20, 20);   

    plModelStack::push();
    {
        plModelStack::scale(radius, radius, radius);        
        disk.draw();
    }
    plModelStack::pop();
}

/*
void plDrawScope(const plVector3 &pos, const plVector3 &rotAxis, PLfloat rotAngle, PLbool visible)
{
    // Draw the TA104 arthroscope
    static plMesh c1(1.5f, 2.0f, 120.0f, 16, 4);
    static plMesh c2(4.0f, 4.0f, 30.0f, 16, 4);
    static plMesh c3(8.0f, 8.0f, 60.0f, 16, 4);
    static plMesh c4(8.0f, 0.0f, 0.0f, 16, 4);

    if (visible)
        glColor3f(0.4,0.4,0.4);
    else
        glColor3f(1.0,0.25,0.05);

    glPushMatrix();
    {
        glTranslatef(pos.x, pos.y, pos.z);
        glRotatef(rotAngle,rotAxis.x,rotAxis.y,rotAxis.z);
        c1.draw();
        glPushMatrix();
        {
            glTranslatef(0, 0, 120);
            c2.draw();
            glPushMatrix();
            {
                glTranslatef(0,0,30);
                c3.draw();
                glTranslatef(0,0,60);
                c4.draw();
            }
            glPopMatrix();
        }    
        glPopMatrix();
    }
    glPopMatrix();
} 

void plDrawProbe(const plVector3 &pos, const plVector3 &rotAxis, PLfloat rotAngle, PLbool visible) 
{
    static plMesh c1(0.0f, 1.0f, 4.0f, 16, 4);
    static plMesh c2(1.0f, 2.5f, 124.0f, 16, 4);
    
    // Draw the TA002 probe
    if (visible)
        glColor3f(0.6,0.6,0.6);
    else
        glColor3f(1.0,0.3,0.1);
   
    glPushMatrix();
    {
        glTranslatef(pos.x, pos.y, pos.z);
        glRotatef(rotAngle,rotAxis.x,rotAxis.y,rotAxis.z);
        c1.draw();
        glPushMatrix();
        {
            glTranslatef(0, 0, 4);
            c2.draw();
        }
        glPopMatrix();
    }
    glPopMatrix();
}
*/
