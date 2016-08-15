#include "plModelEditor.h"

plModelEditor::plModelEditor()
    : _editMode(PL_MODEL_EDIT_MODE_TRANSLATE),
      _selectedModel(nullptr)
{
}


void plModelEditor::clearSelection()
{
    _selectedModel = nullptr;
    for (plModel* model : _plan->models())
    {
        _clearEditable(*model);
    }
}


void plModelEditor::setEditMode(uint32_t editMode)
{
    switch (editMode)
    {
        case PL_MODEL_EDIT_MODE_TRANSLATE:
        case PL_MODEL_EDIT_MODE_ROTATE:

            _editMode = editMode;
            break;

        default:

            std::cerr << "plModelEditor::setEditMode() error: invalid edit mode enumeration provided" << std::endl;
            break;
    }
}


bool plModelEditor::processMousePress(int32_t x, int32_t y)
{
    plPickingInfo pick = plPicking::pickPixel(x, y);

    switch (pick.r)
    {
        case PL_PICKING_TYPE_BONE:

            selectModel(pick.g);
            return true;

        default:

            clearSelection();
            break;
    }

    return false;
}


bool plModelEditor::processMouseDrag(int32_t x, int32_t y)
{
    plPickingInfo pick = plPicking::previousPick();  // read pick from last click, not what is currently under mouse

    switch (pick.r)
    {
        case PL_PICKING_TYPE_BONE:

            _dragModel(x, y);
            return true;

    }
    return false;
}


bool plModelEditor::processMouseRelease(int32_t x, int32_t y)
{
    _isDraggingMenu = false;
    return true;
}


void plModelEditor::selectModel(uint32_t index)
{
    // clear any previous selections
    clearSelection();

    _selectEditable(_plan->models(index));
    _selectedModel = &_plan->models(index);
}


void plModelEditor::_dragModel(int32_t x, int32_t y)
{
    if (_selectedModel == nullptr)
        return;

    switch (_editMode)
    {
        case PL_MODEL_EDIT_MODE_TRANSLATE:

            // translation
            // TODO: impl this
            break;

        case PL_MODEL_EDIT_MODE_ROTATE:

            // rotation
            // TODO: impl this
            break;
    }
}


void plModelEditor::toggleSelectedVisibility()
{
    if (_selectedModel == nullptr)
        return;

    plPickingStack::loadRed(PL_PICKING_TYPE_BONE);
    plPickingStack::loadGreen(selectedModelID());
    plPickingStack::loadBlue(-1); // unused by models
    _selectedModel->toggleVisibility();
}


void plModelEditor::extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const
{
    _extractMenuRenderComponents(renderMap);

    if (_selectedModel == nullptr)
        return;

    // select model
    _selectedModel->extractRenderComponents(renderMap, technique);
}


void plModelEditor::extractRenderComponents(plRenderMap& renderMap) const
{
    extractRenderComponents(renderMap, PL_OUTLINE_TECHNIQUE);
}


void plModelEditor::_extractMenuRenderComponents(plRenderMap& renderMap) const
{
    const float32_t HORIZONTAL = PL_EDITOR_MENU_HORIZONTAL_BUFFER;
    const float32_t INITIAL_VERTICAL = PL_EDITOR_MENU_VERTICAL_BUFFER + _plan->models().size()*PL_EDITOR_MENU_VERTICAL_BUFFER;

    plMatrix44 ortho(0, plWindow::viewportWidth(), 0, plWindow::viewportHeight(), -1, 1);

    plMatrix44 camera(
        1, 0,  0, 0,
        0, 1,  0, 0,
        0, 0, -1, 0,
        0, 0,  0, 1);

    float32_t count = 0;
    plPickingStack::loadBlue(-1);

    plCameraStack::push(camera);
    plProjectionStack::push(ortho);
    plModelStack::push(plMatrix44()); // load identity
    {
        // model
        for (uint32_t i=0; i<_plan->models().size(); i++)
        {
            plPickingStack::loadRed(PL_PICKING_TYPE_BONE);
            plPickingStack::loadGreen(i);
            plPickingStack::loadBlue(-1);
            plColourStack::load(PL_MODEL_COLOUR);

            plRenderer::queueDisk(
                PL_MINIMAL_TECHNIQUE,
                plVector3(HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_BUFFER, 0),
                plVector3(0, 0, 1),
                PL_EDITOR_MENU_CIRCLE_RADIUS);

            if (_plan->models(i).isSelected())
            {
                // draw selection outline
                plRenderer::queueDisk(
                    PL_OUTLINE_TECHNIQUE,
                    plVector3(HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_BUFFER, 0),
                    plVector3(0, 0, 1),
                    PL_EDITOR_MENU_CIRCLE_RADIUS);
            }

            count++;
        }
    }
    plModelStack::pop();
    plCameraStack::pop();
    plProjectionStack::pop();
}


int32_t plModelEditor::selectedModelID() const
{
    for (uint32_t i=0; i< _plan->models().size(); i++)
    {
        if (_selectedModel == &_plan->models(i))
        {
            return i;
        }
    }
    return -1;
}
