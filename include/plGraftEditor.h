#pragma once

#include "plCommon.h"

#include "plVector3.h"
#include "plTransform.h"
#include "plPlan.h"
#include "plWindow.h"
#include "plMath.h"
#include "plRenderer.h"
#include "plEditor.h"

#define PL_GRAFT_EDIT_ROTATION_SENSITIVITY       0.05f   // lower is less sensitive
#define PL_GRAFT_EDIT_MODE_TRANSLATE             1
#define PL_GRAFT_EDIT_MODE_ROTATE                2
#define PL_GRAFT_EDIT_MODE_LENGTH                3
#define PL_GRAFT_EDIT_MODE_RADIUS                4


class plGraftEditor : public plEditor
{
    public:

        plGraftEditor();

        void clearSelection();
        void selectGraft(uint32_t index, uint32_t type);

        void setEditMode(uint32_t editMode);

        void extractRenderComponents(plRenderMap& renderMap) const;
        void extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const;

        bool isGraftSelected() const { return (_selectedGraft != nullptr); }

        bool processMousePress(int32_t x, int32_t y);
        bool processMouseDrag(int32_t x, int32_t y);
        bool processMouseRelease(int32_t x, int32_t y);

        void toggleSelectedVisibility();

    private:

        int32_t _editMode;
        int32_t _selectedType;
        plGraft *_selectedGraft;

        void _dragHandle(int32_t x, int32_t y);
        void _dragMarker(int32_t x, int32_t y);

        void _extractMenuRenderComponents(plRenderMap& renderMap) const;
};
