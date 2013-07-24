#include "plSiteGrid.h"

plSiteGrid::plSiteGrid()
{
}

void plSiteGrid::clear()
{
    _points.clear();
    _normals.clear();
}

void plSiteGrid::addPointAndNormal( const plVector4 &p, const plVector4 &n )
{
    _points.add( p );
    _normals.add( n );
}
        

        
    

