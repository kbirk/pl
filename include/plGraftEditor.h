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
        void selectGraft   (PLuint index, PLuint type);

        void setEditMode(PLuint editMode);

        void extractRenderComponents(plRenderMap& renderMap) const;
        void extractRenderComponents(plRenderMap& renderMap, PLuint technique) const;

        PLbool isGraftSelected() const { return (_selectedGraft != nullptr); }

        PLbool processMouseClick   (PLint x, PLint y);
        PLbool processMouseDrag    (PLint x, PLint y);
        PLbool processMouseRelease (PLint x, PLint y);
        PLbool processJoystickDrag (PLint x, PLint y);

        void toggleSelectedVisibility();

    private:

        PLint    _editMode;
        PLint    _selectedType;
        plGraft *_selectedGraft;

        void _dragHandle(PLint x, PLint y);
        void _dragMarker(PLint x, PLint y);

        void _extractMenuRenderComponents(plRenderMap& renderMap) const;
};
