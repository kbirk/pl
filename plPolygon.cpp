#include "plPolygon.h"

plEdge::plEdge()
{
    pt1 = plVector3(0,0,0);
    pt2 = plVector3(0,0,0);
}

plEdge::plEdge(const plVector3 &v1, const plVector3 &v2)
{
    pt1 = v1;
    pt2 = v2;
}

PLbool plEdge::contains(const plVector3 &point, const PLfloat &epsilon)
{
    return ((pt1 - point).length() <= epsilon || (pt2 - point).length() <= epsilon);
}

plVector3 plEdge::otherPt(const plVector3 &point, const PLfloat &epsilon)
{
    if ((pt1 - point).length() <= epsilon)
        return pt2;
    else if ((pt2 - point).length() <= epsilon)
        return pt1;
    // shouldn't proceed past here if one of this edge's points are used, but this basically just says that no match was found
    std::cout << "Warning in plEdge::otherPt() - point " << point << " was not found! returning vector of zeros!" << std::endl;
    return plVector3(0,0,0);
}

PLbool plEdge::equals(const plEdge &e2, const PLfloat &epsilon)
{
    if ( ( (e2.pt1 - pt1).length() <= epsilon && ((e2.pt2 - pt2).length() <= epsilon) ) ||
         ( (e2.pt2 - pt1).length() <= epsilon && ((e2.pt1 - pt2).length() <= epsilon) ) )
        return true;
    return false;
}

std::ostream& operator << ( std::ostream &stream, const plEdge &e )
{
    stream << e.pt1 << " " << e.pt2 << "\n";
    return stream;
}

std::ostream& operator << ( std::ostream &stream, const plPolygon &p ) 
{
    stream << "Normal    = " << p.normal << "\n";
    for (PLuint i = 0; i < p.points.size(); i++)
    {
        stream << "Vertex " << i << " = " << p.points[i] << "\n";
    } 
    return stream;
} 

// SPECIALIZATION OF SEQ findIndex FUNCTION!
// Find and return index of the first instance of an element
// if it doesn't exist, return -1
template <>
PLint plSeq<plEdge>::findIndex( const plEdge &x )
{
    for (PLint i=0; i<_numElements; i++)
        if (_data[i].equals(x))
            return i;

    return -1;
}

