#include "plPolygon.h"


std::ostream& operator << ( std::ostream &stream, const plPolygon &p ) 
{
    stream << "Normal    = " << p.normal << "\n";
    for (PLuint i = 0; i < p.points.size(); i++)
    {
        stream << "Vertex " << i << " = " << p.points[i] << "\n";
    } 
} 
