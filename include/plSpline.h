#pragma once

#include "plBoundary.h"
#include "plColorMap.h"
#include "plCommon.h"
#include "plEditable.h"
#include "plOctreeMesh.h"
#include "plRenderable.h"
#include "plRenderer.h"
#include "plTimer.h"
#include "plVAO.h"
#include "plVector3.h"
#include "plVector4.h"

// higher number results in worse performance, but higher accuracy in automated
// planning surface area coverage results
#define PL_SPLINE_RESOLUTION 60.0f
#define PL_SPLINE_COLOR_MAP_RANGE 1.5f
#define PL_SPLINE_NO_DATA_COLOR (plVector3(0.2, 0.2, 0.2))

class plSpline : public plBoundary {
public:
    plSpline(std::shared_ptr<plMesh> mesh);
    plSpline(std::shared_ptr<plMesh> mesh, const std::vector<plString>& row);

    std::shared_ptr<plMesh> surfaceMesh() const { return _surfaceMesh; }

    void extractRenderComponents(plRenderMap& renderMap) const;
    void extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const;

    uint32_t addPointAndNormal(const plVector3& point, const plVector3& normal);
    void movePointAndNormal(uint32_t index, const plVector3& point, const plVector3& normal);
    void removePointAndNormal(uint32_t index);

    void clear();

private:
    std::time_t _lastUpdate;

    std::shared_ptr<plMesh> _surfaceMesh;
    std::shared_ptr<plVAO> _surfaceVAO;

    std::vector<plVector3> _averageCornerNormals() const;

    void _computeTangents(
        std::vector<float32_t>& st,
        std::vector<float32_t>& tt,
        const std::vector<plVector3>& p,
        const std::vector<plVector3>& n) const;

    void _computeHermite();

    std::time_t _timeSinceLastUpdate();
};
