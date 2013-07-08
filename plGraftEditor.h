#ifndef __PL_GRAFT_EDITOR_H__
#define __PL_GRAFT_EDITOR_H__

#include "pl.h"
#include "plSeq.h"
#include "plVector3.h"
#include "plTransform.h"
#include "plPlan.h"
#include "plGraft.h"

#define PL_DRAG_SENSITIVITY                      0.1f
#define PL_GRAFT_SELECTED_IS_DONOR               ( _selectedType == PL_PICKING_INDEX_GRAFT_DONOR )
#define PL_GRAFT_SELECTED_IS_DEFECT              ( _selectedType == PL_PICKING_INDEX_GRAFT_DEFECT )
#define PL_GRAFT_EDIT_SHOW_Y_HANDLE              (( _editMode      == PL_GRAFT_EDIT_MODE_ROTATE &&     \
                                                    _selectedType == PL_PICKING_INDEX_GRAFT_DEFECT) || \
                                                  ( _editMode     == PL_GRAFT_EDIT_MODE_TRANSLATE &&   \
                                                    _selectedType == PL_PICKING_INDEX_GRAFT_DEFECT))

class plGraftEditor
{
    public:

        plGraftEditor();
               
        void         clearSelection();
        plGraft     &getSelectedGraft()     const;
        plTransform &getSelectedTransform() const;
        PLuint       getSelectedModelID()   const;
        
        void    setEditMode( PLuint editMode );

        void    draw() const;

        PLbool  processMouseClick( PLint x, PLint y );
        PLbool  processMouseDrag( PLint x, PLint y );
       
        void    selectGraft( PLuint index, PLuint type );       
                
        void    translateSelected ( const plVector3 &translation );
        void    rotateSelected    ( const plVector3 &axis, PLfloat angle_degrees );
        void    spinMarkerSelected( PLfloat angle_degrees );

        //
        void    translate ( plGraft &graft, const plVector3 &translation );
        void    rotate    ( plGraft &graft, const plVector3 &axis, PLfloat angle_degrees );
        void    spinMarker( plGraft &graft, PLfloat angle_degrees );
        //
        
    private: 
    
        PLint       _editMode;
        plVector3   _editAxis;
        plVector3   _screenEditAxis;
        
        PLbool      _handlesEnabled;

        PLint       _selectedType;
        PLint       _selectedIndex;
        
        plVector3   _previousMousePos;
        plVector3   _translationPlaneNormal;
            
        
        void      _selectHandle( PLint x, PLint y, PLuint type );
        void      _dragHandle( PLint x, PLint y );
        plVector3 _getScreenAxis( const plVector3 &edit_axis, const plVector3 &world_position); 
                        
};

#endif

