#pragma once

#include "plBoundary.h"
#include "plCommon.h"
#include "plMeshCutter.h"
#include "plSSBO.h"
#include "plTriangle.h"
#include "plVector4.h"

class plPointAndNormal {
public:
    plVector3 point;
    plVector3 normal;

    plPointAndNormal(const plVector3& p, const plVector3& n)
        : point(p)
        , normal(n)
    {
    }

    bool operator<(const plPointAndNormal& pn) const
    {
        if (point.x == pn.point.x) {
            if (point.y == pn.point.y) {
                return point.z < pn.point.z;
            } else {
                return point.y < pn.point.y;
            }
        } else {
            return point.x < pn.point.x;
        }
    }
};

class plPlanningSite {
public:
    std::vector<plTriangle> triangles;
    std::vector<plVector4> gridPoints;
    std::vector<plVector4> gridNormals;
    std::vector<plVector4> boundaryPoints;
    std::vector<plVector4> boundaryNormals;
    float32_t area;
    plVector3 avgNormal;

    plPlanningSite();
    plPlanningSite(
        const std::vector<plTriangle>& triangles,
        std::shared_ptr<plBoundary> boundary,
        bool fineGrain = false);

    uint32_t totalSize() const { return gridPoints.size() + gridNormals.size() + triangles.size() * 4 + boundaryPoints.size() + boundaryNormals.size(); }

    void getData(std::vector<plVector4>& data) const
    {
        _bufferGridData(data);
        _bufferMeshData(data);
        _bufferBoundaryData(data);
    }
    std::shared_ptr<plSSBO> getSSBO() const;

    bool good() const;

private:
    void _bufferGridData(std::vector<plVector4>& data) const;
    void _bufferMeshData(std::vector<plVector4>& data) const;
    void _bufferBoundaryData(std::vector<plVector4>& data) const;

    void _generateCoarseGridPoints();
    void _generateFineGridPoints();
    void _generateBoundaryPoints(std::shared_ptr<plBoundary> boundary);
    void _calcArea();
    void _calcNormal();
};
