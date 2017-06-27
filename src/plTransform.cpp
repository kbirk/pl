#include "plTransform.h"

plTransform::plTransform()
    : _x(plVector3(1, 0, 0))
    , _y(plVector3(0, 1, 0))
    , _origin(plVector3(0, 0, 0))
{
    _compute();
}

plTransform::plTransform(const plVector3& x, const plVector3& y, const plVector3& origin)
    : _x(x)
    , _y(y)
    , _origin(origin)
{
    _compute();
}

plTransform::plTransform(const std::vector<plString>& row)
    : _x(row[2])
    , _y(row[3])
    , _origin(row[1])
{
    _compute();
}

plTransform::plTransform(const plMatrix44& matrix)
    : _x(matrix(0, 0), matrix(1, 0), matrix(2, 0))
    , _y(matrix(0, 1), matrix(1, 1), matrix(2, 1))
    , _origin(matrix(0, 3), matrix(1, 3), matrix(2, 3))
{
    _compute();
}

void plTransform::_compute()
{
    _z = (_x ^ _y).normalize(); // re-compute z to ensure it is orthogonal to x and y

    if (fabs(_x * _y) > 0.001) {
        LOG_WARN("x and y are not perpendicular (dot product = " << _x * _y);
    }

    _transform.setColumn(0, _x.x, _x.y, _x.z, 0.0f);
    _transform.setColumn(1, _y.x, _y.y, _y.z, 0.0f);
    _transform.setColumn(2, _z.x, _z.y, _z.z, 0.0f);
    _transform.setColumn(3, _origin.x, _origin.y, _origin.z, 1.0f);
}

void plTransform::set(const plVector3& x, const plVector3& y)
{
    _x = x.normalize();
    _y = y.normalize();
    _compute();
}

void plTransform::set(const plVector3& x, const plVector3& y, const plVector3& origin)
{
    _x = x.normalize();
    _y = y.normalize();
    _origin = origin;
    _compute();
}

void plTransform::set(const plVector3& x, const plVector3& y, const plVector3& z, const plVector3& origin)
{
    _x = x.normalize();
    _y = y.normalize();
    _z = z.normalize();
    _origin = origin;
    _compute();
}

plVector3 plTransform::apply(const plVector3& v) const
{
    return _transform * v;
}

plVector3 plTransform::applyNormal(const plVector3& v) const
{
    plMatrix44 rot = _transform;
    rot.setColumn(3, 0.0f, 0.0f, 0.0f, 1.0f);
    return rot * v;
}

plVector3 plTransform::applyInverse(const plVector3& v) const
{
    plVector3 p = v - _origin;
    return plVector3(p * _x, p * _y, p * _z);
}

plVector3 plTransform::applyNormalInverse(const plVector3& v) const
{
    return plVector3(v * _x, v * _y, v * _z);
}

float32_t plTransform::squaredDistToAxis(const plVector3& v) const
{
    // v is already in the *local* coordinate system of the graft
    static plVector3 axis(0, 1, 0);
    return (v - (v * axis) * axis).squaredLength();
}

float32_t plTransform::projectedDistOnAxis(const plVector3& v) const
{
    // v is already in the *local* coordinate system of the graft
    static plVector3 axis(0, 1, 0);
    return v * axis;
}

void plTransform::extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const
{
    if (!_isVisible)
        return;

    // draw axis
    plRenderer::queueAxis(technique, _origin, _x, _y);
}

void plTransform::extractRenderComponents(plRenderMap& renderMap) const
{
    extractRenderComponents(renderMap, PL_PLAN_TECHNIQUE);
}

plTransform plTransform::operator*(const plTransform& transform) const
{
    plMatrix44 m = _transform * transform.matrix();

    return plTransform(plVector3(m(0, 0), m(1, 0), m(2, 0)),
        plVector3(m(0, 1), m(1, 1), m(2, 1)),
        plVector3(m(0, 3), m(1, 3), m(2, 3)));
}

plTransform plTransform::operator*(const plMatrix44& matrix) const
{
    plMatrix44 m = _transform * matrix;

    return plTransform(plVector3(m(0, 0), m(1, 0), m(2, 0)),
        plVector3(m(0, 1), m(1, 1), m(2, 1)),
        plVector3(m(0, 3), m(1, 3), m(2, 3)));
}

std::ostream& operator<<(std::ostream& out, const plTransform& t)
{
    out << t.origin() << "," << t.x() << "," << t.y();
    return out;
}
