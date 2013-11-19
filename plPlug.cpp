#include "plPlug.h"

plPlug::plPlug()
{
}


plPlug::plPlug( PLuint modelID, const plBoneAndCartilage &model, const plTransform &t  )
    : _transform(t), plModelSpecific( modelID, model )
{
}

/*
PLbool plPlug::_surfaceIntersection( plVector3 &point, plVector3 &normal, const plVector3 &translation ) const
{
    // intersect cartilage and bone
    plIntersection boneIntersection = _model->bone.rayIntersect     ( _transform.origin() + translation, -_transform.y() );  
    if ( boneIntersection.exists )
    {    
        // get new graft origin (bone intersection point) and y axis (averaged normals of all triangles in PL_NORMAL_SMOOTHING_RADIUS )
        point  = boneIntersection.point;
        normal = _model->bone.getAverageNormal( PL_NORMAL_SMOOTHING_RADIUS, _transform.origin(), _transform.y() );        
    } 
    return boneIntersection.exists; 
}
*/

void plPlug::move( const plVector3 &origin, const plVector3 &y )
{
    //plMatrix44 rot;  rot.setRotation( _transform.y(), y );
    
    //plVector3 nx = rot * _transform.x();
    //plVector3 ny = rot * _transform.y();

    // translate  
    plVector3 x = y ^ _transform.z();
    
    _transform.set( x, y, origin);
}

/*
void plPlug::translate( const plVector3 &translation )
{            
    plVector3 y, origin;
    
    if ( _surfaceIntersection( origin, y, translation ) )
    {
        // translate  
        plVector3 x = y ^ _transform.z();
        
        _transform.set( x, y, origin);
    }
}


void plPlug::translateX( PLfloat distance, const plVector3 &planeNormal )
{  
    plVector3 y, origin;
    
    if ( _surfaceIntersection( origin, y, _transform.x()*distance ) )
    {
        // translate 
        plVector3 x = (y ^ planeNormal);    
        
        _transform.set( x, y, origin);
    }
}


void plPlug::translateZ( PLfloat distance, const plVector3 &planeNormal )
{   
    plVector3 y, origin;
    
    if ( _surfaceIntersection( origin, y, _transform.z()*distance ) )
    {
        // translate          
        plVector3 z = (y ^ planeNormal); 
        plVector3 x = (y ^ z);
        
        _transform.set( x, y, z, origin);
    } 
}
*/

void plPlug::rotate( const plVector3 &axis, PLfloat angleDegrees )
{     
    plMatrix44 rot; rot.setRotationD( angleDegrees,  axis );

    //_rotationTransform.set( rot * _rotationTransform.x(), rot * _rotationTransform.y() );

    //_updateTransform();
        
    _transform.set( rot * _transform.x(), rot * _transform.y() );   
}


