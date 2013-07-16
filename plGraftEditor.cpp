#include "plGraftEditor.h"

plGraftEditor::plGraftEditor() 
{
    _editMode       = PL_GRAFT_EDIT_MODE_TRANSLATE;
    _editAxis       = plVector3(1,0,0);
    _selectedGraft  = NULL; 
    _selectedType   = -1;
    _handlesEnabled = true;
}


void plGraftEditor::clearSelection( plPlan &plan )
{
    _selectedGraft  = NULL; 
    _selectedType   = -1;
    for (PLuint i=0; i<plan._grafts.size(); i++)
    {
        plan._grafts[i]._clearSelection();      
    }  
}

void plGraftEditor::setEditMode( PLuint editMode )
{
    switch (editMode)
    {
        case PL_GRAFT_EDIT_MODE_TRANSLATE:
        case PL_GRAFT_EDIT_MODE_ROTATE: 
        case PL_GRAFT_EDIT_MODE_LENGTH:
        case PL_GRAFT_EDIT_MODE_MARKER:
        
            _editMode = editMode;
            break;
            
        default:
        
            std::cerr << "plGraftEditor setEditMode() error: invalid edit mode enumeration provided\n";      
    }
}


void plGraftEditor::draw() const
{ 
    if (!_handlesEnabled || _selectedGraft == NULL)
        return;

    plModelStack::push( _selectedGraft->transform(_selectedType).matrix() );
    plModelStack::translate( 0, PL_GRAFT_HANDLE_OFFSET, 0 );

    if (PL_GRAFT_SELECTED_IS_DEFECT)
        plModelStack::translate( 0, _selectedGraft->heightOffset(), 0 );

    switch(_editMode)
    {
        case PL_GRAFT_EDIT_MODE_TRANSLATE:   
        case PL_GRAFT_EDIT_MODE_ROTATE:
        {
            plColourStack::load( 0.2, 0.2, 0.2 ); 
            plDraw::sphere( 1.5 );

            // x axis
            plColourStack::load( 1.0, 0.2, 0.2 ); 
            
            plPicking::value.type = PL_PICKING_TYPE_GRAFT_HANDLE_X;
        
            (_editMode == PL_GRAFT_EDIT_MODE_TRANSLATE) ? plDraw::arrow( plVector3(1,0,0) ) : plDraw::knob( plVector3(1,0,0) );

            if (PL_GRAFT_EDIT_SHOW_Y_HANDLE)    
            {
                // y axis
                plColourStack::load( 0.2, 1.0, 0.2 );  
                plPicking::value.type = PL_PICKING_TYPE_GRAFT_HANDLE_Y;              

                (_editMode == PL_GRAFT_EDIT_MODE_TRANSLATE) ? plDraw::arrow( plVector3(0,1,0) ) : plDraw::knob( plVector3(0,1,0) );
            }

            // z axis
            plColourStack::load( 0.2, 0.2, 1.0 ); 
            plPicking::value.type = PL_PICKING_TYPE_GRAFT_HANDLE_Z;

            (_editMode == PL_GRAFT_EDIT_MODE_TRANSLATE) ? plDraw::arrow( plVector3(0,0,1) ) : plDraw::knob( plVector3(0,0,1) );
            
            break;
        }
        case PL_GRAFT_EDIT_MODE_LENGTH:
        {     
            plColourStack::load( 0.2, 0.2, 0.2 ); 
            plDraw::sphere( plVector3(0, PL_HANDLE_LENGTH/2.0, 0), 1.5 );
                    
            plPicking::value.type = PL_PICKING_TYPE_GRAFT_HANDLE_Y;

            plColourStack::load( 0.2, 1.0, 0.2 ); 
            plDraw::arrow( plVector3(0, PL_HANDLE_LENGTH-PL_ARROW_LENGTH-1, 0), plVector3(0,1,0), PL_HANDLE_LENGTH-PL_ARROW_LENGTH);    
            plColourStack::load( 1.0, 0.2, 0.2 );         
            plDraw::arrow( plVector3(0, PL_HANDLE_LENGTH-PL_ARROW_LENGTH, 0), plVector3(0,-1,0), PL_HANDLE_LENGTH-PL_ARROW_LENGTH);

            break;     
        }
        
        case PL_GRAFT_EDIT_MODE_MARKER:
        {
            plColourStack::load( 0.2, 0.2, 0.2 ); 
            plDraw::sphere( plVector3(0, PL_HANDLE_LENGTH/2.0, 0), 1.5 );

            plPicking::value.type = PL_PICKING_TYPE_GRAFT_HANDLE_Y;

            plColourStack::load( 0.2, 1.0, 0.2 ); 
            plDraw::knob( plVector3(0, PL_HANDLE_LENGTH-PL_ARROW_LENGTH-1, 0), plVector3(0,1,0),  PL_HANDLE_LENGTH-PL_ARROW_LENGTH);            
            plDraw::knob( plVector3(0, PL_HANDLE_LENGTH-PL_ARROW_LENGTH, 0),   plVector3(0,-1,0), PL_HANDLE_LENGTH-PL_ARROW_LENGTH);
           
            break;
        }
    }
    plModelStack::pop();
}


PLbool plGraftEditor::processMouseClick( plPlan &plan, PLint x, PLint y )
{
    plPickingInfo pick = plPicking::texture->readPixel(x,y);

    switch (pick.type) 
    {     
        case PL_PICKING_TYPE_GRAFT:
        
            selectGraft( plan, pick.id, pick.index);
            return true;

        case PL_PICKING_TYPE_GRAFT_HANDLE_X: 
        case PL_PICKING_TYPE_GRAFT_HANDLE_Y: 
        case PL_PICKING_TYPE_GRAFT_HANDLE_Z: 
        
            _selectHandle( plan, x, y, pick.type );
            return true;

        default:

            clearSelection( plan );
            break;
    }
    
    return false;

}


PLbool plGraftEditor::processMouseDrag( plPlan &plan, PLint x, PLint y )
{
    plPickingInfo pick = plPicking::texture->lastPick();  // read pick from last click, not what is currently under mouse

    switch (pick.type) 
    {  
        case PL_PICKING_TYPE_GRAFT_HANDLE_X: 
        case PL_PICKING_TYPE_GRAFT_HANDLE_Y: 
        case PL_PICKING_TYPE_GRAFT_HANDLE_Z: 

            _dragHandle( plan, x, y );
            return true;
    }
    return false;
}

PLbool plGraftEditor::processJoystickDrag( plPlan &plan, PLfloat x, PLfloat y, PLbool flag )
{
    if (_selectedGraft == NULL)    
        return false;                 // no graft selected

    const plTransform &transform = _selectedGraft->transform(_selectedType);    
    
    // translate joystick coords to local graft coords
    /*
    PLfloat localx = plVector3(x, y, 0) *  transform.x(); 
    PLfloat localy = plVector3(x, y, 0) * -transform.z(); 
    PLfloat localz = plVector3(x, y, 0) *  transform.y(); 
    */
    
    PLfloat localx =  transform.x() * plVector3(x, y, 0); 
    PLfloat localy = -transform.z() * plVector3(x, y, 0); 
    PLfloat localz =  transform.y() * plVector3(x, y, 0);
    
    plVector3 translationLocal( localx, localy, localz);
    
    if (translationLocal.squaredLength() > 1)
        translationLocal = translationLocal.normalize();
    
    // translate by local coords
    translateSelected( translationLocal );    
}

void plGraftEditor::selectGraft( plPlan &plan, PLuint index, PLuint type )
{   
    // clear any previous selections
    clearSelection( plan ); 

    for (PLuint i=0; i<plan._grafts.size(); i++)
    {
        if (i == index)
        {
            plan._grafts[i]._selectedValue  = type;
            plan._grafts[i]._isSelected     = true;
            _selectedType                   = type;
            _selectedGraft                  = &plan._grafts[i];
        }
    }   
    
}


void plGraftEditor::_selectHandle( plPlan &plan, PLint x, PLint y, PLuint type )
{   
    if (_selectedGraft == NULL)    
        return;                 // no graft selected

    _previousMousePos = plVector3(x,y,0);
                                                                 
    switch (type) 
    {
        case PL_PICKING_TYPE_GRAFT_HANDLE_X: 

            _editAxis               = _selectedGraft->transform(_selectedType).x();          
            _translationPlaneNormal = _selectedGraft->transform(_selectedType).z();            
            break;
            
        case PL_PICKING_TYPE_GRAFT_HANDLE_Y: 
        
            _editAxis               = _selectedGraft->transform(_selectedType).y();            
            break;
            
        case PL_PICKING_TYPE_GRAFT_HANDLE_Z: 
        
            _editAxis               =  _selectedGraft->transform(_selectedType).z();            
            _translationPlaneNormal = -_selectedGraft->transform(_selectedType).x(); 
            break;
    }                                                         
     
    plVector3 origin = _selectedGraft->transform(_selectedType).origin(); 
                                              
    _screenEditAxis = _getScreenAxis( _editAxis, origin );      
}


void plGraftEditor::_dragHandle( plPlan &plan, PLint x, PLint y )
{
    if (_selectedGraft == NULL)    
        return;                 // no graft selected

    plVector3 screenDragVector = plVector3(x,y,0) - _previousMousePos;
    
    switch (_editMode)
    {
        case PL_GRAFT_EDIT_MODE_TRANSLATE:
        {            
            // translation            
            const plTransform &transform = _selectedGraft->transform(_selectedType);           

            PLfloat distOnAxis = (screenDragVector * _screenEditAxis) * PL_DRAG_SENSITIVITY;

            if (_editAxis == transform.y())  
            {
                // translating along y
                _selectedGraft->adjustHeightOffset(distOnAxis);  
                break;              
            }
            
            distOnAxis = plMath::clamp(distOnAxis);   // prevent values outside of [-1 .. 1] to prevent translation 'jumps' due to discrete algorithm

            // use axis transforms with plane normal to preserve directional plane while draggin
            if (_editAxis == transform.x())  
            {
                _selectedGraft->translateX( _selectedType, distOnAxis, _translationPlaneNormal); 
                _editAxis = transform.x(); 
            } 
            else
            {
                _selectedGraft->translateZ( _selectedType, distOnAxis, _translationPlaneNormal); 
                _editAxis = transform.z(); 
            }     
                       
            break;
            
        }
        
        case PL_GRAFT_EDIT_MODE_ROTATE:
        {
            // rotation

            plVector3 screenAxisNormal(-_screenEditAxis.y, _screenEditAxis.x, 0 );
            PLfloat angle = -(screenDragVector * screenAxisNormal);
            _selectedGraft->rotate( _selectedType, _editAxis, angle);          
            break;
        }
        
        case PL_GRAFT_EDIT_MODE_LENGTH:
        {
            // length
            
            PLfloat distOnAxis = screenDragVector * _screenEditAxis; 
            _selectedGraft->adjustLength(-distOnAxis * PL_DRAG_SENSITIVITY) ;                            
            
            break;
        }
        
        case PL_GRAFT_EDIT_MODE_MARKER:
        {
            // marker
            
            plVector3 screenAxisNormal(-_screenEditAxis.y, _screenEditAxis.x, 0 );
            PLfloat angle = -(screenDragVector * screenAxisNormal);           
            _selectedGraft->spinMark( angle );
            break;
        }
    } 
    
    // update initial drag position
    _previousMousePos = plVector3(x,y,0);          
}


void plGraftEditor::translateSelected( const plVector3 &translation )
{
    if (_selectedGraft == NULL)    
        return;                 // no graft selected
    
    _selectedGraft->translate( _selectedType, translation );
}


void plGraftEditor::rotateSelected( const plVector3 &axis, PLfloat angle_degrees )
{
    if (_selectedGraft == NULL)    
        return;                 // no graft selected
    
    _selectedGraft->rotate( _selectedType, axis, angle_degrees);
}


void plGraftEditor::spinMarkSelected( PLfloat angle_degrees )
{
    if (_selectedGraft == NULL)    
        return;                 // no graft selected
    
    _selectedGraft->spinMark( angle_degrees );
}



plVector3 plGraftEditor::_getScreenAxis( const plVector3 &edit_axis, const plVector3 &world_position)
{
    plVector3 screenOrigin = plWindow::worldToScreen( world_position.x, 
                                                      world_position.y, 
                                                      world_position.z );
    
    plVector3 screenAxisTip = plWindow::worldToScreen( edit_axis.x + world_position.x, 
                                                       edit_axis.y + world_position.y, 
                                                       edit_axis.z + world_position.z );

    return (screenAxisTip - screenOrigin).normalize();
}









