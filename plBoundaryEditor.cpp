#include "plBoundaryEditor.h"

plBoundaryEditor::plBoundaryEditor() 
{
    _selectedBoundaryType   = -1;
    _selectedBoundaryIndex  = -1;
    _selectedPointIndex     = -1;
}

void plBoundaryEditor::clearSelection()
{
    _selectedBoundaryType   = -1;
    _selectedBoundaryIndex  = -1;
    _selectedPointIndex     = -1;
    _clearDefectSplineBoundaries();
    _clearDonorSiteBoundaries();
    _clearIGuideBoundaries();
}

PLbool plBoundaryEditor::processMouseClick( PLint x, PLint y)
{
    plPickingInfo pick = _plPickingTexture->readPixel(x,y);

    switch (pick.type) 
    {  
        case PL_PICKING_TYPE_DEFECT_CORNERS:
        case PL_PICKING_TYPE_DEFECT_BOUNDARY:
        case PL_PICKING_TYPE_DONOR_BOUNDARY:
        case PL_PICKING_TYPE_IGUIDE_BOUNDARY:
        
            selectBoundary( pick.type, pick.id, pick.index );
            return true;

        default:

            clearSelection();
            break;
    }
    
    return false;
}

PLbool plBoundaryEditor::processMouseDrag ( PLint x, PLint y)
{
    plPickingInfo pick =  _plPickingTexture->lastPick(); // read pick from last click, not what is currently under mouse

    switch (pick.type) 
    {  
        case PL_PICKING_TYPE_DEFECT_CORNERS:
        case PL_PICKING_TYPE_DEFECT_BOUNDARY:
        case PL_PICKING_TYPE_DONOR_BOUNDARY:
        case PL_PICKING_TYPE_IGUIDE_BOUNDARY:
        
            moveSelectedPoint( x, y );
            return true; 
    }
    return false;

}

void plBoundaryEditor::_clearDefectSplineBoundaries()
{
    for (PLuint i=0; i<_plPlan->_defectSites.size(); i++)
    {
        _plPlan->_defectSites[i].corners._clearSelection();     
        _plPlan->_defectSites[i].boundary._clearSelection(); 
    }  
}


void plBoundaryEditor::_clearDonorSiteBoundaries()
{
    for (PLuint i=0; i<_plPlan->_donorSites.size(); i++)
    {
        _plPlan->_donorSites[i].boundary._clearSelection();       
    }  
}

void plBoundaryEditor::_clearIGuideBoundaries()
{
    for (PLuint i=0; i<_plPlan->_iGuides.size(); i++)
    {
        _plPlan->_iGuides[i].boundary._clearSelection();       
    }  
}

void plBoundaryEditor::_checkAndSelectBoundary( plBoundary &boundary, PLuint i, PLuint boundaryType, PLuint boundaryIndex, PLuint pointIndex)
{
    if (i == boundaryIndex)
    {
        boundary._selectedValue  = pointIndex;
        boundary._isSelected     = true;
        _selectedBoundaryType   = boundaryType;
        _selectedBoundaryIndex  = boundaryIndex;
        _selectedPointIndex     = pointIndex;    
    }
}

void plBoundaryEditor::_selectDefectSplineCorner( PLuint boundaryIndex, PLuint pointIndex )
{
    for (PLuint i=0; i<_plPlan->_defectSites.size(); i++)
    {
        _checkAndSelectBoundary( _plPlan->_defectSites[i].corners, i, PL_PICKING_TYPE_DEFECT_CORNERS, boundaryIndex, pointIndex );   
    }  
}

void plBoundaryEditor::_selectDefectSplineBoundary( PLuint boundaryIndex, PLuint pointIndex )
{
    for (PLuint i=0; i<_plPlan->_defectSites.size(); i++)
    {
        _checkAndSelectBoundary( _plPlan->_defectSites[i].boundary, i, PL_PICKING_TYPE_DEFECT_BOUNDARY, boundaryIndex, pointIndex );     
    }  
}

void plBoundaryEditor::_selectDonorSiteBoundary( PLuint boundaryIndex, PLuint pointIndex )
{
    for (PLuint i=0; i<_plPlan->_donorSites.size(); i++)
    {
        _checkAndSelectBoundary( _plPlan->_donorSites[i].boundary, i, PL_PICKING_TYPE_DONOR_BOUNDARY, boundaryIndex, pointIndex );    
    }  
}

void plBoundaryEditor::_selectIGuideBoundary( PLuint boundaryIndex, PLuint pointIndex )
{
    for (PLuint i=0; i<_plPlan->_iGuides.size(); i++)
    {
        _checkAndSelectBoundary( _plPlan->_iGuides[i].boundary, i, PL_PICKING_TYPE_IGUIDE_BOUNDARY, boundaryIndex, pointIndex );    
    }  
}

plBoundary &plBoundaryEditor::getSelectedBoundary()
{
    switch (_selectedBoundaryType)
    {
        case PL_PICKING_TYPE_DEFECT_CORNERS:    return _plPlan->_defectSites[_selectedBoundaryIndex].corners;
        case PL_PICKING_TYPE_DEFECT_BOUNDARY:   return _plPlan->_defectSites[_selectedBoundaryIndex].boundary; 
        case PL_PICKING_TYPE_DONOR_BOUNDARY:    return _plPlan->_donorSites[_selectedBoundaryIndex].boundary;
        case PL_PICKING_TYPE_IGUIDE_BOUNDARY:   return _plPlan->_iGuides[_selectedBoundaryIndex].boundary; 
    }

    return _plPlan->_defectSites[0].corners;
}


void plBoundaryEditor::selectBoundary( PLuint boundaryType, PLuint boundaryIndex, PLuint pointIndex)
{   
    // clear any previous selections
    clearSelection();
    // select
    switch (boundaryType) 
    {
        case PL_PICKING_TYPE_DEFECT_CORNERS:    _selectDefectSplineCorner   ( boundaryIndex, pointIndex );     break;
        case PL_PICKING_TYPE_DEFECT_BOUNDARY:   _selectDefectSplineBoundary ( boundaryIndex, pointIndex );     break;       
        case PL_PICKING_TYPE_DONOR_BOUNDARY:    _selectDonorSiteBoundary    ( boundaryIndex, pointIndex );     break;
        case PL_PICKING_TYPE_IGUIDE_BOUNDARY:   _selectIGuideBoundary       ( boundaryIndex, pointIndex );     break;  
    }


}


void plBoundaryEditor::moveSelectedPoint( PLuint x, PLuint y )
{
    if (_selectedBoundaryIndex < 0 || _selectedPointIndex < 0) // no boundary or point is selected
        return;         
        
    plBoundary &boundary = getSelectedBoundary();
        
    plVector3 mouseInWorld = plWindowGetMouseToWorldPos(x, y, 0);
    plVector3 rayOrigin    = _plCamera->position; 
    plVector3 rayDirection = (mouseInWorld - rayOrigin).normalize();

    plIntersection intersection = _plPlan->_models[0].cartilage.rayIntersect( rayOrigin, rayDirection);

    if (intersection.exists) 
    {     
        boundary.movePointAndNormal( boundary._selectedValue, intersection.point, intersection.normal);
    }

    if ( _selectedBoundaryType ==  PL_PICKING_TYPE_DEFECT_CORNERS)
    {
        // recompute hermite spline
        _plPlan->_defectSites[_selectedBoundaryIndex].computeSpline( _plPlan->_models );
    }
}


PLint plBoundaryEditor::addPoint( PLuint x, PLuint y, PLbool selectNewPoint )
{
    if (_selectedBoundaryIndex < 0) // no boundary selected
        return -1;
    
    plVector3 mouseInWorld = plWindowGetMouseToWorldPos(x, y,0);
    plVector3 rayOrigin    = _plCamera->position; //plCameraGetPosition();
    plVector3 rayDirection = (mouseInWorld - rayOrigin).normalize();

    plIntersection intersection = _plPlan->_models[0].cartilage.rayIntersect( rayOrigin, rayDirection); 

    if (intersection.exists) 
    {     
        plBoundary &boundary = getSelectedBoundary();

        if (_selectedBoundaryType == PL_PICKING_TYPE_DEFECT_CORNERS && boundary.size() > 3)
        {   
            // already 4 corner points    
            return -1;
        }
    
        PLint newIndex = boundary.addPointAndNormal( intersection.point, intersection.normal );
        
        if (_selectedBoundaryType ==  PL_PICKING_TYPE_DEFECT_CORNERS)
        {
            // just added 4th point, compute spline    
            _plPlan->_defectSites[_selectedBoundaryIndex].computeSpline( _plPlan->_models );
        }
        
        if (selectNewPoint)
        {
            boundary._selectedValue = newIndex;
            _selectedPointIndex = newIndex;
        }
        
        return newIndex;
    }

    return -1;  // no cartilage at point
}


void plBoundaryEditor::removeSelectedPoint()
{
    if (_selectedBoundaryIndex < 0 || _selectedPointIndex < 0) // no boundary or point is selected
        return;  

    plBoundary &boundary = getSelectedBoundary();
    
    boundary.removePointAndNormal(_selectedPointIndex);
    
    boundary._selectedValue = -1;
    _selectedPointIndex    = -1;    
}

/*
void plBoundaryEditor::addBoundary( PLuint x, PLuint y, PLuint type, PLbool selectNewBoundary)
{
    clearSelection();

    switch (type)
    {
        case PL_PICKING_TYPE_DEFECT_CORNERS:
        case PL_PICKING_TYPE_DEFECT_BOUNDARY:
        
            _plPlan->_defectSite.add( plDefectSite() );
        
        
        case PL_PICKING_TYPE_DONOR_BOUNDARY:
        
            _plPlan._donorSites.add( plDonorSite() );

        
        case PL_PICKING_TYPE_IGUIDE_BOUNDARY:
            
            _plPlan._iGuides.add( plIGuide() );
    }
}
*/

void plBoundaryEditor::draw() const
{

    PLfloat windowWidth  = glutGet(GLUT_WINDOW_WIDTH);
    PLfloat windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    
    const PLfloat HORIZONTAL_BUFFER   = 50;
    const PLfloat VERTICAL_BUFFER     = 50;
    const PLfloat HORIZONTAL_SPACING  = 20;
    const PLfloat VERTICAL_SPACING    = 20;     
    const PLfloat CIRCLE_RADIUS       = 8;
    const PLfloat CORNER_HORIZONTAL   = windowWidth - (HORIZONTAL_BUFFER + CIRCLE_RADIUS + HORIZONTAL_SPACING);  
    const PLfloat BOUNDARY_HORIZONTAL = windowWidth - HORIZONTAL_BUFFER;
    const PLfloat INITIAL_VERTICAL    = windowHeight - VERTICAL_BUFFER;
     
    PLfloat count = 0;
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    {
        glLoadIdentity();   // set orthographic
         
        // TEMPORARY! REMOVE DEPENDANCY ON GLUT
        plMatrix44 ortho( 0, windowWidth, 0, windowHeight, -1, 1);
        glMultMatrixf( (GLfloat*)(&ortho) ); 

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        {
                   
            for (PLuint i=0; i<_plPlan->_defectSites.size(); i++)
            {
                // corners menu
                _plPickingState->type = PL_PICKING_TYPE_EDIT_DEFECT_CORNERS;
                _plPickingState->id = i; 
                _plPickingShader->setPickingUniforms(_plPickingState);           
                glColor3f( PL_BOUNDARY_DEFECT_CORNER_COLOUR ); 
                
                //plDrawDisk ( plVector3( CORNER_HORIZONTAL, INITIAL_VERTICAL + count*VERTICAL_SPACING, 0), CIRCLE_RADIUS );
                
                // boundary menu
                _plPickingState->type = PL_PICKING_TYPE_EDIT_DEFECT_BOUNDARY;
                _plPickingShader->setPickingUniforms(_plPickingState);           
                glColor3f( PL_BOUNDARY_DEFECT_CORNER_COLOUR ); 
                
                //plDrawDisk ( plVector3( BOUNDARY_HORIZONTAL, INITIAL_VERTICAL + count*VERTICAL_SPACING, 0), CIRCLE_RADIUS );

                count++;
            }
        }
        glPopMatrix();
    }
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}


////////////////////////////////////////////////////

void plSet( plBoundaryEditor &boundaryEditor )
{
    _plBoundaryEditor = &boundaryEditor;
}
