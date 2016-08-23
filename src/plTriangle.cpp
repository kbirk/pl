#include "plTriangle.h"

plTriangle::plTriangle()
    : _points(3, plVector3(0, 0, 0)),
      _normal(0, 0, 0),
      _centroid(0, 0, 0)
{
    _calcRadius();
}


plTriangle::plTriangle(const plVector3 &n, const plVector3 &p0, const plVector3 &p1, const plVector3 &p2)
    :   _centroid(0.333333f * (p0 + p1 + p2))
{
    _points.push_back(p0);
    _points.push_back(p1);
    _points.push_back(p2);

    if (n.x == 0.0f && n.y == 0.0f && n.z == 0.0f)
    {
        _normal = ((p1-p0)^(p2-p0)).normalize();
    }
    else
    {
        _normal = n;
    }
    _calcRadius();
}


plTriangle::plTriangle(const plVector3 &p0, const plVector3 &p1, const plVector3 &p2)
    : _normal(((p1 - p0) ^ (p2 - p0)).normalize()),
      _centroid(0.333333f * (p0 + p1 + p2))
{
    _points.push_back(p0);
    _points.push_back(p1);
    _points.push_back(p2);
    _calcRadius();
}


void plTriangle::point0(const plVector3 &point)
{
    _points[0] = point;
    _recalculate();
}


void plTriangle::point1(const plVector3 &point)
{
    _points[1] = point;
    _recalculate();
}


void plTriangle::point2(const plVector3 &point)
{
    _points[2] = point;
    _recalculate();
}


void plTriangle::swapVertices0And1()
{
    plVector3 tempPt (_points[0]);
    _points[0] = _points[1];
    _points[1] = tempPt;
    _recalculate();
}


void plTriangle::_recalculate()
{
   _normal = ((_points[1] - _points[0]) ^ (_points[2] - _points[0])).normalize();
   _centroid = 0.333333f * (_points[0] + _points[1] + _points[2]);
   _calcRadius();
}


void plTriangle::_calcRadius()
{
    _radius = PL_MAX_OF_3((_points[0] - _centroid).length(),
                           (_points[1] - _centroid).length(),
                           (_points[2] - _centroid).length());
}


bool plTriangle::isInside(const plVector3 &point) const
{
    // Compute barycentric coords
    float32_t totalAreaDiv = 1 / (((_points[1]-_points[0]) ^ (_points[2]-_points[0])) * _normal);
    float32_t u = (((_points[2]-_points[1]) ^ (point - _points[1])) * _normal) * totalAreaDiv;
    float32_t v = (((_points[0]-_points[2]) ^ (point - _points[2])) * _normal) * totalAreaDiv;

    // Reject if outside triangle
    if (u < 0 || v < 0 || u + v > 1)
        return false;

    return true;
}


plIntersection plTriangle::rayIntersect(const plVector3 &rayStart, const plVector3 &rayDir, bool ignoreBehindRay, bool backFaceCull) const
{
    // Compute ray/plane intersection
    float32_t dn = rayDir * _normal;

    if (dn == 0 || (backFaceCull && dn > 0))
        return plIntersection(false);   // ray is parallel to plane, or coming from behind

    //float32_t dist = _points[0] * _normal;

    float32_t t = ((_points[0]-rayStart) * _normal) / dn;

    if (ignoreBehindRay && t < 0)
        return plIntersection(false);   // plane is behind ray

    plVector3 intPoint = rayStart + t * rayDir;

    // check if point is inside the triangle
    if (!isInside(intPoint))
    {
       return plIntersection(false);
    }
    return plIntersection(intPoint, _normal, t);
}


plVector3 plTriangle::barycentricCoords(const plVector3 &testPoint) const
{
    // we're assuming that testPoint is in-plane with the triangle.

    // Obtained from: http://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
    //   The more efficient solution proposed below seems to sometimes produce inaccurate
    // results (particularly as the determinant approaches 0), so beware that solution.
    plVector3 bary;

    float32_t area012 = _normal * ((_points[1]-_points[0])^(_points[2]-_points[0]));
    float32_t areaT12 = _normal * ((_points[1]-testPoint) ^(_points[2]-testPoint));
    float32_t areaT20 = _normal * ((_points[2]-testPoint) ^(_points[0]-testPoint));

    bary.x = areaT12 / area012;
    bary.y = areaT20 / area012;
    bary.z = 1.0f - bary.x - bary.y;

    return bary;
}


bool plTriangle::contains(const plVector3& pt, const float32_t& epsilon) const
{
    if (epsilon)
    {
        for (uint32_t i = 0; i < _points.size(); i++)
        {
            if ((_points[i] - pt).length() <= epsilon)
            {
                return true;
            }
        }
    }
    else
    {
        for (uint32_t i = 0; i < _points.size(); i++) {
            if (_points[i] == pt)
            {
                return true;
            }
        }
    }
    return false;
}


float32_t plTriangle::getArea() const
{
    return 0.5f * ((_points[1] - _points[0]) ^ (_points[2] - _points[0])).length();
}


bool plTriangle::operator== (const plTriangle& other) const
{
    return (_points[0] == other._points[0] &&
            _points[1] == other._points[1] &&
            _points[2] == other._points[2] &&
            _normal    == other._normal);
}


plVector3 plTriangle::closestPointTo(const plVector3& point) const
{
    plVector3 e0 = closestPointOnEdge(0, point);
    plVector3 e1 = closestPointOnEdge(1, point);
    plVector3 e2 = closestPointOnEdge(2, point);

    float32_t d0 = (e0 - point).squaredLength();
    float32_t d1 = (e1 - point).squaredLength();
    float32_t d2 = (e2 - point).squaredLength();

    if (d0 < d1)
    {
        if (d0 < d2)
            return e0;
        else
            return e2;
    }
    else
    {
        if (d1 < d2)
            return e1;
        else
            return e2;
    }
}


plVector3 plTriangle::closestPointOnEdge(uint32_t edgeIndex, const plVector3& point) const
{
    return plMath::closestPointOnSegment(point, _points[edgeIndex], _points[(edgeIndex + 1) % 3]);
}


// I/O operators
std::ostream& operator << (std::ostream& stream, const plTriangle &p)
{
    stream << "Normal    = " << p.normal() << "\n"
           << "Vertex 0  = " << p.point0() << "\n"
           << "Vertex 1  = " << p.point1() << "\n"
           << "Vertex 2  = " << p.point2() << "" << std::endl;
    return stream;
}
