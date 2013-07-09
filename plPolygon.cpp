#include "plPolygon.h"


std::ostream& operator << ( std::ostream &stream, const plPolygon &p ) {
    stream << "Normal    = " << p.getNormal() << "\n";
    for (PLuint i = 0; i < p.getPolygonSize(); i++)
    {
        stream << "Vertex " << i << " = " << p.getPoint(i) << "\n";
    } // end for
}


void plPolygon::setPoint ( const PLuint index , const plVector3 &value )
{
    if (index >= _points.size()) {
        std::cerr << "Error: plPolygon setPoint(index,value) - index " << index << " is out of range, maximum is currently " << _points.size() << std::endl;
    }
    _points[index] = value;
} // end void function setPoint()


void plPolygon::setNormal( const plVector3 &value )
{
    _normal = value;
}// end void function setNormal()


void plPolygon::addPoint ( const plVector3 &value ) {
    _points.add(value);
}


void plPolygon::removePoint ( const PLuint &index ) {
    if (index >= _points.size()) {
        std::cerr << "Error: plPolygon setPoint(index,value) - index " << index << " is out of range, maximum is currently " << _points.size() << std::endl;
    }
    _points.remove(index);
}
