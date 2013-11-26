#ifndef PL_BOUNDARY_EDITOR_H
#define PL_BOUNDARY_EDITOR_H

#include "plCommon.h"

#include "plVector3.h"
#include "plPlan.h"
#include "plBoundary.h"
#include "plDefectSite.h"
#include "plDonorSite.h"
#include "plMatrixStack.h"
#include "plDraw.h"
#include "plWindow.h"
#include "plRenderer.h"
#include "plDisk.h"

class plBoundaryEditor : public plRenderable
{
    public:

        plBoundaryEditor();
        
        void    clearSelection( plPlan &plan );                     
        void    selectBoundary( plPlan &plan, PLuint boundaryType, PLuint boundaryIndex, PLuint pointIndex ); 

        PLbool  isBoundarySelected() const { return (_selectedBoundary != NULL); }

        PLbool  processMouseClick   ( plPlan &plan, PLint x, PLint y );
        PLbool  processMouseDrag    ( plPlan &plan, PLint x, PLint y );
        PLbool  processMouseRelease ( plPlan &plan, PLint x, PLint y );
        PLbool  processJoystickDrag ( plPlan &plan, PLint x, PLint y );

        void    addPoint                ( plPlan &plan, PLuint x, PLuint y, PLbool selectNewPoint = true );
        void    moveSelectedPoint       ( plPlan &plan, PLuint x, PLuint y );
        void    removeSelectedPoint     ();
        void    clearSelectedBoundary   ( plPlan &plan );
        void    removeSelectedSite      ( plPlan &plan );
        void    toggleSelectedVisibility();

        void extractRenderComponents( plRenderMap& renderMap ) const; 
        void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const;

    private: 
    
        PLbool      _isDraggingMenu;
    
        PLint       _selectedBoundaryType;  
        PLint       _selectedSiteIndex;    // use this and above to determine which pl*Site 
        plBoundary *_selectedBoundary;
        PLint       _selectedPointIndex;
         
        void _clearDefectSiteBoundaries  ( plPlan &plan );
        void _clearDonorSiteBoundaries   ( plPlan &plan );
        void _clearIGuideBoundaries      ( plPlan &plan );

        void _checkAndSelectBoundary     ( plBoundary &boundary, PLuint i, PLuint boundaryType, PLuint boundaryIndex, PLuint pointIndex);
        
        void _selectDefectSiteSpline     ( plPlan &plan, PLuint boundaryIndex, PLuint pointIndex );
        void _selectDefectSiteBoundary   ( plPlan &plan, PLuint boundaryIndex, PLuint pointIndex );
        void _selectDonorSiteBoundary    ( plPlan &plan, PLuint boundaryIndex, PLuint pointIndex );
        void _selectIGuideBoundary       ( plPlan &plan, PLuint boundaryIndex, PLuint pointIndex );
       
        plIntersection _getBoundaryIntersection( plPlan &plan, PLuint x, PLuint y );

};

#endif

