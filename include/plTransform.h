#pragma once

#include "plCommon.h"
#include "plMatrix44.h"
#include "plRenderable.h"
#include "plRenderer.h"
#include "plString.h"
#include "plVector3.h"

class plTransform : public plRenderable {
public:
    plTransform();
    plTransform(const std::vector<plString>& row);
    plTransform(const plVector3& x, const plVector3& y, const plVector3& origin);
    plTransform(const plMatrix44& matrix);

    const plVector3& x() const { return _x; }
    const plVector3& y() const { return _y; }
    const plVector3& z() const { return _z; }
    const plVector3& origin() const { return _origin; }
    const plMatrix44& matrix() const { return _transform; }
    plMatrix44 rotation() const
    {
        plMatrix44 m(matrix());
        m.setColumn(3, plVector3(0, 0, 0));
        return m;
    }

    void x(const plVector3& x)
    {
        _x = x.normalize();
        _compute();
    }
    void y(const plVector3& y)
    {
        _y = y.normalize();
        _compute();
    }
    void z(const plVector3& z)
    {
        _z = z.normalize();
        _compute();
    }
    void origin(const plVector3& origin)
    {
        _origin = origin;
        _compute();
    }

    void set(const plVector3& x, const plVector3& y);
    void set(const plVector3& x, const plVector3& y, const plVector3& origin);
    void set(const plVector3& x, const plVector3& y, const plVector3& z, const plVector3& origin);

    plVector3 apply(const plVector3& v) const;
    plVector3 applyNormal(const plVector3& v) const;
    plVector3 applyInverse(const plVector3& v) const;
    plVector3 applyNormalInverse(const plVector3& v) const;
    float32_t squaredDistToAxis(const plVector3& v) const;
    float32_t projectedDistOnAxis(const plVector3& v) const;

    void extractRenderComponents(plRenderMap& renderMap) const;
    void extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const;

    plTransform operator*(const plTransform& transform) const;
    plTransform operator*(const plMatrix44& matrix) const;

private:
    plMatrix44 _transform;
    plVector3 _x, _y, _z;
    plVector3 _origin;

    void _compute();
};

std::ostream& operator<<(std::ostream& out, const plTransform& t);
