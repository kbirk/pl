#pragma once

#include "plCommon.h"
#include "plString.h"
#include "plVector3.h"
#include "plMath.h"

class plIntersection
{
    public:

        bool exists;
        plVector3 point;
        plVector3 normal;
        float32_t t;

        plIntersection(bool e)
            : exists(e)
        {
        }

        plIntersection(const plVector3 &p, const plVector3 &n, float32_t tt)
            : exists(true), point(p), normal(n), t(tt)
        {
        }

};

class plTriangle
{
    public:

        plTriangle();
        plTriangle(const plVector3 &n, const plVector3 &p0, const plVector3 &p1, const plVector3 &p2);
        plTriangle(const plVector3 &p0, const plVector3 &p1, const plVector3 &p2);

        const plVector3 &point0() const   { return _points[0]; }
        const plVector3 &point1() const   { return _points[1]; }
        const plVector3 &point2() const   { return _points[2]; }
        const plVector3 &normal() const   { return _normal;    }
        const plVector3 &centroid() const { return _centroid;  }
        float32_t radius() const          { return _radius;    }

        void point0(const plVector3 &point);
        void point1(const plVector3 &point);
        void point2(const plVector3 &point);

        void swapVertices0And1();

        bool operator== (const plTriangle&) const;
        const plVector3& operator[](int32_t index) const { return _points[index]; }

        bool isInside(const plVector3 &point) const;

        plIntersection rayIntersect(
            const plVector3 &rayStart,
            const plVector3 &rayDir,
            bool ignoreBehindRay = false,
            bool backFaceCull = false) const;

        plVector3 barycentricCoords(const plVector3 &point) const;

        bool contains(const plVector3&, const float32_t& epsilon = PL_EPSILON) const;

        float32_t getArea() const;

        plVector3 closestPointTo(const plVector3& point) const;

        plVector3 closestPointOnEdge(uint32_t edgeNum, const plVector3& point) const;

    private:

        std::vector<plVector3> _points;
        plVector3 _normal;
        plVector3 _centroid;
        float32_t _radius;

        void _recalculate();
        void _calcRadius();
};
