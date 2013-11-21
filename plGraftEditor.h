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

#define PL_DRAG_SENSITIVITY                      0.1f
#define PL_GRAFT_SELECTED_IS_DONOR               ( _selectedType == PL_PICKING_INDEX_GRAFT_DONOR )
#define PL_GRAFT_SELECTED_IS_DEFECT              ( _selectedType == PL_PICKING_INDEX_GRAFT_DEFECT )
#define PL_GRAFT_EDIT_SHOW_Y_HANDLE              (( _editMode      == PL_GRAFT_EDIT_MODE_ROTATE &&     \
                                                    _selectedType == PL_PICKING_INDEX_GRAFT_DEFECT) || \
                                                  ( _editMode     == PL_GRAFT_EDIT_MODE_TRANSLATE &&   \
                                                    _selectedType == PL_PICKING_INDEX_GRAFT_DEFECT))

class plGraftEditor : public plRenderable
{
    public:

        plGraftEditor();
               
        void    clearSelection( plPlan &plan );
        void    selectGraft   ( plPlan &plan, PLuint index, PLuint type );
        
        void    setEditMode( PLuint editMode );

        void    extractRenderComponents( plRenderMap& renderMap ) const;
        
        PLbool  isGraftSelected() const { return ( _selectedGraft != NULL ); }    

        PLbool  processMouseClick   ( plPlan &plan, PLint x, PLint y );
        PLbool  processMouseDrag    ( plPlan &plan, PLint x, PLint y );
        PLbool  processMouseRelease ( plPlan &plan, PLint x, PLint y );
        PLbool  processJoystickDrag ( plPlan &plan, PLint x, PLint y );
            
        //void    translateSelected ( const plVector3 &translation );
        //void    rotateSelected    ( const plVector3 &axis, PLfloat angle_degrees );
        //void    spinMarkSelected  ( PLfloat angle_degrees );
        
        void    toggleSelectedVisibility();

    private: 
    
        PLbool      _isDraggingMenu;
    
        PLint       _editMode;
        plVector3   _editAxis;
        plVector3   _screenEditAxis;
        
        PLbool      _handlesEnabled;

        PLint       _selectedType;
        plGraft    *_selectedGraft;
        
        plVector3   _previousMousePos;
        plVector3   _translationPlaneNormal;
        
        void      _selectMarker( plPlan &plan, PLuint x, PLuint y, PLuint index, PLuint type );
        void      _selectHandle ( plPlan &plan, PLint x, PLint y, PLuint type );
        void      _dragHandle   ( plPlan &plan, PLint x, PLint y );
        void      _dragMarker   ( plPlan &plan, PLint x, PLint y );
        plVector3 _getScreenAxis( const plVector3 &edit_axis, const plVector3 &world_position); 
                        
};

#endif

