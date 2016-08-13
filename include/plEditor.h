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

        virtual bool processMouseClick   (int32_t x, int32_t y) = 0;
        virtual bool processMouseDrag    (int32_t x, int32_t y) = 0;
        virtual bool processMouseRelease (int32_t x, int32_t y) = 0;
        virtual bool processJoystickDrag (int32_t x, int32_t y) = 0;

        virtual void toggleSelectedVisibility() = 0;

        void extractRenderComponents(plRenderMap& renderMap) const = 0;
        void extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const = 0;

    protected:

        plPlan *_plan;
        bool  _isDraggingMenu;

        // used to prevent anything other than a pl*Editor from changing a plEditable selection state
        void _clearEditable(plEditable& editable) const;
        void _selectEditable(plEditable& editable, uint32_t value = -1) const;

};
