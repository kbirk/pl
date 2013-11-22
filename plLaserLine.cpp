#include "plLaserLine.h"


plLaserLine::plLaserLine( const plVector3 &position, const plVector3 &direction, PLfloat length )
    : _techniqueEnum( techniqueEnum ), _position( position ), _direction( direction ), _length( length )
{
}


void plLaserLine::extractRenderComponents( plRenderMap& renderMap ) const
{
    plRenderer::queue( plCylinder( PL_PLAN_TECHNIQUE, _position, _direction, 0.1f, _length ) );  
}
