#pragma once

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
        void selectModel(uint32_t index);

        void setEditMode(uint32_t editMode);

        void extractRenderComponents(plRenderMap& renderMap) const;
        void extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const;

        bool isModelSelected() const { return (_selectedModel != nullptr); }
        int32_t  selectedModelID() const;

        bool processMousePress(int32_t x, int32_t y);
        bool processMouseDrag(int32_t x, int32_t y);
        bool processMouseRelease(int32_t x, int32_t y);

        void toggleSelectedVisibility();

    private:

        int32_t _editMode;
        plModel* _selectedModel;

        void _dragModel(int32_t x, int32_t y);
        void _extractMenuRenderComponents(plRenderMap& renderMap) const;
};
