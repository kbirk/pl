#include "plEditor.h"

plEditor::plEditor()
    : _plan(nullptr)
    , _isDraggingMenu(false)
{
}

void plEditor::attach(std::shared_ptr<plPlan> plan)
{
    if (_plan != nullptr) {
        LOG_WARN("Plan already attached to editor");
    }
    _plan = plan;
}

void plEditor::_clearEditable(std::shared_ptr<plEditable> editable) const
{
    editable->_clearSelection();
}

void plEditor::_selectEditable(std::shared_ptr<plEditable> editable, uint32_t value) const
{
    editable->_isSelected = true;
    editable->_selectedValue = value;
}
