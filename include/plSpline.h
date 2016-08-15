#pragma once

#include "plCommon.h"
#include "plVector3.h"
#include "plVector4.h"
#include "plBoundary.h"
#include "plRenderable.h"
#include "plEditable.h"
#include "plColourMap.h"
#include "plOctreeMesh.h"
#include "plVAO.h"
#include "plRenderer.h"
#include "plTimer.h"

#define PL_SPLINE_RESOLUTION                60.0f // higher number results in worse performance, but
                                                  // higher accuracy in automated planning surface area
                                                  // coverage results
#define PL_SPLINE_COLOUR_MAP_RANGE          1.5f
#define PL_SPLINE_NO_DATA_COLOUR            (plVector3(0.2, 0.2, 0.2))

class plSpline : public plBoundary
{
    public:

        plSpline();
        plSpline(const plMesh &mesh);
        plSpline(const plMesh &mesh, const std::vector<plString> &row);

        const plMesh& surfaceMesh() const { return _surfaceMesh; }

        void extractRenderComponents(plRenderMap& renderMap) const;
        void extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const;

        uint32_t addPointAndNormal(const plVector3 &point, const plVector3 &normal);
        void movePointAndNormal(uint32_t index, const plVector3 &point, const plVector3 &normal);
        void removePointAndNormal(uint32_t index);

        void   clear();

    private:

        std::time_t _lastUpdate;

        plMesh _surfaceMesh;
        std::shared_ptr<plVAO> _surfaceVAO;

        std::vector<plVector3> _averageCornerNormals() const;

        void _computeTangents(
            std::vector<float32_t> &st,
            std::vector<float32_t> &tt,
            const std::vector<plVector3> &p,
            const std::vector<plVector3> &n) const;

        void _computeHermite();

        std::time_t _timeSinceLastUpdate();

};
