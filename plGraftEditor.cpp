#include "plGraftEditor.h"

plGraftEditor::plGraftEditor() 
{
    _editMode       = PL_GRAFT_EDIT_MODE_TRANSLATE;
    _editAxis       = plVector3(1,0,0);
    _selectedIndex  = -1; 
    _selectedType   = -1;
    _handlesEnabled = true;
}


void plGraftEditor::clearSelection()
{
    _selectedIndex  = -1; 
    _selectedType   = -1;
    for (PLuint i=0; i<_plPlan->_grafts.size(); i++)
    {
        _plPlan->_grafts[i]._clearSelection();      
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
    if (!_handlesEnabled || _selectedIndex < 0)
        return;

    glMatrixMode( GL_MODELVIEW );
    
    glPushMatrix();
    getSelectedTransform().apply();
    glTranslatef( 0, PL_GRAFT_HANDLE_OFFSET, 0 );

    if (PL_GRAFT_SELECTED_IS_DEFECT)
        glTranslatef( 0, getSelectedGraft().heightOffset(), 0 );

    switch(_editMode)
    {
        case PL_GRAFT_EDIT_MODE_TRANSLATE:   
        case PL_GRAFT_EDIT_MODE_ROTATE:
        {
            glColor3f( 0.2, 0.2, 0.2 ); 
            plDrawSphere( 1.5 );

            // x axis
            glColor3f( 1.0, 0.2, 0.2 ); 
            
            _plPickingState->type = PL_PICKING_TYPE_GRAFT_HANDLE_X;
            _plPickingShader->setPickingUniforms(_plPickingState);
        
            (_editMode == PL_GRAFT_EDIT_MODE_TRANSLATE) ? plDrawArrow( plVector3(1,0,0) ) : plDrawCircleArrow( plVector3(1,0,0) );

            if (PL_GRAFT_EDIT_SHOW_Y_HANDLE)    
            {
                // y axis
                glColor3f( 0.2, 1.0, 0.2 );  

                _plPickingState->type = PL_PICKING_TYPE_GRAFT_HANDLE_Y;
                _plPickingShader->setPickingUniforms(_plPickingState);

                (_editMode == PL_GRAFT_EDIT_MODE_TRANSLATE) ? plDrawArrow( plVector3(0,1,0) ) : plDrawCircleArrow( plVector3(0,1,0) );
            }

            // z axis
            glColor3f( 0.2, 0.2, 1.0 ); 
            _plPickingState->type = PL_PICKING_TYPE_GRAFT_HANDLE_Z;
            _plPickingShader->setPickingUniforms(_plPickingState);
        
            (_editMode == PL_GRAFT_EDIT_MODE_TRANSLATE) ? plDrawArrow( plVector3(0,0,1) ) : plDrawCircleArrow( plVector3(0,0,1) );
            
            break;
        }
        case PL_GRAFT_EDIT_MODE_LENGTH:
        {     
            glColor3f( 0.2, 0.2, 0.2 ); 
            glPushMatrix();
                glTranslatef( 0, PL_HANDLE_LENGTH/2.0, 0 );
                plDrawSphere( 1.5 );
            glPopMatrix();
                    
            _plPickingState->type = PL_PICKING_TYPE_GRAFT_HANDLE_Y;
            _plPickingShader->setPickingUniforms(_plPickingState);
            glColor3f( 0.2, 1.0, 0.2 ); 
            plDrawArrow( plVector3(0, PL_HANDLE_LENGTH-PL_ARROW_LENGTH-1, 0), plVector3(0,1,0), PL_HANDLE_LENGTH-PL_ARROW_LENGTH);    
            glColor3f( 1.0, 0.2, 0.2 );         
            plDrawArrow( plVector3(0, PL_HANDLE_LENGTH-PL_ARROW_LENGTH, 0), plVector3(0,-1,0), PL_HANDLE_LENGTH-PL_ARROW_LENGTH);

            break;     
        }
        
        case PL_GRAFT_EDIT_MODE_MARKER:
        {
            glColor3f( 0.2, 0.2, 0.2 ); 
            glPushMatrix();
                glTranslatef( 0, PL_HANDLE_LENGTH/2.0, 0 );
                plDrawSphere( 1.5 );
            glPopMatrix();

            _plPickingState->type = PL_PICKING_TYPE_GRAFT_HANDLE_Y;
            _plPickingShader->setPickingUniforms(_plPickingState);
            glColor3f( 0.2, 1.0, 0.2 ); 
            plDrawCircleArrow( plVector3(0, PL_HANDLE_LENGTH-PL_ARROW_LENGTH-1, 0), plVector3(0,1,0), PL_HANDLE_LENGTH-PL_ARROW_LENGTH);            
            plDrawCircleArrow( plVector3(0, PL_HANDLE_LENGTH-PL_ARROW_LENGTH, 0), plVector3(0,-1,0), PL_HANDLE_LENGTH-PL_ARROW_LENGTH);
           
            break;
        }
    }
    glPopMatrix();
}


PLbool plGraftEditor::processMouseClick( PLint x, PLint y )
{
    plPickingInfo pick = _plPickingTexture->readPixel(x,y);

    switch (pick.type) 
    {     
        case PL_PICKING_TYPE_GRAFT:
        
            selectGraft(pick.id, pick.index);
            return true;

        case PL_PICKING_TYPE_GRAFT_HANDLE_X: 
        case PL_PICKING_TYPE_GRAFT_HANDLE_Y: 
        case PL_PICKING_TYPE_GRAFT_HANDLE_Z: 
        
            _selectHandle( x, y, pick.type );
            return true;

        default:

            clearSelection();
            break;
    }
    
    return false;

}


PLbool plGraftEditor::processMouseDrag( PLint x, PLint y )
{
    plPickingInfo pick = _plPickingTexture->lastPick();  // read pick from last click, not what is currently under mouse

    switch (pick.type) 
    {  
        case PL_PICKING_TYPE_GRAFT_HANDLE_X: 
        case PL_PICKING_TYPE_GRAFT_HANDLE_Y: 
        case PL_PICKING_TYPE_GRAFT_HANDLE_Z: 

            _dragHandle( x, y );
            return true;
    }
    return false;
}


void plGraftEditor::selectGraft( PLuint index, PLuint type )
{   
    // clear any previous selections
    clearSelection(); 

    for (PLuint i=0; i<_plPlan->_grafts.size(); i++)
    {
        if (i == index)
        {
            _plPlan->_grafts[i]._selectedValue  = type;
            _plPlan->_grafts[i]._isSelected     = true;
            _selectedType                       = type;
            _selectedIndex                      = i;
        }
    }   
    
}


plGraft &plGraftEditor::getSelectedGraft() const
{
    return _plPlan->_grafts[_selectedIndex];
}


plTransform &plGraftEditor::getSelectedTransform() const
{
    return (PL_GRAFT_SELECTED_IS_DONOR) ? getSelectedGraft().harvestTransform : getSelectedGraft().recipientTransform; 
}


PLuint plGraftEditor::getSelectedModelID() const
{
    return (PL_GRAFT_SELECTED_IS_DONOR) ? getSelectedGraft().harvestModelID() : getSelectedGraft().recipientModelID(); 
}


void plGraftEditor::_selectHandle( PLint x, PLint y, PLuint type )
{   
    if (_selectedIndex < 0)    
        return;                 // no graft selected

    plGraft &graft = getSelectedGraft(); 
    
    _previousMousePos = plVector3(x,y,0);
                                                                 
    switch (type) 
    {
        case PL_PICKING_TYPE_GRAFT_HANDLE_X: 

            _editAxis               = getSelectedTransform().x();          
            _translationPlaneNormal = getSelectedTransform().z();            
            break;
            
        case PL_PICKING_TYPE_GRAFT_HANDLE_Y: 
        
            _editAxis               = getSelectedTransform().y();            
            break;
            
        case PL_PICKING_TYPE_GRAFT_HANDLE_Z: 
        
            _editAxis               =  getSelectedTransform().z();            
            _translationPlaneNormal = -getSelectedTransform().x(); 
            break;
    }                                                         
     
    plVector3 origin = getSelectedTransform().origin(); 
                                              
    _screenEditAxis = _getScreenAxis(_editAxis, origin);      
}


void plGraftEditor::_dragHandle( PLint x, PLint y )
{
    if (_selectedIndex < 0)    
        return;                 // no graft selected

    plGraft &graft = getSelectedGraft();
    
    plVector3 screenDragVector = plVector3(x,y,0) - _previousMousePos;
    
    switch (_editMode)
    {
        case PL_GRAFT_EDIT_MODE_TRANSLATE:
        {
            // translation
            
            PLuint       modelID   = getSelectedModelID();  
            plTransform &transform = getSelectedTransform();          
            
            PLfloat distOnAxis = (screenDragVector * _screenEditAxis) * PL_DRAG_SENSITIVITY;

            if (_editAxis == transform.y())  
            {
                // translating along y
                graft.adjustHeightOffset(distOnAxis);  
                break;              
            }
            
            distOnAxis = plClamp(distOnAxis);   // prevent values larger than -1/1 to prevent translation 'jumps' do to discrete approach

            plVector3 rayOrigin = transform.origin() + (distOnAxis * _editAxis);

            plIntersection intersection = _plPlan->_models[modelID].bone.rayIntersect( rayOrigin, -transform.y()); 

            if (intersection.exists)
            {   
                PLfloat normal_radius = 6.0f;
                plVector3 normal = _plPlan->_models[modelID].bone.getAverageNormal( normal_radius, transform.origin(), transform.y()); 
                
                plVector3 x,y,z;
                if (_editAxis == transform.x())  
                {
                    // translating along x                    
                    x = (normal ^ _translationPlaneNormal);                       
                    y = normal;                        
                    z = (x ^ y);
                    _editAxis = x;                       
                }
                else 
                {
                    // translating along z
                    z = (normal ^ _translationPlaneNormal);                       
                    y = normal;                         
                    x = (y ^ z);     
                    _editAxis = z; 
                } 
                transform.set( x, y, z, intersection.point );  
                
                if (PL_GRAFT_SELECTED_IS_DONOR)
                {
                    graft.setCaps( _plPlan->_models );
                }
            }
                    
            break;
        }
        
        case PL_GRAFT_EDIT_MODE_ROTATE:
        {
            // rotation

            plVector3 screenAxisNormal(-_screenEditAxis.y, _screenEditAxis.x, 0 );
            PLfloat angle = -(screenDragVector * screenAxisNormal);
            rotate( graft, _editAxis, angle);          
            break;
        }
        
        case PL_GRAFT_EDIT_MODE_LENGTH:
        {
            // length
            
            PLfloat distOnAxis = screenDragVector * _screenEditAxis; 
            graft.adjustLength(-distOnAxis * PL_DRAG_SENSITIVITY) ;                            
            
            break;
        }
        
        case PL_GRAFT_EDIT_MODE_MARKER:
        {
            // marker
            
            plVector3 screenAxisNormal(-_screenEditAxis.y, _screenEditAxis.x, 0 );
            PLfloat angle = -(screenDragVector * screenAxisNormal);           
            spinMarker( graft, angle );
            break;
        }
    } 
    
    // update initial drag position
    _previousMousePos = plVector3(x,y,0);          
}


void plGraftEditor::translateSelected( const plVector3 &translation )
{
    if (_selectedIndex < 0)    
        return;                 // no graft selected
    
    translate( getSelectedGraft(), translation);
}


void plGraftEditor::translate( plGraft &graft, const plVector3 &translation )
{             
    PLuint       modelID   = getSelectedModelID();    
    plTransform &transform = getSelectedTransform();    

    plIntersection intersection = _plPlan->_models[modelID].bone.rayIntersect( transform.origin() + translation, -transform.y() );  

    if (intersection.exists)
    {   
        PLfloat normal_radius = 6.0f;
        plVector3 normal = _plPlan->_models[modelID].bone.getAverageNormal( normal_radius, transform.origin(), transform.y() );  

        // translate       
        plVector3 y      = normal.normalize();
        plVector3 x      = (y ^ transform.z()).normalize();                       
        plVector3 origin = intersection.point;   
        transform.set( x, y, origin); 

        if (PL_GRAFT_SELECTED_IS_DONOR)
        {
            // harvest, re-compute cap  
            graft.setCaps( _plPlan->_models );
        }
    }
}


void plGraftEditor::rotateSelected( const plVector3 &axis, PLfloat angle_degrees )
{
    if (_selectedIndex < 0)    
        return;                 // no graft selected
    
    rotate( getSelectedGraft(), axis, angle_degrees);
}


void plGraftEditor::rotate( plGraft &graft, const plVector3 &axis, PLfloat angle_degrees )
{
    PLuint       modelID   = getSelectedModelID();    
    plTransform &transform = getSelectedTransform();  
  
    plMatrix44 rot; rot.setRotationD(angle_degrees,  axis);

    transform.set( rot * transform.x(), rot * transform.y() );   

    if (PL_GRAFT_SELECTED_IS_DONOR)
    {
        // harvest, re-compute cap      
        graft.setCaps( _plPlan->_models );    
    }
}


void plGraftEditor::spinMarkerSelected( PLfloat angle_degrees )
{
    if (_selectedIndex < 0)    
        return;                 // no graft selected
    
    spinMarker( getSelectedGraft(), angle_degrees);
}


void plGraftEditor::spinMarker( plGraft &graft, PLfloat angle_degrees )
{ 
    graft.spinMark(angle_degrees);
}


plVector3 plGraftEditor::_getScreenAxis( const plVector3 &edit_axis, const plVector3 &world_position)
{
    plVector3 screenOrigin = plWindowGetWorldToScreenPos( world_position.x, 
                                                          world_position.y, 
                                                          world_position.z );
    
    plVector3 screenAxisTip = plWindowGetWorldToScreenPos( edit_axis.x + world_position.x, 
                                                           edit_axis.y + world_position.y, 
                                                           edit_axis.z + world_position.z );

    return (screenAxisTip - screenOrigin).normalize();
}


////////////////////////////////////////////////////

void plSet( plGraftEditor &graftEditor )
{
    _plGraftEditor = &graftEditor;
}











