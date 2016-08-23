#include "plGraftEditor.h"

plGraftEditor::plGraftEditor()
    : _editMode(PL_GRAFT_EDIT_MODE_TRANSLATE),
      _selectedType(-1),
      _selectedGraft(nullptr)
{
}

void plGraftEditor::clearSelection()
{
    _selectedGraft = nullptr;
    _selectedType = -1;
    for (auto graft : _plan->grafts())
    {
        _clearEditable(graft);
    }
}


void plGraftEditor::setEditMode(uint32_t editMode)
{
    switch (editMode)
    {
        case PL_GRAFT_EDIT_MODE_TRANSLATE:
        case PL_GRAFT_EDIT_MODE_ROTATE:
        case PL_GRAFT_EDIT_MODE_LENGTH:

            _editMode = editMode;
            break;

        default:

            std::cerr << "plGraftEditor::setEditMode() error: invalid edit mode enumeration provided" << std::endl;
            break;
    }
}


bool plGraftEditor::processMousePress(int32_t x, int32_t y)
{
    plPickingInfo pick = plPicking::pickPixel(x, y);

    switch (pick.r)
    {
        case PL_PICKING_TYPE_GRAFT:
        case PL_PICKING_TYPE_GRAFT_MARKER:

            selectGraft(pick.g, pick.b);
            return true;

        case PL_PICKING_TYPE_GRAFT_HANDLE:

            return true;

        default:

            clearSelection();
            break;
    }

    return false;
}


bool plGraftEditor::processMouseDrag(int32_t x, int32_t y)
{
    plPickingInfo pick = plPicking::previousPick();  // read pick from last click, not what is currently under mouse

    switch (pick.r)
    {
        case PL_PICKING_TYPE_GRAFT_HANDLE:

            _dragHandle(x, y);
            return true;

        case PL_PICKING_TYPE_GRAFT_MARKER:

            _dragMarker(x, y);
            return true;

        case PL_PICKING_TYPE_GRAFT:

            _isDraggingMenu = true;
            return true;

    }
    return false;
}


bool plGraftEditor::processMouseRelease(int32_t x, int32_t y)
{
    _isDraggingMenu = false;
    return true;
}


void plGraftEditor::selectGraft(uint32_t index, uint32_t type)
{
    // clear any previous selections
    clearSelection();

    _selectEditable(_plan->grafts(index), type);
    _selectedType = type;
    _selectedGraft = _plan->grafts(index);
}


void plGraftEditor::_dragMarker(int32_t x, int32_t y)
{
    if (_selectedGraft == nullptr)
        return;

    // get ray from mouse
    plVector3 rayOrigin, rayDirection;
    plWindow::cameraToMouseRay(rayOrigin, rayDirection, x, y);

    // graft origin and surface normal
    plVector3 surfaceNormal = _selectedGraft->plug(_selectedType)->surfaceTransform().y();
    plVector3 graftOrigin = _selectedGraft->plug(_selectedType)->surfaceTransform().origin();

    // intersect plane of graft
    plIntersection intersection = plMath::rayIntersect(rayOrigin, rayDirection, graftOrigin, surfaceNormal);

    if (intersection.exists)
    {
        plVector3 newMarkDir = _selectedGraft->plug(_selectedType)->finalTransform().applyInverse(intersection.point);
        newMarkDir = plVector3(newMarkDir.x, 0.0f, newMarkDir.z).normalize();

        if (_selectedType == PL_PICKING_INDEX_GRAFT_DEFECT)
        {
            // recipient, rotate around up axis
            float32_t angle = _selectedGraft->markDirection().signedAngle(newMarkDir, plVector3(0, 1, 0));
            _selectedGraft->rotate(_selectedType, PL_RAD_TO_DEG(angle));
        }
        else
        {
            // harvest, move marker
            // find angle between wanted direction and actual direction to determine angular offset
            _selectedGraft->setMarkDirection(newMarkDir);
        }
    }
}


void plGraftEditor::_dragHandle(int32_t x, int32_t y)
{
    if (_selectedGraft == nullptr)
        return;

    switch (_editMode)
    {
        case PL_GRAFT_EDIT_MODE_TRANSLATE:
        {
            // translation

            // get ray from camera to mouse
            plVector3 rayOrigin, rayDirection;
            plWindow::cameraToMouseRay(rayOrigin, rayDirection, x, y);

            // intersect bound mesh
            plIntersection intersection = _selectedGraft->plug(_selectedType)->mesh()->rayIntersect(rayOrigin, rayDirection, true); // smooth normal

            if (intersection.exists)
            {
                // if intersection exists, move graft to new position
                _selectedGraft->move(_selectedType, intersection.point, intersection.normal);
            }
            else
            {
                // if no intersection, and is defect graft, move to closest point on spline
                if (_selectedType == PL_PICKING_INDEX_GRAFT_DEFECT)
                {
                    intersection = plMath::getClosestPointToRay(_selectedGraft->plug(_selectedType)->mesh()->triangles(), rayOrigin, rayDirection);
                    _selectedGraft->move(_selectedType, intersection.point, intersection.normal);
                }
            }
            break;
        }

        case PL_GRAFT_EDIT_MODE_ROTATE:
        {
            // rotation

            // get ray from mouse
            plVector3 rayOrigin, rayDirection;
            plWindow::cameraToMouseRay(rayOrigin, rayDirection, x, y);

            // graft origin and surface normal
            plVector3 graftSurfaceNormal = _selectedGraft->plug(_selectedType)->surfaceTransform().y();
            plVector3 graftOrigin = _selectedGraft->plug(_selectedType)->finalTransform().origin();

            // intersect plane of graft
            plIntersection intersection = plMath::rayIntersect(rayOrigin, rayDirection, graftOrigin, graftSurfaceNormal);

            if (intersection.exists)
            {
                // get vector from graft origin to intersection, scale up by graft surface normal to scale rotation by distance
                plVector3 newGraftY = (PL_GRAFT_EDIT_ROTATION_SENSITIVITY * (intersection.point - graftOrigin) + graftSurfaceNormal).normalize();

                _selectedGraft->rotate(_selectedType, newGraftY);
            }

            break;
        }

        case PL_GRAFT_EDIT_MODE_LENGTH:
        {
            // length
            // TODO: impl this
            break;
        }

        case PL_GRAFT_EDIT_MODE_RADIUS:
        {
            // radius
            // TODO: impl this
            break;
        }
    }
}


void plGraftEditor::toggleSelectedVisibility()
{
    if (_selectedGraft == nullptr)
        return;

    _selectedGraft->toggleVisibility();
}


void plGraftEditor::extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const
{
    _extractMenuRenderComponents(renderMap);

    if (_selectedGraft == nullptr)
        return;

    // graft outline
    _selectedGraft->extractRenderComponents(renderMap, technique);

    plColorStack::load(PL_AXIS_GREY);
    plPickingStack::loadRed(PL_PICKING_TYPE_GRAFT_HANDLE);

    plModelStack::push();
    plModelStack::translate(_selectedGraft->plug(_selectedType)->surfaceTransform().y());

    plModelStack::push();
    plModelStack::mult(_selectedGraft->plug(_selectedType)->finalTransform().matrix());

    plRenderer::queueSphere(PL_PLAN_TECHNIQUE, plVector3(0, 0, 0), PL_HANDLE_SPHERE_RADIUS);
    plModelStack::load(_selectedGraft->plug(_selectedType)->finalTransform().matrix());

    plModelStack::pop();

    if (_editMode == PL_GRAFT_EDIT_MODE_TRANSLATE)
    {
        // draw axis
        _selectedGraft->plug(_selectedType)->surfaceTransform().extractRenderComponents(renderMap, PL_PLAN_TECHNIQUE);
    }
    else
    {
        // draw axis
        _selectedGraft->plug(_selectedType)->finalTransform().extractRenderComponents(renderMap, PL_PLAN_TECHNIQUE);
    }

    plModelStack::pop();
}


void plGraftEditor::extractRenderComponents(plRenderMap& renderMap) const
{
    extractRenderComponents(renderMap, PL_OUTLINE_TECHNIQUE);
}


void plGraftEditor::_extractMenuRenderComponents(plRenderMap& renderMap) const
{
    const float32_t HARVEST_HORIZONTAL = PL_EDITOR_MENU_HORIZONTAL_BUFFER;
    const float32_t RECIPIENT_HORIZONTAL = (PL_EDITOR_MENU_HORIZONTAL_BUFFER + PL_EDITOR_MENU_CIRCLE_RADIUS + PL_EDITOR_MENU_HORIZONTAL_SPACING);
    const float32_t INITIAL_VERTICAL = plWindow::viewportHeight() - PL_EDITOR_MENU_VERTICAL_BUFFER;

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
        // grafts
        for (uint32_t i=0; i<_plan->grafts().size(); i++)
        {
            plPickingStack::loadRed(PL_PICKING_TYPE_GRAFT);
            plPickingStack::loadGreen(i);

            // harvest
            plPickingStack::loadBlue(PL_PICKING_INDEX_GRAFT_DONOR);
            plColorStack::load(PL_GRAFT_DONOR_CARTILAGE_COLOR);

            plRenderer::queueDisk(
                PL_MINIMAL_TECHNIQUE,
                plVector3(HARVEST_HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_BUFFER, 0),
                plVector3(0, 0, 1),
                PL_EDITOR_MENU_CIRCLE_RADIUS);

            if (_plan->grafts(i)->isSelected() && _selectedType == PL_PICKING_INDEX_GRAFT_DONOR)
            {
                // draw selection outline
                plRenderer::queueDisk(
                    PL_OUTLINE_TECHNIQUE,
                    plVector3(HARVEST_HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_BUFFER, 0),
                    plVector3(0, 0, 1),
                    PL_EDITOR_MENU_CIRCLE_RADIUS);
            }

            // recipient
            plPickingStack::loadBlue(PL_PICKING_INDEX_GRAFT_DEFECT);
            plColorStack::load(PL_GRAFT_DEFECT_CARTILAGE_COLOR);

            plRenderer::queueDisk(
                PL_MINIMAL_TECHNIQUE,
                plVector3(RECIPIENT_HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_BUFFER, 0),
                plVector3(0, 0, 1),
                PL_EDITOR_MENU_CIRCLE_RADIUS);

            if (_plan->grafts(i)->isSelected() && _selectedType == PL_PICKING_INDEX_GRAFT_DEFECT)
            {
                // draw selection outline
                plRenderer::queueDisk(
                    PL_OUTLINE_TECHNIQUE,
                    plVector3(RECIPIENT_HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_BUFFER, 0),
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
