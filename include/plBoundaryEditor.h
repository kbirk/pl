#pragma once

#include "plCommon.h"

#include "plVector3.h"
#include "plPlan.h"
#include "plEditor.h"
#include "plWindow.h"
#include "plRenderer.h"

class plBoundaryEditor : public plEditor
{
    public:

        plBoundaryEditor();

        void clearSelection();
        void selectBoundary(uint32_t boundaryType, uint32_t boundaryIndex, uint32_t pointIndex);

        bool isBoundarySelected() const { return _selectedBoundary != nullptr; }

        bool processMousePress(int32_t x, int32_t y);
        bool processMouseDrag(int32_t x, int32_t y);
        bool processMouseRelease(int32_t x, int32_t y);

        void addPoint(uint32_t x, uint32_t y, bool selectNewPoint = true);
        void moveSelectedPoint(uint32_t x, uint32_t y);
        void removeSelectedPoint();
        void clearSelectedBoundary();
        void removeSelectedSite();
        void toggleSelectedVisibility();

        void extractRenderComponents(plRenderMap& renderMap) const;
        void extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const;

    private:

        int32_t _selectedSiteIndex;
        std::shared_ptr<plBoundary> _selectedBoundary;
        int32_t _selectedPointIndex;

        void _clearSiteBoundaries();

        void _selectBoundary(std::shared_ptr<plBoundary> boundary, uint32_t boundaryIndex, uint32_t pointIndex);

        void _selectDefectSiteSpline(uint32_t boundaryIndex, uint32_t pointIndex);
        void _selectDefectSiteBoundary(uint32_t boundaryIndex, uint32_t pointIndex);
        void _selectDonorSiteBoundary(uint32_t boundaryIndex, uint32_t pointIndex);

        plIntersection _getBoundaryIntersection(uint32_t x, uint32_t y);

        void _extractMenuRenderComponents(plRenderMap& renderMap) const;

};
