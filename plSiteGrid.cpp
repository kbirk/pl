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

void plSiteGrid::calcArea( const plSeq<plTriangle> &triangles )
{
    _area = 0;
    for (PLuint i=0; i< triangles.size(); i++)
    {
        _area += triangles[i].getArea();
    }    
}        

        
    

