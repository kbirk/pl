#include "plBoundaryEditor.h"

plBoundaryEditor::plBoundaryEditor() 
{
    _selectedBoundaryType   = -1;
    _selectedSiteIndex      = -1;
    _selectedBoundary       = NULL;
    _selectedPointIndex     = -1;
    _isDraggingMenu         = false;
}


void plBoundaryEditor::clearSelection( plPlan &plan )
{
    _selectedBoundaryType   = -1;
    _selectedSiteIndex      = -1;
    _selectedBoundary       = NULL;
    _selectedPointIndex     = -1;
    _clearDefectSiteBoundaries( plan );
    _clearDonorSiteBoundaries ( plan );
    _clearIGuideBoundaries    ( plan );
}


PLbool plBoundaryEditor::processMouseClick( plPlan &plan, PLint x, PLint y )
{
    plPickingInfo pixel = plPicking::pickPixel( x, y );

    switch ( pixel.r ) 
    {  
        case PL_PICKING_TYPE_DEFECT_CORNERS:
        case PL_PICKING_TYPE_DEFECT_BOUNDARY:
        case PL_PICKING_TYPE_DONOR_BOUNDARY:
        case PL_PICKING_TYPE_IGUIDE_BOUNDARY:
        
            selectBoundary( plan, pixel.r, pixel.g, pixel.b );
            return true;

        default:

            clearSelection( plan );
            break;
    }
    
    return false;
}


PLbool plBoundaryEditor::processMouseDrag ( plPlan &plan, PLint x, PLint y )
{
    plPickingInfo pixel = plPicking::previousPick(); // read pick from last click, not what is currently under mouse

    switch ( pixel.r ) 
    {  
        case PL_PICKING_TYPE_IGUIDE_BOUNDARY:   
        case PL_PICKING_TYPE_DEFECT_CORNERS:
        
            _isDraggingMenu = ( _selectedPointIndex < 0 ); // drag menu if a point isn't selected
            // no break here, it should continue to process moveSelectedPoint()
                
        case PL_PICKING_TYPE_DEFECT_BOUNDARY:
        case PL_PICKING_TYPE_DONOR_BOUNDARY:
        
            moveSelectedPoint( plan, x, y );
            return true; 
    }
    return false;
}


PLbool plBoundaryEditor::processJoystickDrag ( plPlan &plan, PLint x, PLint y)
{
    if (_selectedBoundary == NULL)
        return false;
    
    return processMouseDrag( plan, x, y );
}


PLbool plBoundaryEditor::processMouseRelease( plPlan &plan, PLint x, PLint y )
{
    _isDraggingMenu = false;
    return true;   
}


void plBoundaryEditor::_clearDefectSiteBoundaries( plPlan &plan  )
{
    for (PLuint i=0; i<plan.defectSites().size(); i++)
    {
        plan.defectSites(i).spline._clearSelection();     
        plan.defectSites(i).boundary._clearSelection(); 
    }  
}


void plBoundaryEditor::_clearDonorSiteBoundaries( plPlan &plan )
{
    for (PLuint i=0; i<plan.donorSites().size(); i++)
    {
        plan.donorSites(i).boundary._clearSelection();       
    }  
}


void plBoundaryEditor::_clearIGuideBoundaries( plPlan &plan )
{
    for (PLuint i=0; i<plan.iGuideSites().size(); i++)
    {
        plan.iGuideSites(i).boundary._clearSelection();       
    }  
}


void plBoundaryEditor::_checkAndSelectBoundary( plBoundary &boundary, PLuint i, PLuint boundaryType, PLuint boundaryIndex, PLuint pointIndex)
{
    if ( i == boundaryIndex )
    {
        boundary._selectedValue  = pointIndex;        
        boundary._isSelected     = true;
        _selectedBoundaryType    = boundaryType;
        _selectedSiteIndex       = boundaryIndex;
        _selectedPointIndex      = pointIndex;
        _selectedBoundary        = &boundary;            
    }
}


void plBoundaryEditor::_selectDefectSiteSpline( plPlan &plan, PLuint boundaryIndex, PLuint pointIndex )
{
    for (PLuint i=0; i<plan.defectSites().size(); i++)
    {
        _checkAndSelectBoundary( plan.defectSites(i).spline, i, PL_PICKING_TYPE_DEFECT_CORNERS, boundaryIndex, pointIndex );   
    }  
}


void plBoundaryEditor::_selectDefectSiteBoundary( plPlan &plan, PLuint boundaryIndex, PLuint pointIndex )
{
    for (PLuint i=0; i<plan.defectSites().size(); i++)
    {
        _checkAndSelectBoundary( plan.defectSites(i).boundary, i, PL_PICKING_TYPE_DEFECT_BOUNDARY, boundaryIndex, pointIndex );     
    }  
}


void plBoundaryEditor::_selectDonorSiteBoundary( plPlan &plan, PLuint boundaryIndex, PLuint pointIndex )
{
    for (PLuint i=0; i<plan.donorSites().size(); i++)
    {
        _checkAndSelectBoundary( plan.donorSites(i).boundary, i, PL_PICKING_TYPE_DONOR_BOUNDARY, boundaryIndex, pointIndex );    
    }  
}


void plBoundaryEditor::_selectIGuideBoundary( plPlan &plan, PLuint boundaryIndex, PLuint pointIndex )
{
    for (PLuint i=0; i<plan.iGuideSites().size(); i++)
    {
        _checkAndSelectBoundary( plan.iGuideSites(i).boundary, i, PL_PICKING_TYPE_IGUIDE_BOUNDARY, boundaryIndex, pointIndex );    
    }  
}


void plBoundaryEditor::selectBoundary( plPlan &plan, PLuint boundaryType, PLuint boundaryIndex, PLuint pointIndex)
{   
    // clear any previous selections
    clearSelection( plan );
    // select
    switch (boundaryType) 
    {
        case PL_PICKING_TYPE_DEFECT_CORNERS:    _selectDefectSiteSpline  ( plan, boundaryIndex, pointIndex );    break;
        case PL_PICKING_TYPE_DEFECT_BOUNDARY:   _selectDefectSiteBoundary( plan, boundaryIndex, pointIndex );    break;       
        case PL_PICKING_TYPE_DONOR_BOUNDARY:    _selectDonorSiteBoundary ( plan, boundaryIndex, pointIndex );    break;
        case PL_PICKING_TYPE_IGUIDE_BOUNDARY:   _selectIGuideBoundary    ( plan, boundaryIndex, pointIndex );    break;  
    }

}


plIntersection plBoundaryEditor::_getBoundaryIntersection( plPlan &plan, PLuint x, PLuint y )
{
    plVector3 rayOrigin, rayDirection;
    plWindow::cameraToMouseRay( rayOrigin, rayDirection, x, y );
    
    return _selectedBoundary->mesh().rayIntersect( rayOrigin, rayDirection );
}


void plBoundaryEditor::moveSelectedPoint( plPlan &plan, PLuint x, PLuint y )
{
    if ( _selectedBoundary == NULL || _selectedPointIndex < 0 ) // no boundary or point is selected
        return;         

    plIntersection intersection = _getBoundaryIntersection( plan, x, y );

    if ( intersection.exists ) 
    {            
        _selectedBoundary->movePointAndNormal( _selectedPointIndex, intersection.point, intersection.normal);
    }

}


void plBoundaryEditor::addPoint( plPlan &plan, PLuint x, PLuint y, PLbool selectNewPoint )
{
    if ( _selectedBoundary == NULL ) // no boundary selected
        return;

    plIntersection intersection = _getBoundaryIntersection( plan, x, y );    
    
    if ( intersection.exists ) 
    {     
        PLint newIndex = _selectedBoundary->addPointAndNormal( intersection.point, intersection.normal );
        
        if ( selectNewPoint && newIndex >= 0 )
        {
            _selectedBoundary->_selectedValue = newIndex;
            _selectedPointIndex = newIndex;
        }       
    }

}


void plBoundaryEditor::removeSelectedPoint()
{
    if ( _selectedBoundary == NULL || _selectedPointIndex < 0 ) // no boundary or point is selected
        return;  

    _selectedBoundary->removePointAndNormal(_selectedPointIndex);   
    _selectedBoundary->_selectedValue = -1;
    _selectedPointIndex = -1;    
}


void plBoundaryEditor::toggleSelectedVisibility()
{
    if (_selectedBoundary == NULL)
        return;

    _selectedBoundary->toggleVisibility();
}


void plBoundaryEditor::clearSelectedBoundary( plPlan &plan )
{
    if (_selectedBoundary == NULL)
        return;
    
    _selectedBoundary->clear();
    
    // clearing corners, destroy defect site boundary
    if ( _selectedBoundaryType == PL_PICKING_TYPE_DEFECT_CORNERS )
    {
        plan.defectSites( _selectedSiteIndex ).boundary.clear();
    }
}


void plBoundaryEditor::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    if (_selectedBoundary == NULL)
        return;
        
    _selectedBoundary->extractRenderComponents( renderMap, technique );   
    
    if ( _selectedPointIndex > 0 )
    {
        plRenderer::queue( plSphere( PL_PLAN_TECHNIQUE, _selectedBoundary->points( _selectedPointIndex ), PL_SELECTED_BOUNDARY_POINT_RADIUS ) );         
        plRenderer::queue( plSphere( technique, _selectedBoundary->points( _selectedPointIndex ), PL_SELECTED_BOUNDARY_POINT_RADIUS ) );            
    }    
}


void plBoundaryEditor::extractRenderComponents( plRenderMap& renderMap ) const
{
     extractRenderComponents( renderMap, PL_OUTLINE_TECHNIQUE );
}



/*
void plBoundaryEditor::extractRenderComponents( std::set<plRenderComponent>& renderComponents ) const
{
    const PLfloat CORNER_HORIZONTAL   = plWindow::viewportWidth() - (PL_EDITOR_MENU_HORIZONTAL_BUFFER + PL_EDITOR_MENU_CIRCLE_RADIUS + PL_EDITOR_MENU_HORIZONTAL_SPACING);  
    const PLfloat BOUNDARY_HORIZONTAL = plWindow::viewportWidth() - PL_EDITOR_MENU_HORIZONTAL_BUFFER;
    const PLfloat INITIAL_VERTICAL    = plWindow::viewportHeight() - PL_EDITOR_MENU_VERTICAL_BUFFER;
     
    PLfloat count = 0;
    plPickingStack::loadBlue( -1 );   
       
    plModelStack::push( plMatrix44() ); // load identity
    {
        // defect sites       
        for ( PLuint i=0; i<plan.defectSites().size(); i++ )
        {
            plPickingStack::loadGreen( i );
            
            // spline menu
            plPickingStack::loadRed( PL_PICKING_TYPE_DEFECT_CORNERS );
                           
            if ( plan.defectSites(i).spline._isSelected )
            {
                plColourStack::load( PL_BOUNDARY_DEFECT_CORNER_COLOUR_DULL ); 
            }
            else
            {
                plColourStack::load( PL_BOUNDARY_DEFECT_CORNER_COLOUR ); 
            }
            plRenderer::queue( plDisk( plVector3( CORNER_HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_SPACING, 0), PL_EDITOR_MENU_CIRCLE_RADIUS ) );
            
            // boundary menu    
            plPickingStack::loadRed( PL_PICKING_TYPE_DEFECT_BOUNDARY );    
                    
            if ( plan.defectSites(i).boundary._isSelected )
            {
                plColourStack::load( PL_BOUNDARY_DEFECT_BOUNDARY_COLOUR_DULL ); 
            }
            else
            {
                plColourStack::load( PL_BOUNDARY_DEFECT_BOUNDARY_COLOUR ); 
            }
            plRenderer::queue( plDisk( plVector3( BOUNDARY_HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_SPACING, 0), PL_EDITOR_MENU_CIRCLE_RADIUS ) );

            count++;
        }

        // donor sites       
        for (PLuint i=0; i<plan.donorSites().size(); i++)
        {
            plPickingStack::loadGreen( i );
            
            // boundary menu
            plPickingStack::loadRed( PL_PICKING_TYPE_DONOR_BOUNDARY );
 
            if (plan.donorSites(i).boundary._isSelected)
            {
                plColourStack::load( PL_BOUNDARY_DONOR_COLOUR_DULL ); 
            }
            else
            {
                plColourStack::load( PL_BOUNDARY_DONOR_COLOUR ); 
            }
            plRenderer::queue( plDisk( plVector3( BOUNDARY_HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_SPACING, 0), PL_EDITOR_MENU_CIRCLE_RADIUS );
            
            count++;
        }

        // iGuide site boundaries       
        for (PLuint i=0; i<plan.iGuideSites().size(); i++)
        {
            plPickingStack::loadGreen( i );
            
            // boundary menu
            plPickingStack::loadRed( PL_PICKING_TYPE_IGUIDE_BOUNDARY );
     
            if (plan.iGuideSites(i).boundary._isSelected)
            {
                plColourStack::load( PL_BOUNDARY_IGUIDE_COLOUR_DULL ); 
            }
            else
            {
                plColourStack::load( PL_BOUNDARY_IGUIDE_COLOUR ); 
            }
            plRenderer::queue( plDisk( plVector3( BOUNDARY_HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_SPACING, 0), PL_EDITOR_MENU_CIRCLE_RADIUS );
            
            count++;
        }

        // dragged menu item
        if ( _isDraggingMenu )
        {   
            if ( _selectedBoundaryType == PL_PICKING_TYPE_IGUIDE_BOUNDARY )
            {
                plColourStack::load( PL_BOUNDARY_IGUIDE_COLOUR_DULL ); 
            } 
            else if ( _selectedBoundaryType == PL_PICKING_TYPE_DEFECT_CORNERS )
            {
                plColourStack::load( PL_BOUNDARY_DEFECT_CORNER_COLOUR_DULL ); 
            } 
            plRenderer::queue( plDisk( plVector3( plWindow::windowToViewportX( x ), plWindow::windowToViewportY( y ), 0.0f), PL_EDITOR_MENU_CIRCLE_RADIUS );            
        }
    }
    plModelStack::pop();
}
*/



