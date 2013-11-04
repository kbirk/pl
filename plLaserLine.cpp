#include "plLaserLine.h"

plLaserLine::plLaserLine()
{

}

plLaserLine::plLaserLine( const plVector3 &origin, const plVector3 &direction,
                PLfloat length, const plVector4 &colour)
{
    this->origin    = origin;
    this->direction = direction;
    this->length    = length;
    this->colour    = colour;
}
