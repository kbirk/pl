#include "plPlug.h"

plPlug::plPlug()
{

}

void plPlug::translate( const plVector3 &translation )
{                 
    plIntersection intersection = model->bone.rayIntersect( transform.origin() + translation, -transform.y() );  

    if (intersection.exists)
    {   
        PLfloat normalRadius = 6.0f;
        plVector3 normal = model->bone.getAverageNormal( normalRadius, transform.origin(), transform.y() );  

        // translate       
        plVector3 y      = normal;
        plVector3 x      = y ^ transform.z();                       
        plVector3 origin = intersection.point;   
        transform.set( x, y, origin); 
    }
}


void plPlug::translateX( PLfloat distance, const plVector3 &planeNormal )
{                 
    plIntersection intersection = model->bone.rayIntersect( transform.origin() + distance*transform.x(), -transform.y() );  

    if (intersection.exists)
    {   
        PLfloat normalRadius = 6.0f;
        plVector3 normal = model->bone.getAverageNormal( normalRadius, transform.origin(), transform.y() );  
             
        // translating along x                    
        plVector3 x = (normal ^ planeNormal);                       
        plVector3 y = normal;                        
        plVector3 z = (x ^ y);
        plVector3 origin = intersection.point;
        transform.set( x, y, z, origin); 
    }
}


void plPlug::translateZ( PLfloat distance, const plVector3 &planeNormal )
{                 
    plIntersection intersection = model->bone.rayIntersect( transform.origin() + distance*transform.z(), -transform.y() );  

    if (intersection.exists)
    {   
        PLfloat normalRadius = 6.0f;
        plVector3 normal = model->bone.getAverageNormal( normalRadius, transform.origin(), transform.y() );  
        
        // translate       
        plVector3 z = (normal ^ planeNormal);                       
        plVector3 y = normal;                         
        plVector3 x = (y ^ z);                         
        plVector3 origin = intersection.point;   
        transform.set( x, y, z, origin); 
    }
}


void plPlug::rotate( const plVector3 &axis, PLfloat angle_degrees )
{     
    plMatrix44 rot; rot.setRotationD(angle_degrees,  axis);

    transform.set( rot * transform.x(), rot * transform.y() );   
}
