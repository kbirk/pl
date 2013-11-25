#include "plLaserLine.h"


plLaserLine::plLaserLine()
{
}


plLaserLine::plLaserLine( const plVector3 &position, const plVector3 &direction, PLfloat length )
    : _position( position ), _direction( direction ), _length( length )
{
}


void plLaserLine::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    plRenderer::queue( plCylinder( technique, _position, _direction, 0.1f, _length ) );  
}


void plLaserLine::extractRenderComponents( plRenderMap& renderMap ) const
{
    extractRenderComponents( renderMap, PL_PLAN_TECHNIQUE );
}
