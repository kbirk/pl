#include "plPlug.h"


plPlug::plPlug( PLuint type )
    : _type( type )
{
}


plPlug::plPlug( const plMesh& mesh, PLuint type, const plTransform& surfaceTransform, const plTransform& rotationalOffset )
    :   plMeshSpecific( mesh ), 
        _type( type ), 
        _surfaceTransform( surfaceTransform ), 
        _rotationalOffset( rotationalOffset )
{
}


const plTransform& plPlug::transform() const 
{ 
    return _surfaceTransform;  
}


const plTransform& plPlug::rotation() const 
{ 
    return _rotationalOffset;   
}
  
        
plMatrix44 plPlug::matrix() const
{
    plMatrix44 m( _rotationalOffset.matrix() * _surfaceTransform.rotation() );
    m.setColumn( 3, _surfaceTransform.origin() );
    return m;
}        


plTransform plPlug::finalTransform() const 
{ 
    plTransform t( _rotationalOffset * _surfaceTransform.rotation() );
    t.origin( _surfaceTransform.origin() );
    return t; 
}


void plPlug::move( const plVector3& origin, const plVector3& y )
{
    // find new surface normal
    //plIntersection intersection = _mesh->rayIntersect( origin, -_surfaceTransform.y(), true );
    // get x axis
    plVector3 x = y ^ _surfaceTransform.z();  
    // set transform
    _surfaceTransform.set( x, y, origin );
}


void plPlug::rotate( const plVector3& y )
{
    const plVector3& surfaceNormal = _surfaceTransform.y();
    
    // check angle between new y and surface normal
    PLfloat angle = acos( surfaceNormal * y );

    plVector3 finalY = y;

    // if past threshold, set to
    if ( angle > PL_DEG_TO_RAD( PL_MAX_GRAFT_ROTATION ) )
    {
        // find vector in plane of surface normal and new y that is orthogonal to surface normal
        plVector3 planeNormal = y ^ surfaceNormal;               
        plVector3 ortho = ( surfaceNormal ^ planeNormal ).normalize();                
        // trig to find scaling of new vector on plane
        finalY = ( surfaceNormal*cos( PL_DEG_TO_RAD( PL_MAX_GRAFT_ROTATION ) ) + ortho*sin( PL_DEG_TO_RAD( PL_MAX_GRAFT_ROTATION ) ) ).normalize();
    } 

    // get the rotation matrix
    plMatrix44 rotation;     rotation.setRotation( surfaceNormal, finalY );

    _rotationalOffset = plTransform( rotation ); 
}

