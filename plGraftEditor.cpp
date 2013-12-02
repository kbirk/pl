#include "plGraftEditor.h"

plGraftEditor::plGraftEditor() 
    :   _editMode( PL_GRAFT_EDIT_MODE_TRANSLATE ),
        _selectedGraft( nullptr ),
        _selectedType( -1 )
{
}


void plGraftEditor::clearSelection()
{
    _selectedGraft = NULL; 
    _selectedType  = -1;
    for ( plGraft* graft : _plan->grafts() )
    {
        graft->_clearSelection();      
    }  
}


void plGraftEditor::setEditMode( PLuint editMode )
{
    switch ( editMode )
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


PLbool plGraftEditor::processMouseClick( PLint x, PLint y )
{
    plPickingInfo pick = plPicking::pickPixel( x, y );

    switch ( pick.r ) 
    {     
        case PL_PICKING_TYPE_GRAFT:
        case PL_PICKING_TYPE_GRAFT_MARKER:
        
            selectGraft( pick.g, pick.b );
            return true;
           
        case PL_PICKING_TYPE_GRAFT_HANDLE:  
           
            return true;
           
        default:

            clearSelection();
            break;
    }
    
    return false;
}


PLbool plGraftEditor::processMouseDrag(  PLint x, PLint y )
{
    plPickingInfo pick = plPicking::previousPick();  // read pick from last click, not what is currently under mouse

    switch ( pick.r ) 
    {  
        case PL_PICKING_TYPE_GRAFT_HANDLE:
 
            _dragHandle( x, y );
            return true;
        
        case PL_PICKING_TYPE_GRAFT_MARKER:         
        
            _dragMarker( x, y );
            return true;
             
        case PL_PICKING_TYPE_GRAFT: 
  
            _isDraggingMenu = true;  
            return true;  
                    
    }
    return false;
}


PLbool plGraftEditor::processMouseRelease(  PLint x, PLint y )
{
    _isDraggingMenu = false;
    return true;   
}


PLbool plGraftEditor::processJoystickDrag(  PLint x, PLint y )
{
    if (_selectedGraft == NULL)    
        return false;                 // no graft selected

    plVector3 translation( -y, 0.0f, x);
    
    if ( translation.squaredLength() > 1.0 )
    {
        translation = translation.normalize();
    }
    
    // get screen plane
    plVector3 localXAxis = ( plCameraStack::direction() ^ _selectedGraft->surfaceNormal( _selectedType ) ).normalize();
    plVector3 localZAxis = ( _selectedGraft->surfaceNormal( _selectedType ) ^ localXAxis ).normalize();
    plVector3 localYAxis = ( localXAxis ^ localZAxis ).normalize();

    translation = ( translation * localXAxis ) * localXAxis +
                  ( translation * localYAxis ) * localYAxis +
                  ( translation * localZAxis ) * localZAxis;

    // move graft
    //_selectedGraft->move( _selectedType, intersection.point, intersection.normal );

    return true;
}


void plGraftEditor::selectGraft(  PLuint index, PLuint type )
{   
    // clear any previous selections
    clearSelection(); 

    _plan->grafts( index )._selectedValue = type;
    _plan->grafts( index )._isSelected    = true;
    _selectedType  = type;
    _selectedGraft = &_plan->grafts( index );    
}


void plGraftEditor::_dragMarker( PLint x, PLint y )
{
    if ( _selectedGraft == NULL )    
        return;                 // no graft selected

    // get ray from mouse
    plVector3 rayOrigin, rayDirection;
    plWindow::cameraToMouseRay( rayOrigin, rayDirection, x, y );

    // graft origin and surface normal            
    plVector3 graftY      = _selectedGraft->transform( _selectedType ).y();
    plVector3 graftOrigin = _selectedGraft->transform( _selectedType ).origin();
    
    // intersect plane of graft
    plIntersection intersection = plMath::rayIntersect( rayOrigin, rayDirection, graftOrigin, graftY );

    if ( intersection.exists )
    {
        _selectedGraft->setMark( _selectedGraft->transform( _selectedType ).applyInverse( intersection.point ) );
    }


}


void plGraftEditor::_dragHandle( PLint x, PLint y )
{
    if ( _selectedGraft == NULL )    
        return;                 // no graft selected


    switch ( _editMode )
    {
        case PL_GRAFT_EDIT_MODE_TRANSLATE:
        {      
            // translation
                  
            // get ray from camera to mouse              
            plVector3 rayOrigin, rayDirection;
            plWindow::cameraToMouseRay( rayOrigin, rayDirection, x, y );
            
            // intersect bound mesh
            plIntersection intersection = _selectedGraft->plug( _selectedType ).mesh().rayIntersect( rayOrigin, rayDirection, true ); // smooth normal
            
            if ( intersection.exists )
            {
                // if intersection exists, move graft to new position
                _selectedGraft->move( _selectedType, intersection.point, intersection.normal );
            }
            else
            {
                // if no intersection, and is defect graft, move to closest point on spline
                if ( _selectedType == PL_PICKING_INDEX_GRAFT_DEFECT )
                {
                    intersection = plMath::getClosestPointToRay( _selectedGraft->plug( _selectedType ).mesh().triangles(), rayOrigin, rayDirection );
                    _selectedGraft->move( _selectedType, intersection.point, intersection.normal );
                }
            }            
            break;            
        }
        
        case PL_GRAFT_EDIT_MODE_ROTATE:
        {
            // rotation
            
            // get ray from mouse
            plVector3 rayOrigin, rayDirection;
            plWindow::cameraToMouseRay( rayOrigin, rayDirection, x, y );

            // graft origin and surface normal            
            plVector3 graftSurfaceNormal = _selectedGraft->surfaceNormal( _selectedType );
            plVector3 graftOrigin        = _selectedGraft->transform( _selectedType ).origin();
            
            // intersect plane of graft
            plIntersection intersection = plMath::rayIntersect( rayOrigin, rayDirection, graftOrigin, graftSurfaceNormal );
            
            if ( intersection.exists )
            {
                // get vector from graft origin to intersection, scale up by graft surface normal to scale rotation by distance
                plVector3 newGraftY = ( PL_ROTATION_SENSITIVITY * (intersection.point - graftOrigin) + graftSurfaceNormal ).normalize();

                _selectedGraft->rotate( _selectedType, newGraftY );    
            }
            break;
        }
        
        case PL_GRAFT_EDIT_MODE_LENGTH:
        {
            // length
            
            //PLfloat distOnAxis = screenDragVector * _screenEditAxis; 
            //_selectedGraft->adjustLength(-distOnAxis * PL_DRAG_SENSITIVITY) ;                            
            
            break;
        }
        
        case PL_GRAFT_EDIT_MODE_RADIUS:
        {
            // radius
            
            break;
        }
    }        
}


void plGraftEditor::toggleSelectedVisibility()
{
    if (_selectedGraft == NULL)
        return;
        
    _selectedGraft->toggleVisibility();
}


void plGraftEditor::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    _extractMenuRenderComponents( renderMap );

    if ( _selectedGraft == NULL || !_selectedGraft->isVisible() )    
        return;                 // no graft selected
      
    // select graft  
    _selectedGraft->extractRenderComponents( renderMap, technique );  
         
    // draw graft editor 
    plModelStack::push();
    plModelStack::load( _selectedGraft->transform(_selectedType).matrix() );

    plColourStack::load( PL_AXIS_GREY ); 
    plPickingStack::loadRed( PL_PICKING_TYPE_GRAFT_HANDLE );
    plRenderer::queue( plSphere( PL_PLAN_TECHNIQUE, plVector3( 0, 0, 0 ), PL_HANDLE_SPHERE_RADIUS ) );
   
    plModelStack::pop();

}


void plGraftEditor::extractRenderComponents( plRenderMap& renderMap ) const
{
    extractRenderComponents( renderMap, PL_OUTLINE_TECHNIQUE );
}


void plGraftEditor::_extractMenuRenderComponents( plRenderMap& renderMap ) const
{ 
    const PLfloat HARVEST_HORIZONTAL   = PL_EDITOR_MENU_HORIZONTAL_BUFFER;
    const PLfloat RECIPIENT_HORIZONTAL = (PL_EDITOR_MENU_HORIZONTAL_BUFFER + PL_EDITOR_MENU_CIRCLE_RADIUS + PL_EDITOR_MENU_HORIZONTAL_SPACING );      
    const PLfloat INITIAL_VERTICAL     = plWindow::viewportHeight() - PL_EDITOR_MENU_VERTICAL_BUFFER;

    plMatrix44 ortho( 0, plWindow::viewportWidth(), 0, plWindow::viewportHeight(), -1, 1 );

    plMatrix44 camera( 1, 0,  0, 0,
                       0, 1,  0, 0,
                       0, 0, -1, 0,
                       0, 0,  0, 1 ); 

    PLfloat count = 0;
    plPickingStack::loadBlue( -1 );

    plCameraStack::push( camera );
    plProjectionStack::push( ortho );       
    plModelStack::push( plMatrix44() ); // load identity
    {      
        // grafts       
        for (PLuint i=0; i<_plan->grafts().size(); i++)
        {
            plPickingStack::loadRed( PL_PICKING_TYPE_GRAFT );
            plPickingStack::loadGreen( i );  
             
            // harvest           
            plPickingStack::loadBlue( PL_PICKING_INDEX_GRAFT_DONOR );
            plColourStack::load( PL_GRAFT_DONOR_CARTILAGE_COLOUR ); 

            plRenderer::queue( plDisk( PL_MINIMAL_TECHNIQUE, plVector3( HARVEST_HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_BUFFER, 0), PL_EDITOR_MENU_CIRCLE_RADIUS ) );
             
            if ( _plan->grafts(i)._isSelected && _selectedType == PL_PICKING_INDEX_GRAFT_DONOR )
                plRenderer::queue( plDisk( PL_OUTLINE_TECHNIQUE, plVector3( HARVEST_HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_BUFFER, 0), PL_EDITOR_MENU_CIRCLE_RADIUS ) );
             
            // recipient
            plPickingStack::loadBlue( PL_PICKING_INDEX_GRAFT_DEFECT );           
            plColourStack::load( PL_GRAFT_DEFECT_CARTILAGE_COLOUR ); 

            plRenderer::queue( plDisk( PL_MINIMAL_TECHNIQUE, plVector3( RECIPIENT_HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_BUFFER, 0), PL_EDITOR_MENU_CIRCLE_RADIUS ) );

            if ( _plan->grafts(i)._isSelected && _selectedType == PL_PICKING_INDEX_GRAFT_DEFECT )
                plRenderer::queue( plDisk( PL_OUTLINE_TECHNIQUE, plVector3( RECIPIENT_HORIZONTAL, INITIAL_VERTICAL - count*PL_EDITOR_MENU_VERTICAL_BUFFER, 0), PL_EDITOR_MENU_CIRCLE_RADIUS ) );

            count++;
        }
        
        /*
        // dragged menu item
        if ( _isDraggingMenu )
        {   
            if ( _selectedType == PL_PICKING_INDEX_GRAFT_DONOR )
            {
                plColourStack::load( PL_GRAFT_DONOR_CARTILAGE_COLOUR_DULL ); 
            }
            else
            {     
                plColourStack::load( PL_GRAFT_DEFECT_CARTILAGE_COLOUR_DULL );
            }   
            plDraw::disk( plVector3( plWindow::windowToViewportX( x ), plWindow::windowToViewportY( y ), 0.0f), PL_EDITOR_MENU_CIRCLE_RADIUS );            
        }
        */
        

    }
    plModelStack::pop();
    plCameraStack::pop();
    plProjectionStack::pop();  
}
