#include "plModelEditor.h"

plModelEditor::plModelEditor() 
    :   _editMode( PL_MODEL_EDIT_MODE_TRANSLATE ),
        _selectedModel( nullptr )
{
}


void plModelEditor::clearSelection()
{
    _selectedModel = NULL; 
    for ( plGraft* graft : _plan->grafts() )
    {
        graft->_clearSelection();      
    }  
}


void plModelEditor::setEditMode( PLuint editMode )
{
    switch ( editMode )
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


PLbool plModelEditor::processMouseClick( PLint x, PLint y )
{
    plPickingInfo pick = plPicking::pickPixel( x, y );

    switch ( pick.r ) 
    {     
        case PL_PICKING_TYPE_BONE:
        
            selectModel( pick.g );
            return true;
           
        default:

            clearSelection();
            break;
    }
    
    return false;
}


PLbool plModelEditor::processMouseDrag(  PLint x, PLint y )
{
    plPickingInfo pick = plPicking::previousPick();  // read pick from last click, not what is currently under mouse

    switch ( pick.r ) 
    {  
        case PL_PICKING_TYPE_BONE:
 
            _dragModel( x, y );
            return true;
                    
    }
    return false;
}


PLbool plModelEditor::processMouseRelease(  PLint x, PLint y )
{
    _isDraggingMenu = false;
    return true;   
}


PLbool plModelEditor::processJoystickDrag(  PLint x, PLint y )
{
    if (_selectedModel == NULL)    
        return false;                 // no graft selected

    processMouseDrag( x, y );
}


void plModelEditor::selectModel( PLuint index )
{   
    // clear any previous selections
    clearSelection(); 

    _plan->models( index )._selectedValue = -1;
    _plan->models( index )._isSelected    = true;
    _selectedModel = &_plan->models( index );    
}


void plModelEditor::_dragModel( PLint x, PLint y )
{
    if ( _selectedModel == NULL )    
        return;                 // no graft selected


    switch ( _editMode )
    {
        case PL_MODEL_EDIT_MODE_TRANSLATE:
        {      
            // translation
            /*      
            // get ray from camera to mouse              
            plVector3 rayOrigin, rayDirection;
            plWindow::cameraToMouseRay( rayOrigin, rayDirection, x, y );
            
            // intersect bound mesh
            plIntersection intersection = _selectedModel->plug( _selectedType ).mesh().rayIntersect( rayOrigin, rayDirection, true ); // smooth normal
            
            if ( intersection.exists )
            {
                // if intersection exists, move graft to new position
                _selectedModel->move( _selectedType, intersection.point, intersection.normal );
            }
            else
            {
                // if no intersection, and is defect graft, move to closest point on spline
                if ( _selectedType == PL_PICKING_INDEX_GRAFT_DEFECT )
                {
                    intersection = plMath::getClosestPointToRay( _selectedModel->plug( _selectedType ).mesh().triangles(), rayOrigin, rayDirection );
                    _selectedModel->move( _selectedType, intersection.point, intersection.normal );
                }
            }   
            */         
            break;            
        }
        
        case PL_MODEL_EDIT_MODE_ROTATE:
        {
            // rotation
            /*
            // get ray from mouse
            plVector3 rayOrigin, rayDirection;
            plWindow::cameraToMouseRay( rayOrigin, rayDirection, x, y );

            // graft origin and surface normal            
            plVector3 graftSurfaceNormal = _selectedModel->plug( _selectedType ).surfaceTransform().y();
            plVector3 graftOrigin        = _selectedModel->plug( _selectedType ).finalTransform().origin();
            
            // intersect plane of graft
            plIntersection intersection = plMath::rayIntersect( rayOrigin, rayDirection, graftOrigin, graftSurfaceNormal );
            
            if ( intersection.exists )
            {
                // get vector from graft origin to intersection, scale up by graft surface normal to scale rotation by distance
                plVector3 newGraftY = ( PL_ROTATION_SENSITIVITY * (intersection.point - graftOrigin) + graftSurfaceNormal ).normalize();

                _selectedModel->rotate( _selectedType, newGraftY );    
            }
            */        
            break;
        }

    }        
}


void plModelEditor::toggleSelectedVisibility()
{
    if (_selectedModel == NULL)
        return;
        
    plPickingStack::loadRed( PL_PICKING_TYPE_BONE );                 
    plPickingStack::loadBlue( -1 ); // unused by models       
    _selectedModel->toggleVisibility();
}


void plModelEditor::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    if ( _selectedModel == NULL || !_selectedModel->isVisible() )    
        return;                 // no graft selected
      
    // select model  
    _selectedModel->extractRenderComponents( renderMap, technique );  

}


void plModelEditor::extractRenderComponents( plRenderMap& renderMap ) const
{
    extractRenderComponents( renderMap, PL_OUTLINE_TECHNIQUE );
}


PLint plModelEditor::selectedModelID() const
{
    for ( PLuint i=0; i< _plan->models().size(); i++ )
    {
        if ( _selectedModel == &_plan->models(i) )
        {
            return i;
        }
    }
    return -1;
}

