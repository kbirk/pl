#ifndef PL_EDITOR_H
#define PL_EDITOR_H

#include "plCommon.h"
#include "plRenderable.h"
#include "plPlan.h"

class plEditor : public plRenderable
{
    public:

        plEditor();
        
        virtual void attach( plPlan& plan );

        virtual void clearSelection() = 0;           

        virtual PLbool processMouseClick   ( PLint x, PLint y ) = 0;
        virtual PLbool processMouseDrag    ( PLint x, PLint y ) = 0;
        virtual PLbool processMouseRelease ( PLint x, PLint y ) = 0;
        virtual PLbool processJoystickDrag ( PLint x, PLint y ) = 0;

        virtual void toggleSelectedVisibility() = 0;

        void extractRenderComponents( plRenderMap& renderMap ) const = 0; 
        void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const = 0;

    protected: 
    
        PLbool _isDraggingMenu;

        plPlan *_plan;

};

#endif

