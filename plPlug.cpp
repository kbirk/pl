#include "plPlug.h"


plPlug::plPlug( PLuint type )
    : _type( type )
{
}


plPlug::plPlug( const plMesh& mesh, PLuint type, const plTransform &transform, const plVector3 surfaceNormal )
    :  plMeshSpecific( mesh ), _type( type ), _transform( transform ), _surfaceNormal( surfaceNormal )
{
}


void plPlug::move( const plVector3& origin, const plVector3& y )
{
    // ensure transform is orthogonal  
    plVector3 x = y ^ _transform.z();   
    _transform.set( x, y, origin );
    // find surface normal
    plIntersection intersecion = _mesh->rayIntersect( origin, -y, true );
    _surfaceNormal = intersecion.normal;
}
