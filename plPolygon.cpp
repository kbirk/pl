#include "plPolygon.h"

std::ostream& operator << ( std::ostream &stream, const plPolygon &p ) {

    stream << "Normal    = " << p._normal << "\n";
    for (PLuint i = 0; i < p._points.size(); i++)
    {
        stream << "Vertex " << i << " = " << p._points[i] << "\n";
    } // end for

}
