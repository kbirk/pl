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

        PLbool  isGBoundarySelected() const { return (_selectedBoundary != NULL); }

        PLbool  processMouseClick( plPlan &plan, PLint x, PLint y );
        PLbool  processMouseDrag ( plPlan &plan, PLint x, PLint y );

        PLint   addPoint           ( plPlan &plan, PLuint x, PLuint y, PLbool selectNewPoint = true );
        void    moveSelectedPoint  ( PLuint x, PLuint y );
        void    removeSelectedPoint();

        void    draw( const plPlan &plan ) const;

    private: 
    
        PLint       _selectedBoundaryType;  
        // Plint       _selectedBoundaryIndex;    use this and above to determine with pl*Site 
        plBoundary *_selectedBoundary;
        PLint       _selectedPointIndex;
         
        void _clearDefectSiteBoundaries  ( plSeq<plDefectSite> &defectSites );
        void _clearDonorSiteBoundaries   ( plSeq<plDonorSite>  &donorSites );
        void _clearIGuideBoundaries      ( plSeq<plIGuide>     &iGuides );

        void _checkAndSelectBoundary     ( plBoundary &boundary, PLuint i, PLuint boundaryType, PLuint boundaryIndex, PLuint pointIndex);
        
        void _selectDefectSiteSpline     ( plSeq<plDefectSite> &defectSites, PLuint boundaryIndex, PLuint pointIndex );
        void _selectDefectSiteBoundary   ( plSeq<plDefectSite> &defectSites, PLuint boundaryIndex, PLuint pointIndex );
        void _selectDonorSiteBoundary    ( plSeq<plDonorSite>  &donorSites,  PLuint boundaryIndex, PLuint pointIndex );
        void _selectIGuideBoundary       ( plSeq<plIGuide> &iGuides,         PLuint boundaryIndex, PLuint pointIndex );
       
};

#endif

