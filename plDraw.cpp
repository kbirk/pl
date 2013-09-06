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

    /*
    void scope( const plTrackedObject &scope )
    {
        // Draw the TA104 arthroscope
        static plShapeMesh c1(1.5f, 2.0f, 120.0f, 16, 4);
        static plShapeMesh c2(4.0f, 4.0f, 30.0f, 16, 4 );
        static plShapeMesh c3(8.0f, 8.0f, 60.0f, 16, 4 );
        static plShapeMesh c4(8.0f, 0.0f, 0.0f, 16, 4  );

        if (scope.isVisible)
            plColourStack::load(0.4, 0.4, 0.4);
        else
            plColourStack::load(1.0, 0.25, 0.05);

        plModelStack::push();
        {
            plModelStack::translate(scope.getPoint());
            plModelStack::rotate(scope.getRotationAngle(), scope.getRotationAxis());
            c1.draw();
            plModelStack::push();
            {
                plModelStack::translate(0, 0, 120);
                c2.draw();
                plModelStack::push();
                {
                    plModelStack::translate(0,0,30);
                    c3.draw();
                    plModelStack::translate(0,0,60);
                    c4.draw();
                }
                plModelStack::pop();
            }    
            plModelStack::pop();
        }
        plModelStack::pop();
    } 

    void probe( const plTrackedObject &probe )
    {
        static plShapeMesh c1(0.0f, 1.0f, 4.0f, 16, 4);
        static plShapeMesh c2(1.0f, 2.5f, 124.0f, 16, 4);
        
        // Draw the TA002 probe
        if (probe.isVisible)
            plColourStack::load(0.6, 0.6, 0.6);
        else
            plColourStack::load(1.0, 0.3, 0.1);
       
        plModelStack::push();
        {
            plModelStack::translate(probe.getPoint());
            plModelStack::rotate(probe.getRotationAngle(), probe.getRotationAxis());
            c1.draw();
            plModelStack::push();
            {
                plModelStack::translate(0, 0, 4);
                c2.draw();
            }
            plModelStack::pop();
        }
        plModelStack::pop();
    }
    */

}

