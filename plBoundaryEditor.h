#ifndef __PL_BOUNDARY_EDITOR_H__
#define __PL_BOUNDARY_EDITOR_H__

#include "pl.h"
//#include "plError.h"
#include "plSeq.h"
#include "plVector3.h"
//#include "plTransform.h"
#include "plPlan.h"
#include "plBoundary.h"
#include "plDefectSite.h"
#include "plDonorSite.h"

class plBoundaryEditor
{
    public:

        plBoundaryEditor();
        
        void        clearSelection();           
        void        selectBoundary( PLuint boundaryType, PLuint boundaryIndex, PLuint pointIndex); 
        plBoundary& getSelectedBoundary();
        
        PLbool  processMouseClick( PLint x, PLint y );
        PLbool  processMouseDrag( PLint x, PLint y );

        //void moveSelectedBoundary (); // shift entire boundary around!
        
        PLint addPoint           ( PLuint x, PLuint y, PLbool selectNewPoint = true );
        //void  addBoundary        ( PLuint x, PLuint y, PLuint type, PLbool selectNewBoundary = true );
        
        void  moveSelectedPoint  ( PLuint x, PLuint y );
        void  removeSelectedPoint();

        void  draw() const;

    private: 
    
        PLint _selectedBoundaryType;
        PLint _selectedBoundaryIndex;
        PLint _selectedPointIndex;
         
        void _clearDefectSplineBoundaries();
        void _clearDonorSiteBoundaries();
        void _clearIGuideBoundaries();

        void _checkAndSelectBoundary     ( plBoundary &boundary, PLuint i, PLuint boundaryType, PLuint boundaryIndex, PLuint pointIndex);
        void _selectDefectSplineCorner   ( PLuint boundaryIndex, PLuint pointIndex );
        void _selectDefectSplineBoundary ( PLuint boundaryIndex, PLuint pointIndex );
        void _selectDonorSiteBoundary    ( PLuint boundaryIndex, PLuint pointIndex );
        void _selectIGuideBoundary       ( PLuint boundaryIndex, PLuint pointIndex );
       
};

#endif

