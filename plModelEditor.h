#ifndef PL_MODEL_EDITOR_H
#define PL_MODEL_EDITOR_H

#include "plCommon.h"

#include "plVector3.h"
#include "plTransform.h"
#include "plPlan.h"
#include "plWindow.h"
#include "plMath.h"
#include "plRenderer.h"
#include "plEditor.h"

#define PL_MODEL_EDIT_MODE_TRANSLATE             1
#define PL_MODEL_EDIT_MODE_ROTATE                2 

class plModelEditor : public plEditor
{
    public:

        plModelEditor();
               
        void clearSelection();
        void selectModel( PLuint index );
        
        void setEditMode( PLuint editMode );

        void extractRenderComponents( plRenderMap& renderMap ) const;
        void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const;
        
        PLbool isModelSelected() const { return ( _selectedModel != NULL ); }    
        PLint  selectedModelID() const;

        PLbool processMouseClick   ( PLint x, PLint y );
        PLbool processMouseDrag    ( PLint x, PLint y );
        PLbool processMouseRelease ( PLint x, PLint y );
        PLbool processJoystickDrag ( PLint x, PLint y );

        void toggleSelectedVisibility();

    private: 
    
        PLint    _editMode;
        plModel *_selectedModel;  
        
        void _dragModel( PLint x, PLint y );    
        void _extractMenuRenderComponents( plRenderMap& renderMap ) const;
};

#endif

