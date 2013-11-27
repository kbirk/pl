#include "plGraftEditor.h"

plGraftEditor::plGraftEditor() 
{
    _editMode       = PL_GRAFT_EDIT_MODE_TRANSLATE;
    _selectedGraft  = NULL; 
    _selectedType   = -1;
    _isDraggingMenu = false;
}


void plGraftEditor::clearSelection( plPlan &plan )
{
    _selectedGraft  = NULL; 
    _selectedType   = -1;
    for (PLuint i=0; i<plan.grafts().size(); i++)
    {
        plan.grafts(i)._clearSelection();      
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
        
            std::cerr << "plGraftEditor setEditMode() error: invalid edit mode enumeration provided" << std::endl;      
            break;
    }
}


PLbool plGraftEditor::processMouseClick( plPlan &plan, PLint x, PLint y )
{
    plPickingInfo pick = plPicking::pickPixel( x, y );

    switch ( pick.r ) 
    {     
        case PL_PICKING_TYPE_GRAFT:
        case PL_PICKING_TYPE_GRAFT_MARKER:
        
            selectGraft( plan, pick.g, pick.b );
            return true;
           
        case PL_PICKING_TYPE_GRAFT_HANDLE:  
           
            return true;
           
        default:

            clearSelection( plan );
            break;
    }
    
    return false;

}


PLbool plGraftEditor::processMouseDrag( plPlan &plan, PLint x, PLint y )
{
    plPickingInfo pick = plPicking::previousPick();  // read pick from last click, not what is currently under mouse

    switch ( pick.r ) 
    {  
        case PL_PICKING_TYPE_GRAFT_HANDLE:
 
            _dragHandle( plan, x, y );
            return true;
        
        case PL_PICKING_TYPE_GRAFT_MARKER:         
        
            _dragMarker( plan, x, y );
            return true;
             
        case PL_PICKING_TYPE_GRAFT: 
  
            _isDraggingMenu = true;  
            return true;  
                    
    }
    return false;
}


PLbool plGraftEditor::processMouseRelease( plPlan &plan, PLint x, PLint y )
{
    _isDraggingMenu = false;
    return true;   
}


PLbool plGraftEditor::processJoystickDrag( plPlan &plan, PLint x, PLint y )
{
    if (_selectedGraft == NULL)    
        return false;                 // no graft selected

    plVector3 translation( -y, 0.0f, x);
    
    if ( translation.squaredLength() > 1.0 )
    {
        translation = translation.normalize();
    }
    
    // get screen plane
    plVector3 localXAxis = ( plCameraStack::direction() ^ _selectedGraft->transform( _selectedType ).y() ).normalize();
    plVector3 localZAxis = ( _selectedGraft->transform( _selectedType ).y() ^ localXAxis ).normalize();
    plVector3 localYAxis = ( localXAxis ^ localZAxis ).normalize();

    translation = ( translation * localXAxis ) * localXAxis +
                  ( translation * localYAxis ) * localYAxis +
                  ( translation * localZAxis ) * localZAxis;

    return true;
}


void plGraftEditor::selectGraft( plPlan &plan, PLuint index, PLuint type )
{   
    // clear any previous selections
    clearSelection( plan ); 

    for (PLuint i=0; i<plan.grafts().size(); i++)
    {
        if (i == index)
        {
            plan.grafts(i)._selectedValue = type;
            plan.grafts(i)._isSelected    = true;
            _selectedType                 = type;
            _selectedGraft                = &plan.grafts(i);
        }
    }   
    
}


void plGraftEditor::_dragMarker( plPlan &plan, PLint x, PLint y )
{
    if ( _selectedGraft == NULL )    
        return;                 // no graft selected

    // get grafts origin and y    
    plVector3 graftOrigin = _selectedGraft->transform( _selectedType ).origin();
    plVector3 graftY      = _selectedGraft->transform( _selectedType ).y();

    // project origin into screen coords
    plVector3 originWindow = plWindow::worldToScreen( graftOrigin.x, graftOrigin.y, graftOrigin.z );

    // project mouse and origin back into world coords
    plVector3 mouseWorld  = plWindow::mouseToWorld( x, y, 0.0f );
    plVector3 originWorld = plWindow::mouseToWorld( originWindow.x, originWindow.y, 0.0f );
     
    plVector3 direction      = (mouseWorld - originWorld).normalize();   
    plVector3 tangent        = (direction ^ graftY).normalize();   
    plVector3 orthoDirection = (graftY ^ tangent).normalize() + graftOrigin;

    _selectedGraft->setMark( _selectedGraft->transform( _selectedType ).applyInverse( orthoDirection ) );
        
}


void plGraftEditor::_dragHandle( plPlan &plan, PLint x, PLint y )
{
    if ( _selectedGraft == NULL )    
        return;                 // no graft selected


    switch ( _editMode )
    {
        case PL_GRAFT_EDIT_MODE_TRANSLATE:
        {            
            // translation                
            plVector3 rayOrigin, rayDirection;
            plWindow::cameraToMouseRay( rayOrigin, rayDirection, x, y );
            
            plIntersection intersection( false );
            
            if ( _selectedType == PL_PICKING_INDEX_GRAFT_DEFECT )
            {
                // intersect spline
                for ( const plDefectSite* site : plan.defectSites() )
                {
                    intersection = site->spline.surfaceMesh().rayIntersect( rayOrigin, rayDirection );  
                    if ( intersection.exists )
                        break;    
                }
                
                // if not on spline, find closest point on spline
                if ( !intersection.exists )
                {
                    PLfloat lowestDist = FLT_MAX;
                    plVector3 lowestPoint, lowestNormal;
                    for ( const plDefectSite* site : plan.defectSites() )
                    {                   
                        for ( const plTriangle& triangle : site->spline.surfaceMesh().triangles() )
                        {
                            // find closest point on ray from triangle centre
                            plVector3 closestPointOnLine = plMath::closestPointOnLine( triangle.centroid(), rayOrigin, rayDirection );
                            
                            // find point on sphere closest to point
                            plVector3 closestPoint = triangle.centroid() + ( closestPointOnLine - triangle.centroid() ).normalize() * triangle.radius();
                            
                            PLfloat dist = ( closestPoint - closestPointOnLine ).squaredLength(); 
                            
                            if ( dist < lowestDist )
                            {
                                lowestDist = dist;
                                lowestPoint = closestPoint;
                                lowestNormal = triangle.normal();
                            }
                        }
                    }
                    intersection.exists = true;
                    intersection.point = lowestPoint;
                    intersection.normal = lowestNormal;
                }
            }
            else
            {
                // intersect model
                intersection = _selectedGraft->plug( _selectedType ).mesh().rayIntersect( rayOrigin, rayDirection, true ); // smooth normal
            }

            if ( intersection.exists )
            { 
                _selectedGraft->move( _selectedType, intersection.point, intersection.normal );
            }
            
            break;            
        }
        
        case PL_GRAFT_EDIT_MODE_ROTATE:
        {
            // rotation
            
            // get rotation from graft y to screen normal ( facing viewer )
            plVector3 viewDir = plCameraStack::direction();
            plVector3 graftY  = _selectedGraft->surfaceNormal( _selectedType );

            // transfrom from graft surface normal to screen normal ( facing viewer )
            plMatrix44 graftToScreen;
            graftToScreen.setRotation( graftY, -viewDir );

            // get graft origin in screen coords
            plVector3 graftOrigin = _selectedGraft->transform( _selectedType ).origin();
            plVector3 originWindow = plWindow::worldToScreen( graftOrigin.x, graftOrigin.y, graftOrigin.z );

            // get scaled direction vector between graft origin and mouse
            plVector3 mouseDirection = PL_ROTATION_SENSITIVITY * ( plVector3( -y, 0, x ) - plVector3( -originWindow.y, 0, originWindow.x ) ); //.normalize();

            // transform mouse direction to world direction by applying transform from graft normal to view direction
            // add graft surface normal to this vector to allow distance sensitive rotation ( more rotation if u move away from graft origin )
            plVector3 newGraftY = ( ( graftToScreen * mouseDirection ) + graftY ).normalize(); 

            // check angle between new and old y axis
            PLfloat angle = acos( graftY * newGraftY );

            // if past threshold, set to
            if ( angle > PL_DEG_TO_RAD( PL_MAX_GRAFT_ROTATION ) )
            {
                // find vector in plane of graftY and newGraftY that is orthogonal to graftY
                plVector3 tangent = newGraftY ^ graftY;               
                plVector3 ortho = ( graftY ^ tangent ).normalize();                
                // trig to find scaling of new vector on plane
                newGraftY = ( graftY*cos( PL_DEG_TO_RAD( PL_MAX_GRAFT_ROTATION ) ) + ortho*sin( PL_DEG_TO_RAD( PL_MAX_GRAFT_ROTATION ) ) ).normalize();
            } 

             _selectedGraft->move( _selectedType, graftOrigin, newGraftY );      
             
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


/*
void plGraftEditor::drawMenu( const plPlan &plan, PLuint x, PLuint y ) const
{ 
    // menu interface

    const PLfloat HORIZONTAL_BUFFER    = 50;
    const PLfloat VERTICAL_BUFFER      = 50;
    const PLfloat HORIZONTAL_SPACING   = 40;
    const PLfloat VERTICAL_SPACING     = 40;     
    const PLfloat CIRCLE_RADIUS        = 14;
    const PLfloat HARVEST_HORIZONTAL   = HORIZONTAL_BUFFER;
    const PLfloat RECIPIENT_HORIZONTAL = (HORIZONTAL_BUFFER + CIRCLE_RADIUS + HORIZONTAL_SPACING);      
    const PLfloat INITIAL_VERTICAL     = plWindow::viewportHeight() - VERTICAL_BUFFER;
     
    PLfloat count = 0;
    plPickingStack::loadBlue( -1 );
       
    plModelStack::push( plMatrix44() ); // load identity
    {      
        // grafts       
        for (PLuint i=0; i<plan.grafts().size(); i++)
        {
            plPickingStack::loadRed( PL_PICKING_TYPE_GRAFT );
            plPickingStack::loadGreen( i );  
             
            // harvest           
            plPickingStack::loadBlue( PL_PICKING_INDEX_GRAFT_DONOR );
             
            if (plan.grafts(i)._isSelected && _selectedType == PL_PICKING_INDEX_GRAFT_DONOR)
            {
                plColourStack::load( PL_GRAFT_DONOR_CARTILAGE_COLOUR_DULL ); 
            }
            else
            {
                plColourStack::load( PL_GRAFT_DONOR_CARTILAGE_COLOUR ); 
            } 
            plDraw::disk( plVector3( HARVEST_HORIZONTAL, INITIAL_VERTICAL - count*VERTICAL_SPACING, 0), CIRCLE_RADIUS );
             
            // recipient
            plPickingStack::loadBlue( PL_PICKING_INDEX_GRAFT_DEFECT );
            
            if (plan.grafts(i)._isSelected && _selectedType == PL_PICKING_INDEX_GRAFT_DEFECT)
            {
                plColourStack::load( PL_GRAFT_DEFECT_CARTILAGE_COLOUR_DULL ); 
            }
            else
            {
                plColourStack::load( PL_GRAFT_DEFECT_CARTILAGE_COLOUR ); 
            } 
            plDraw::disk( plVector3( RECIPIENT_HORIZONTAL, INITIAL_VERTICAL - count*VERTICAL_SPACING, 0), CIRCLE_RADIUS );
            
            count++;
        }
        
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
            plDraw::disk( plVector3( plWindow::windowToViewportX( x ), plWindow::windowToViewportY( y ), 0.0f), CIRCLE_RADIUS );            
        }
        

    }
    plModelStack::pop();
}
 
    
void plGraftEditor::drawHandles() const
{     
    if ( !_handlesEnabled || _selectedGraft == NULL || !_selectedGraft->isVisible() )
        return;

    plModelStack::push( _selectedGraft->transform(_selectedType).matrix() );
    plModelStack::translate( 0, PL_GRAFT_HANDLE_OFFSET, 0 );

    if (PL_GRAFT_SELECTED_IS_DEFECT)
        plModelStack::translate( 0, _selectedGraft->heightOffset(), 0 );


    plColourStack::load( PL_AXIS_GREY ); 
    plPickingStack::loadRed( PL_PICKING_TYPE_GRAFT_HANDLE );
    plDraw::sphere( PL_HANDLE_SPHERE_RADIUS );
    
    switch(_editMode)
    {
        case PL_GRAFT_EDIT_MODE_TRANSLATE:   
        case PL_GRAFT_EDIT_MODE_ROTATE:
        {
            //plColourStack::load( PL_AXIS_GREY ); 
            //plPickingStack::loadRed( PL_PICKING_TYPE_GRAFT_HANDLE );
            
            

            // x axis
            plColourStack::load( PL_X_AXIS_COLOUR ); 
            
            //plPickingStack::loadRed( PL_PICKING_TYPE_GRAFT_HANDLE_X );
        
            (_editMode == PL_GRAFT_EDIT_MODE_TRANSLATE) ? plDraw::arrow( plVector3(1,0,0) ) : plDraw::knob( plVector3(1,0,0) );

            if (PL_GRAFT_EDIT_SHOW_Y_HANDLE)    
            {
                // y axis
                plColourStack::load( PL_Y_AXIS_COLOUR );  
                //plPickingStack::loadRed( PL_PICKING_TYPE_GRAFT_HANDLE_Y );             

                (_editMode == PL_GRAFT_EDIT_MODE_TRANSLATE) ? plDraw::arrow( plVector3(0,1,0) ) : plDraw::knob( plVector3(0,1,0) );
            }

            // z axis
            plColourStack::load( PL_Z_AXIS_COLOUR ); 
            //plPickingStack::loadRed( PL_PICKING_TYPE_GRAFT_HANDLE_Z );

            (_editMode == PL_GRAFT_EDIT_MODE_TRANSLATE) ? plDraw::arrow( plVector3(0,0,1) ) : plDraw::knob( plVector3(0,0,1) );
            
            break;
        }
        case PL_GRAFT_EDIT_MODE_LENGTH:
        {     
            //plColourStack::load( PL_AXIS_GREY ); 
            //plDraw::sphere( plVector3(0, PL_HANDLE_LENGTH/2.0, 0), 1.5 );
                    
            //plPickingStack::loadRed( PL_PICKING_TYPE_GRAFT_HANDLE_Y );        

            plColourStack::load( PL_Y_AXIS_COLOUR ); 
            plDraw::arrow( plVector3(0, PL_HANDLE_LENGTH-PL_ARROW_LENGTH-1, 0), plVector3(0,1,0), PL_HANDLE_LENGTH-PL_ARROW_LENGTH);    
            plColourStack::load( PL_X_AXIS_COLOUR );         
            plDraw::arrow( plVector3(0, PL_HANDLE_LENGTH-PL_ARROW_LENGTH, 0), plVector3(0,-1,0), PL_HANDLE_LENGTH-PL_ARROW_LENGTH);

            break;     
        }

    }
    plModelStack::pop();
}
*/






