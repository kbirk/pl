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
#include "plDisk.h"
#include "plEditor.h"

#define PL_ROTATION_SENSITIVITY                  0.10f   // lower is less sensitive



class plGraftEditor : public plEditor
{
    public:

        plGraftEditor();
               
        void clearSelection();
        void selectGraft   ( PLuint index, PLuint type );
        
        void setEditMode( PLuint editMode );

        void extractRenderComponents( plRenderMap& renderMap ) const;
        void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const;
        
        PLbool isGraftSelected() const { return ( _selectedGraft != NULL ); }    

        PLbool processMouseClick   ( PLint x, PLint y );
        PLbool processMouseDrag    ( PLint x, PLint y );
        PLbool processMouseRelease ( PLint x, PLint y );
        PLbool processJoystickDrag ( PLint x, PLint y );

        void toggleSelectedVisibility();

    private: 
    
        PLint    _editMode;
        PLint    _selectedType;
        plGraft *_selectedGraft;
        
        void _dragHandle( PLint x, PLint y );
        void _dragMarker( PLint x, PLint y );

        void _extractMenuRenderComponents( plRenderMap& renderMap ) const;                
};

#endif

