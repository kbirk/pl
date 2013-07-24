#ifndef __PL_BOUNDARY_EDITOR_H__
#define __PL_BOUNDARY_EDITOR_H__

#include "plCommon.h"
#include "plSeq.h"
#include "plVector3.h"
#include "plPlan.h"
#include "plBoundary.h"
#include "plDefectSite.h"
#include "plDonorSite.h"
#include "plMatrixStack.h"
#include "plDraw.h"
#include "plWindow.h"

class plBoundaryEditor
{
    public:

        plBoundaryEditor();
        
        void    clearSelection( plPlan &plan );                     
        void    selectBoundary( plPlan &plan, PLuint boundaryType, PLuint boundaryIndex, PLuint pointIndex ); 

        PLbool  isBoundarySelected() const { return (_selectedBoundary != NULL); }

        PLbool  processMouseClick( plPlan &plan, PLint x, PLint y );
        PLbool  processMouseDrag ( plPlan &plan, PLint x, PLint y );
        void    processJoystickDrag ( plPlan &plan, PLint x, PLint y);


        void    addPoint           ( plPlan &plan, PLuint x, PLuint y, PLbool selectNewPoint = true );
        void    moveSelectedPoint  ( plPlan &plan, PLuint x, PLuint y );
        void    removeSelectedPoint();
        void    clearSelectedBoundary();
        void    removeSelectedSite( plPlan &plan );
        void    toggleSelectedVisibility();

        void    drawMenu( const plPlan &plan ) const;

    private: 
    
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

