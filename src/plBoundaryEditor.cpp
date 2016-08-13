#include "plBoundaryEditor.h"

plBoundaryEditor::plBoundaryEditor()
    : _selectedSiteIndex( -1 ),
      _selectedBoundary( NULL ),
      _selectedPointIndex( -1 )
{
}


void plBoundaryEditor::clearSelection()
{
    _selectedSiteIndex  = -1;
    _selectedBoundary   = NULL;
    _selectedPointIndex = -1;
    _clearSiteBoundaries();
}


PLbool plBoundaryEditor::processMouseClick( PLint x, PLint y )
{
    plPickingInfo pixel = plPicking::pickPixel( x, y );

    switch ( pixel.r )
    {
        case PL_PICKING_TYPE_DEFECT_CORNERS:
        case PL_PICKING_TYPE_DEFECT_BOUNDARY:
        case PL_PICKING_TYPE_DONOR_BOUNDARY:
        case PL_PICKING_TYPE_IGUIDE_BOUNDARY:

            selectBoundary( pixel.r, pixel.g, pixel.b );
            return true;

        default:

            clearSelection();
            break;
    }

    return false;
}


PLbool plBoundaryEditor::processMouseDrag ( PLint x, PLint y )
{
    plPickingInfo pixel = plPicking::previousPick(); // read pick from last click, not what is currently under mouse

    switch ( pixel.r )
    {
        case PL_PICKING_TYPE_IGUIDE_BOUNDARY:
        case PL_PICKING_TYPE_DEFECT_CORNERS:

            _isDraggingMenu = ( _selectedPointIndex < 0 ); // drag menu if a point isn't selected
            // no break here, let it continue to process moveSelectedPoint()

        case PL_PICKING_TYPE_DEFECT_BOUNDARY:
        case PL_PICKING_TYPE_DONOR_BOUNDARY:

            moveSelectedPoint( x, y );
            return true;
    }
    return false;
}


PLbool plBoundaryEditor::processJoystickDrag ( PLint x, PLint y)
{
    if ( _selectedBoundary == NULL )
        return false;

    return processMouseDrag( x, y );
}


PLbool plBoundaryEditor::processMouseRelease( PLint x, PLint y )
{
    if ( !_selectedBoundary ) // no boundary or point is selected
        return false;

    // moving spline corners, recast defect site boundary
    if ( _selectedBoundary->type() == PL_PICKING_TYPE_DEFECT_SPLINE )
    {
        _plan->defectSites( _selectedSiteIndex ).recastBoundary();
    }

    _isDraggingMenu = false;
    return true;
}


void plBoundaryEditor::_clearSiteBoundaries()
{
    // defect sites
    for ( plDefectSite* defectSite : _plan->defectSites() )
    {
        _clearEditable( defectSite->spline );
        _clearEditable( defectSite->boundary );
    }

    // donor sites
    for ( plDonorSite* donorSite : _plan->donorSites() )
    {
        _clearEditable( donorSite->boundary );
    }

    // iguide sites
    for ( plIGuideSite* iguideSite : _plan->iGuideSites() )
    {
        _clearEditable( iguideSite->boundary );
    }
}


void plBoundaryEditor::_selectBoundary( plBoundary &boundary, PLuint boundaryIndex, PLuint pointIndex)
{
    _selectEditable( boundary, pointIndex );
    _selectedSiteIndex  = boundaryIndex;
    _selectedPointIndex = pointIndex;
    _selectedBoundary   = &boundary;
}


void plBoundaryEditor::selectBoundary( PLuint boundaryType, PLuint boundaryIndex, PLuint pointIndex)
{
    // clear any previous selections
    clearSelection();
    // select
    switch (boundaryType)
    {
        case PL_PICKING_TYPE_DEFECT_CORNERS:   _selectBoundary( _plan->defectSites( boundaryIndex ).spline,   boundaryIndex, pointIndex );   break;
        case PL_PICKING_TYPE_DEFECT_BOUNDARY:  _selectBoundary( _plan->defectSites( boundaryIndex ).boundary, boundaryIndex, pointIndex );   break;
        case PL_PICKING_TYPE_DONOR_BOUNDARY:   _selectBoundary( _plan->donorSites( boundaryIndex ).boundary,  boundaryIndex, pointIndex );   break;
        case PL_PICKING_TYPE_IGUIDE_BOUNDARY:  _selectBoundary( _plan->iGuideSites( boundaryIndex ).boundary, boundaryIndex, pointIndex );   break;
    }

}


plIntersection plBoundaryEditor::_getBoundaryIntersection( PLuint x, PLuint y )
{
    plVector3 rayOrigin, rayDirection;
    plWindow::cameraToMouseRay( rayOrigin, rayDirection, x, y );

    plIntersection intersection = _selectedBoundary->mesh().rayIntersect( rayOrigin, rayDirection );

    if ( !intersection.exists )
    {
        // if no intersection, and is defect boundary point, find closest point on spline
        if ( _selectedBoundary->type() == PL_PICKING_TYPE_DEFECT_BOUNDARY )
        {
            intersection = plMath::getClosestPointToRay( _selectedBoundary->mesh().triangles(), rayOrigin, rayDirection );
        }
    }

    return intersection;
}


void plBoundaryEditor::moveSelectedPoint( PLuint x, PLuint y )
{
    if ( _selectedBoundary == NULL || _selectedPointIndex < 0 ) // no boundary or point is selected
        return;

    plIntersection intersection = _getBoundaryIntersection( x, y );

    if ( intersection.exists )
    {
        _selectedBoundary->movePointAndNormal( _selectedPointIndex, intersection.point, intersection.normal);
    }
}


void plBoundaryEditor::addPoint( PLuint x, PLuint y, PLbool selectNewPoint )
{
    if ( _selectedBoundary == NULL ) // no boundary selected
        return;

    plIntersection intersection = _getBoundaryIntersection( x, y );

    if ( intersection.exists )
    {
        PLint newIndex = _selectedBoundary->addPointAndNormal( intersection.point, intersection.normal );

        if ( selectNewPoint && newIndex >= 0 )
        {
            _selectEditable( *_selectedBoundary, newIndex );
            _selectedPointIndex = newIndex;
        }
    }

}


void plBoundaryEditor::removeSelectedPoint()
{
    if ( _selectedBoundary == NULL || _selectedPointIndex < 0 ) // no boundary or point is selected
        return;

    _selectedBoundary->removePointAndNormal( _selectedPointIndex );
    _selectEditable( *_selectedBoundary );
    _selectedPointIndex = -1;
}


void plBoundaryEditor::toggleSelectedVisibility()
{
    if (_selectedBoundary == NULL)
        return;

    _selectedBoundary->toggleVisibility();
}


void plBoundaryEditor::clearSelectedBoundary()
{
    if (_selectedBoundary == NULL)
        return;

    _selectedBoundary->clear();

    // clearing corners, destroy defect site boundary
    if ( _selectedBoundary->type() == PL_PICKING_TYPE_DEFECT_CORNERS )
    {
        _plan->defectSites( _selectedSiteIndex ).boundary.clear();
    }
}


void plBoundaryEditor::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    _extractMenuRenderComponents( renderMap );

    if ( _selectedBoundary == NULL )
        return;

    _selectedBoundary->extractRenderComponents( renderMap, technique );
}


void plBoundaryEditor::extractRenderComponents( plRenderMap& renderMap ) const
{
     extractRenderComponents( renderMap, PL_OUTLINE_TECHNIQUE );
}


void plBoundaryEditor::_extractMenuRenderComponents( plRenderMap& renderMap ) const
{
    const PLfloat CORNER_HORIZONTAL   = plWindow::viewportWidth() - (PL_EDITOR_MENU_HORIZONTAL_BUFFER + PL_EDITOR_MENU_CIRCLE_RADIUS + PL_EDITOR_MENU_HORIZONTAL_SPACING);
    const PLfloat BOUNDARY_HORIZONTAL = plWindow::viewportWidth() - PL_EDITOR_MENU_HORIZONTAL_BUFFER;
    const PLfloat INITIAL_VERTICAL    = plWindow::viewportHeight() - PL_EDITOR_MENU_VERTICAL_BUFFER;

    PLfloat count = 0;
    plPickingStack::loadBlue( -1 );

    plMatrix44 ortho( 0, plWindow::viewportWidth(), 0, plWindow::viewportHeight(), -1, 1 );

    plMatrix44 camera( 1, 0,  0, 0,
                       0, 1,  0, 0,
                       0, 0, -1, 0,
                       0, 0,  0, 1 );

    plCameraStack::push( camera );
    plProjectionStack::push( ortho );
    plModelStack::push( plMatrix44() ); // load identity
    {
        // defect sites
        for ( PLuint i=0; i<_plan->defectSites().size(); i++ )
        {
            plPickingStack::loadGreen( i );

            // spline menu
            plPickingStack::loadRed( PL_PICKING_TYPE_DEFECT_CORNERS );
            plColourStack::load( PL_BOUNDARY_DEFECT_CORNER_COLOUR );
            plRenderer::queueDisk( PL_MINIMAL_TECHNIQUE,
                                   plVector3( CORNER_HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_SPACING, 0 ),
                                   plVector3( 0, 0, 1 ),
                                   PL_EDITOR_MENU_CIRCLE_RADIUS );

            if ( _plan->defectSites(i).spline.isSelected() )
            {
                // draw selection outline
                plRenderer::queueDisk( PL_OUTLINE_TECHNIQUE,
                                       plVector3( CORNER_HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_SPACING, 0 ),
                                       plVector3( 0, 0, 1 ),
                                       PL_EDITOR_MENU_CIRCLE_RADIUS );
            }
            // boundary menu
            plPickingStack::loadRed( PL_PICKING_TYPE_DEFECT_BOUNDARY );
            plColourStack::load( PL_BOUNDARY_DEFECT_BOUNDARY_COLOUR );

            plRenderer::queueDisk( PL_MINIMAL_TECHNIQUE,
                                   plVector3( BOUNDARY_HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_SPACING, 0),
                                   plVector3( 0, 0, 1 ),
                                   PL_EDITOR_MENU_CIRCLE_RADIUS );

            if ( _plan->defectSites(i).boundary.isSelected() )
            {
                // draw selection outline
                plRenderer::queueDisk( PL_OUTLINE_TECHNIQUE,
                                       plVector3( BOUNDARY_HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_SPACING, 0),
                                       plVector3( 0, 0, 1 ),
                                       PL_EDITOR_MENU_CIRCLE_RADIUS );
            }
            count++;
        }

        // donor sites
        for (PLuint i=0; i<_plan->donorSites().size(); i++)
        {
            plPickingStack::loadGreen( i );

            // boundary menu
            plPickingStack::loadRed( PL_PICKING_TYPE_DONOR_BOUNDARY );
            plColourStack::load( PL_BOUNDARY_DONOR_COLOUR );

            plRenderer::queueDisk( PL_MINIMAL_TECHNIQUE,
                                   plVector3( BOUNDARY_HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_SPACING, 0),
                                   plVector3( 0, 0, 1 ),
                                   PL_EDITOR_MENU_CIRCLE_RADIUS );

            if ( _plan->donorSites(i).boundary.isSelected() )
            {
                // draw selection outline
                plRenderer::queueDisk( PL_OUTLINE_TECHNIQUE,
                                       plVector3( BOUNDARY_HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_SPACING, 0),
                                       plVector3( 0, 0, 1 ),
                                       PL_EDITOR_MENU_CIRCLE_RADIUS );
            }
            count++;
        }

        // iGuide site boundaries
        for (PLuint i=0; i<_plan->iGuideSites().size(); i++)
        {
            plPickingStack::loadGreen( i );

            // boundary menu
            plPickingStack::loadRed( PL_PICKING_TYPE_IGUIDE_BOUNDARY );
            plColourStack::load( PL_BOUNDARY_IGUIDE_COLOUR );

            plRenderer::queueDisk( PL_MINIMAL_TECHNIQUE,
                                   plVector3( BOUNDARY_HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_SPACING, 0),
                                   plVector3( 0, 0, 1 ),
                                   PL_EDITOR_MENU_CIRCLE_RADIUS );

            if ( _plan->iGuideSites(i).boundary.isSelected() )
            {
                // draw selection outline
                plRenderer::queueDisk( PL_OUTLINE_TECHNIQUE,
                                       plVector3( BOUNDARY_HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_SPACING, 0),
                                       plVector3( 0, 0, 1 ),
                                       PL_EDITOR_MENU_CIRCLE_RADIUS );
            }
            count++;
        }

        /*
        // dragged menu item
        if ( _isDraggingMenu )
        {
            plColourStack::load( PL_BOUNDARY_DEFECT_CORNER_COLOUR_DULL );

            if ( _selectedBoundary->type() == PL_PICKING_TYPE_IGUIDE_BOUNDARY )
                plRenderer::queue( plDisk( PL_MINIMAL_TECHNIQUE, plVector3( plWindow::windowToViewportX( x ), plWindow::windowToViewportY( y ), 0.0f), PL_EDITOR_MENU_CIRCLE_RADIUS );
        }
        */
    }
    plModelStack::pop();
    plCameraStack::pop();
    plProjectionStack::pop();
}


void plBoundaryEditor::removeSelectedSite()
{
    // does not have access to the site that contains the boundary, so search and find which one it is
    for ( PLuint i=0; i < _plan->defectSites().size(); i++ )
    {
        if ( _selectedBoundary == &_plan->defectSites(i).boundary )
        {
            _plan->removeDefectSite( i );
        }
        if ( _selectedBoundary == &_plan->defectSites(i).spline )
        {
            _plan->removeDefectSite( i );
        }
    }

    for ( PLuint i=0; i < _plan->donorSites().size(); i++ )
    {
        if ( _selectedBoundary == &_plan->donorSites(i).boundary )
        {
            _plan->removeDonorSite( i );
        }
    }

    for ( PLuint i=0; i < _plan->iGuideSites().size(); i++ )
    {
        if ( _selectedBoundary == &_plan->iGuideSites(i).boundary )
        {
            _plan->removeIGuideSite( i );
        }
    }
    clearSelection();
}
