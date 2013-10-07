#include "plPlug.h"

plPlug::plPlug()
{
}


plPlug::plPlug( PLuint modelID, const plBoneAndCartilage &model, const plTransform &t  )
    : transform(t), plModelSpecific( modelID, model )
{
}


PLbool plPlug::_surfaceIntersection( plVector3 &point, plVector3 &normal, const plVector3 &translation ) const
{
    // intersect cartilage and bone
    plIntersection boneIntersection = _model->bone.rayIntersect     ( transform.origin() + translation, -transform.y() );  
    if ( boneIntersection.exists )
    {    
        // get new graft origin (bone intersection point) and y axis (averaged normals of all triangles in PL_NORMAL_SMOOTHING_RADIUS )
        point  = boneIntersection.point;
        normal = _model->bone.getAverageNormal( PL_NORMAL_SMOOTHING_RADIUS, transform.origin(), transform.y() );        
    } 
    return boneIntersection.exists; 
}


void plPlug::translate( const plVector3 &translation )
{            
    plVector3 y, origin;
    
    if ( _surfaceIntersection( origin, y, translation ) )
    {
        // translate  
        plVector3 x = y ^ transform.z();
        transform.set( x, y, origin);
    }
}


void plPlug::translateX( PLfloat distance, const plVector3 &planeNormal )
{  
    plVector3 y, origin;
    
    if ( _surfaceIntersection( origin, y, transform.x()*distance ) )
    {
        // translate 
        plVector3 x = (y ^ planeNormal);    
        transform.set( x, y, origin);
    }
}


void plPlug::translateZ( PLfloat distance, const plVector3 &planeNormal )
{   
    plVector3 y, origin;
    
    if ( _surfaceIntersection( origin, y, transform.z()*distance ) )
    {
        // translate          
        plVector3 z = (y ^ planeNormal); 
        plVector3 x = (y ^ z);
        transform.set( x, y, z, origin);
    } 
}


void plPlug::rotate( const plVector3 &axis, PLfloat angleDegrees )
{     
    plMatrix44 rot; rot.setRotationD(angleDegrees,  axis);

    transform.set( rot * transform.x(), rot * transform.y() );   
}


