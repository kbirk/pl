#ifndef __PL_STATE_H__
#define __PL_STATE_H__

#include "pl.h"
#include "plVector3.h"
#include "plTransform.h"

class plState
{   
    public:
    
        PLint       modelSelectedID;
    
        PLint       graftEditMode;
        plVector3   graftEditAxis;
        PLbool      graftHandlesEnabled;

        PLint       graftSelectedID;
        PLint       graftSelectedIndex;
        
        plVector3   graftInitialDragPos;
        plVector3   graftScreenAxis;
        plTransform graftInitialTransform;
        plVector3   graftInitialPlaneNormal;

        PLint       boundarySelectedType;
        PLint       boundarySelectedID;        
        PLint       boundarySelectedPointID;       
        
        PLfloat     splineTangentFactor;
        PLint       splineSelectedID;
        
        plState();

        void selectNothing();
        void selectGraft        ( PLuint id, PLuint index);
        void selectModel        ( PLuint id);
        void selectBoundary     ( PLuint type, PLuint id);    
        void selectBoundaryPoint( PLuint index);    
        void selectBoundaryPoint( PLuint type, PLuint id, PLuint index);   
        void selectSpline       ( PLuint id);   
};


#endif 
