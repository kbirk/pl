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
    // translate  
    plVector3 x = y ^ _transform.z();
    
    _transform.set( x, y, origin );

    plIntersection intersecion = _mesh->rayIntersect( origin, -y );

    _surfaceNormal = intersecion.normal;
}

/*
void plPlug::rotate( const plVector3 &axis, PLfloat angleDegrees )
{     
    plMatrix44 rot; rot.setRotationD( angleDegrees,  axis );

    _transform.set( rot * _transform.x(), rot * _transform.y() );   
}
*/

