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
    /*
    // ensure transform is orthogonal
    plVector3 x = y ^ _transform.z();   
    _transform.set( x, y, origin );
    // find surface normal
    plIntersection intersecion = _mesh->rayIntersect( origin, -y, true );
    //_surfaceNormal = intersecion.normal;
    */
}

