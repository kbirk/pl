#ifndef PL_BOUNDARY_EDITOR_H
#define PL_BOUNDARY_EDITOR_H

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
        void selectBoundary(PLuint boundaryType, PLuint boundaryIndex, PLuint pointIndex );

        PLbool isBoundarySelected() const { return (_selectedBoundary != NULL); }

        PLbool processMouseClick   ( PLint x, PLint y );
        PLbool processMouseDrag    ( PLint x, PLint y );
        PLbool processMouseRelease ( PLint x, PLint y );
        PLbool processJoystickDrag ( PLint x, PLint y );

        void addPoint                ( PLuint x, PLuint y, PLbool selectNewPoint = true );
        void moveSelectedPoint       ( PLuint x, PLuint y );
        void removeSelectedPoint     ();
        void clearSelectedBoundary   ();
        void removeSelectedSite      ();
        void toggleSelectedVisibility();

        void extractRenderComponents( plRenderMap& renderMap ) const;
        void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const;

    private:

        PLint       _selectedSiteIndex;    // use this and above to determine which pl*Site
        plBoundary *_selectedBoundary;
        PLint       _selectedPointIndex;

        void _clearSiteBoundaries();

        void _selectBoundary( plBoundary &boundary, PLuint boundaryIndex, PLuint pointIndex );

        void _selectDefectSiteSpline     ( PLuint boundaryIndex, PLuint pointIndex );
        void _selectDefectSiteBoundary   ( PLuint boundaryIndex, PLuint pointIndex );
        void _selectDonorSiteBoundary    ( PLuint boundaryIndex, PLuint pointIndex );
        void _selectIGuideBoundary       ( PLuint boundaryIndex, PLuint pointIndex );

        plIntersection _getBoundaryIntersection( PLuint x, PLuint y );

        void _extractMenuRenderComponents( plRenderMap& renderMap ) const;

};

#endif
