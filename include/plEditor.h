#pragma once

#include "plCommon.h"
#include "plRenderable.h"
#include "plPlan.h"


#define PL_EDITOR_MENU_HORIZONTAL_BUFFER         50
#define PL_EDITOR_MENU_VERTICAL_BUFFER           50
#define PL_EDITOR_MENU_HORIZONTAL_SPACING        40
#define PL_EDITOR_MENU_VERTICAL_SPACING          40
#define PL_EDITOR_MENU_CIRCLE_RADIUS             14


class plEditor : public plRenderable
{
    public:

        plEditor();

        virtual void attach(plPlan& plan);

        virtual void clearSelection() = 0;

        virtual PLbool processMouseClick   (PLint x, PLint y) = 0;
        virtual PLbool processMouseDrag    (PLint x, PLint y) = 0;
        virtual PLbool processMouseRelease (PLint x, PLint y) = 0;
        virtual PLbool processJoystickDrag (PLint x, PLint y) = 0;

        virtual void toggleSelectedVisibility() = 0;

        void extractRenderComponents(plRenderMap& renderMap) const = 0;
        void extractRenderComponents(plRenderMap& renderMap, PLuint technique) const = 0;

    protected:

        plPlan *_plan;
        PLbool  _isDraggingMenu;

        // used to prevent anything other than a pl*Editor from changing a plEditable selection state
        void _clearEditable(plEditable& editable) const;
        void _selectEditable(plEditable& editable, PLuint value = -1) const;

};
