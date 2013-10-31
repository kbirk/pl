#include "plDraw.h"
 
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
        static plShapeMesh cone(PL_HEAD_RADIUS, 0.0f, PL_ARROW_LENGTH, 30, 1);

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
        static plShapeMesh sphere(1, 20, 20);

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
        static plShapeMesh cylinder(1.0f, 1.0f, 1.0f, 30, 1);    

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

    void disk(const plVector3 &origin, const plVector3 &direction, float radius, PLbool flip)
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
        static plShapeMesh disk(0.0f, 1.0f, 20, 20);   

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
        static plShapeMesh cube(1.f);

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

}

