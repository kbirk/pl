#ifndef PL_GRAFT_EDITOR_H
#define PL_GRAFT_EDITOR_H

#include "plCommon.h"

#include "plVector3.h"
#include "plTransform.h"
#include "plPlan.h"
#include "plGraft.h"
#include "plDraw.h"
#include "plWindow.h"
#include "plMath.h"
#include "plRenderer.h"
#include "plCylinder.h"
#include "plCone.h"

#define PL_ROTATION_SENSITIVITY                  0.010f   // lower is less sensitive

#define PL_MAX_GRAFT_ROTATION                    25.00f


class plGraftEditor : public plRenderable
{
    public:

        plGraftEditor();
               
        void clearSelection( plPlan &plan );
        void selectGraft   ( plPlan &plan, PLuint index, PLuint type );
        
        void setEditMode( PLuint editMode );

        void extractRenderComponents( plRenderMap& renderMap ) const;
        void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const;
        
        PLbool isGraftSelected() const { return ( _selectedGraft != NULL ); }    

        PLbool processMouseClick   ( plPlan &plan, PLint x, PLint y );
        PLbool processMouseDrag    ( plPlan &plan, PLint x, PLint y );
        PLbool processMouseRelease ( plPlan &plan, PLint x, PLint y );
        PLbool processJoystickDrag ( plPlan &plan, PLint x, PLint y );

        void toggleSelectedVisibility();

    private: 
    
        PLbool      _isDraggingMenu;
    
        PLint       _editMode;
        PLint       _selectedType;
        plGraft    *_selectedGraft;
        
        void      _dragHandle   ( plPlan &plan, PLint x, PLint y );
        void      _dragMarker   ( plPlan &plan, PLint x, PLint y );

                        
};

#endif

