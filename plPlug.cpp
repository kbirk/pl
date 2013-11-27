#include "plPlug.h"


plPlug::plPlug( PLuint type )
    : _type( type )
{
}


plPlug::plPlug( PLuint modelID, const plBoneAndCartilage &model, PLuint type, const plTransform &transform, const plVector3 surfaceNormal )
    : _type( type ), _transform( transform ), _surfaceNormal( surfaceNormal ), plModelSpecific( modelID, model )
{
}


void plPlug::move( const plVector3& origin, const plVector3& y, const plVector3& surfaceNormal )
{
    // translate  
    plVector3 x = y ^ _transform.z();
    
    _transform.set( x, y, origin );


    _surfaceNormal = surfaceNormal;
}

/*
void plPlug::rotate( const plVector3 &axis, PLfloat angleDegrees )
{     
    plMatrix44 rot; rot.setRotationD( angleDegrees,  axis );

    _transform.set( rot * _transform.x(), rot * _transform.y() );   
}
*/

