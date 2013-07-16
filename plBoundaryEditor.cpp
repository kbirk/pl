#include "plBoundaryEditor.h"

plBoundaryEditor::plBoundaryEditor() 
{
    _selectedBoundaryType   = -1;
    //_selectedBoundaryIndex  = -1;
    _selectedBoundary       = NULL;
    _selectedPointIndex     = -1;
}

void plBoundaryEditor::clearSelection( plPlan &plan )
{
    _selectedBoundaryType   = -1;
    //_selectedBoundaryIndex  = -1;
    _selectedBoundary       = NULL;
    _selectedPointIndex     = -1;
    _clearDefectSiteBoundaries  ( plan._defectSites );
    _clearDonorSiteBoundaries   ( plan._donorSites );
    _clearIGuideBoundaries      ( plan._iGuides );
}

PLbool plBoundaryEditor::processMouseClick( plPlan &plan, PLint x, PLint y)
{
    plPickingInfo pick = plPicking::texture->readPixel(x,y);

    switch (pick.type) 
    {  
        case PL_PICKING_TYPE_DEFECT_CORNERS:
        case PL_PICKING_TYPE_DEFECT_BOUNDARY:
        case PL_PICKING_TYPE_DONOR_BOUNDARY:
        case PL_PICKING_TYPE_IGUIDE_BOUNDARY:
        
            selectBoundary( plan, pick.type, pick.id, pick.index );
            return true;

        default:

            clearSelection( plan );
            break;
    }
    
    return false;
}

PLbool plBoundaryEditor::processMouseDrag ( plPlan &plan, PLint x, PLint y)
{
    plPickingInfo pick =  plPicking::texture->lastPick(); // read pick from last click, not what is currently under mouse

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

void plBoundaryEditor::_clearDefectSiteBoundaries( plSeq<plDefectSite> &defectSites )
{
    for (PLuint i=0; i<defectSites.size(); i++)
    {
        defectSites[i].spline._clearSelection();     
        defectSites[i].boundary._clearSelection(); 
    }  
}


void plBoundaryEditor::_clearDonorSiteBoundaries( plSeq<plDonorSite> &donorSites )
{
    for (PLuint i=0; i<donorSites.size(); i++)
    {
        donorSites[i].boundary._clearSelection();       
    }  
}

void plBoundaryEditor::_clearIGuideBoundaries( plSeq<plIGuide> &iGuides )
{
    for (PLuint i=0; i<iGuides.size(); i++)
    {
        iGuides[i].boundary._clearSelection();       
    }  
}

void plBoundaryEditor::_checkAndSelectBoundary( plBoundary &boundary, PLuint i, PLuint boundaryType, PLuint boundaryIndex, PLuint pointIndex)
{
    if (i == boundaryIndex)
    {
        boundary._selectedValue  = pointIndex;
        boundary._isSelected     = true;
        _selectedBoundaryType    = boundaryType;
        _selectedBoundary        = &boundary;
        _selectedPointIndex      = pointIndex;    
    }
}

void plBoundaryEditor::_selectDefectSiteSpline( plSeq<plDefectSite> &defectSites, PLuint boundaryIndex, PLuint pointIndex )
{
    for (PLuint i=0; i<defectSites.size(); i++)
    {
        _checkAndSelectBoundary( defectSites[i].spline, i, PL_PICKING_TYPE_DEFECT_CORNERS, boundaryIndex, pointIndex );   
    }  
}

void plBoundaryEditor::_selectDefectSiteBoundary( plSeq<plDefectSite> &defectSites, PLuint boundaryIndex, PLuint pointIndex )
{
    for (PLuint i=0; i<defectSites.size(); i++)
    {
        _checkAndSelectBoundary( defectSites[i].boundary, i, PL_PICKING_TYPE_DEFECT_BOUNDARY, boundaryIndex, pointIndex );     
    }  
}

void plBoundaryEditor::_selectDonorSiteBoundary( plSeq<plDonorSite> &donorSites, PLuint boundaryIndex, PLuint pointIndex )
{
    for (PLuint i=0; i<donorSites.size(); i++)
    {
        _checkAndSelectBoundary( donorSites[i].boundary, i, PL_PICKING_TYPE_DONOR_BOUNDARY, boundaryIndex, pointIndex );    
    }  
}

void plBoundaryEditor::_selectIGuideBoundary( plSeq<plIGuide> &iGuides, PLuint boundaryIndex, PLuint pointIndex )
{
    for (PLuint i=0; i<iGuides.size(); i++)
    {
        _checkAndSelectBoundary( iGuides[i].boundary, i, PL_PICKING_TYPE_IGUIDE_BOUNDARY, boundaryIndex, pointIndex );    
    }  
}


void plBoundaryEditor::selectBoundary( plPlan &plan, PLuint boundaryType, PLuint boundaryIndex, PLuint pointIndex)
{   
    // clear any previous selections
    clearSelection( plan );
    // select
    switch (boundaryType) 
    {
        case PL_PICKING_TYPE_DEFECT_CORNERS:    _selectDefectSiteSpline     ( plan._defectSites, boundaryIndex, pointIndex );    break;
        case PL_PICKING_TYPE_DEFECT_BOUNDARY:   _selectDefectSiteBoundary   ( plan._defectSites, boundaryIndex, pointIndex );    break;       
        case PL_PICKING_TYPE_DONOR_BOUNDARY:    _selectDonorSiteBoundary    ( plan._donorSites, boundaryIndex, pointIndex );     break;
        case PL_PICKING_TYPE_IGUIDE_BOUNDARY:   _selectIGuideBoundary       ( plan._iGuides, boundaryIndex, pointIndex );        break;  
    }

}


void plBoundaryEditor::moveSelectedPoint( PLuint x, PLuint y )
{
    if (_selectedBoundary == NULL || _selectedPointIndex < 0) // no boundary or point is selected
        return;         

    plVector3 rayOrigin, rayDirection;
    plWindow::mouseToRay( rayOrigin, rayDirection, x, y );

    plIntersection intersection = _selectedBoundary->model().cartilage.rayIntersect( rayOrigin, rayDirection);

    if (intersection.exists) 
    {     
        
        _selectedBoundary->movePointAndNormal( _selectedPointIndex, intersection.point, intersection.normal);
    }

}


PLint plBoundaryEditor::addPoint( plPlan &plan, PLuint x, PLuint y, PLbool selectNewPoint )
{
    if (_selectedBoundary == NULL) // no boundary selected
        return -1;
    
    plVector3 rayOrigin, rayDirection;
    plWindow::mouseToRay( rayOrigin, rayDirection, x, y );
    
    plIntersection intersection = _selectedBoundary->model().cartilage.rayIntersect( rayOrigin, rayDirection); 

    if (intersection.exists) 
    {     
        if (_selectedBoundaryType == PL_PICKING_TYPE_DEFECT_CORNERS && _selectedBoundary->size() > 3)
        {   
            // already 4 corner points    
            return -1;
        }
    
        PLint newIndex = _selectedBoundary->addPointAndNormal( intersection.point, intersection.normal );
        
        if (selectNewPoint)
        {
            _selectedBoundary->_selectedValue = newIndex;
            _selectedPointIndex = newIndex;
        }
        
        std::cout << "origin: " << rayOrigin << "\n";
        std::cout << "ray: "    << rayDirection << "\n";
        std::cout << "result: " << intersection.point << "\n";
        
        return newIndex;
    }

    return -1;  // no cartilage at point
}


void plBoundaryEditor::removeSelectedPoint()
{
    if (_selectedBoundary == NULL || _selectedPointIndex < 0) // no boundary or point is selected
        return;  

    _selectedBoundary->removePointAndNormal(_selectedPointIndex);   
    _selectedBoundary->_selectedValue = -1;
    _selectedPointIndex    = -1;    
}


void plBoundaryEditor::draw( const plPlan &plan ) const
{

    PLfloat windowWidth  = glutGet(GLUT_WINDOW_WIDTH);
    PLfloat windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    
    const PLfloat HORIZONTAL_BUFFER   = 50;
    const PLfloat VERTICAL_BUFFER     = 50;
    const PLfloat HORIZONTAL_SPACING  = 40;
    const PLfloat VERTICAL_SPACING    = 40;     
    const PLfloat CIRCLE_RADIUS       = 14;
    const PLfloat CORNER_HORIZONTAL   = windowWidth - (HORIZONTAL_BUFFER + CIRCLE_RADIUS + HORIZONTAL_SPACING);  
    const PLfloat BOUNDARY_HORIZONTAL = windowWidth - HORIZONTAL_BUFFER;
    const PLfloat INITIAL_VERTICAL    = windowHeight - VERTICAL_BUFFER;
     
    PLfloat count = 0;
    plPicking::value.index = -1;    
       
    plModelStack::push( plMatrix44() ); // load identity
    {
        // defect sites       
        for (PLuint i=0; i<plan._defectSites.size(); i++)
        {
            // spline menu
            plPicking::value.type = PL_PICKING_TYPE_DEFECT_CORNERS;           
            plPicking::value.id = i;  
               
            if (plan._defectSites[i].spline._isSelected)
            {
                plColourStack::load( PL_BOUNDARY_DEFECT_CORNER_COLOUR_DULL ); 
            }
            else
            {
                plColourStack::load( PL_BOUNDARY_DEFECT_CORNER_COLOUR ); 
            }
            plDraw::disk( plVector3( CORNER_HORIZONTAL, INITIAL_VERTICAL - count*VERTICAL_SPACING, 0), CIRCLE_RADIUS );
            
            // boundary menu
            plPicking::value.type = PL_PICKING_TYPE_DEFECT_BOUNDARY;                 
            if (plan._defectSites[i].boundary._isSelected)
            {
                plColourStack::load( PL_BOUNDARY_DEFECT_BOUNDARY_COLOUR_DULL ); 
            }
            else
            {
                plColourStack::load( PL_BOUNDARY_DEFECT_BOUNDARY_COLOUR ); 
            }
            plDraw::disk( plVector3( BOUNDARY_HORIZONTAL, INITIAL_VERTICAL - count*VERTICAL_SPACING, 0), CIRCLE_RADIUS );

            count++;
        }

        // donor sites       
        for (PLuint i=0; i<plan._donorSites.size(); i++)
        {
            // boundary menu
            plPicking::value.type = PL_PICKING_TYPE_DONOR_BOUNDARY;           
            plPicking::value.id = i;   
            if (plan._donorSites[i].boundary._isSelected)
            {
                plColourStack::load( PL_BOUNDARY_DONOR_COLOUR_DULL ); 
            }
            else
            {
                plColourStack::load( PL_BOUNDARY_DONOR_COLOUR ); 
            }
            plDraw::disk( plVector3( BOUNDARY_HORIZONTAL, INITIAL_VERTICAL - count*VERTICAL_SPACING, 0), CIRCLE_RADIUS );
            
            count++;
        }

        // iGuides boundaries       
        for (PLuint i=0; i<plan._iGuides.size(); i++)
        {
            // boundary menu
            plPicking::value.type = PL_PICKING_TYPE_IGUIDE_BOUNDARY;           
            plPicking::value.id = i;         
            if (plan._iGuides[i].boundary._isSelected)
            {
                plColourStack::load( PL_BOUNDARY_IGUIDE_COLOUR_DULL ); 
            }
            else
            {
                plColourStack::load( PL_BOUNDARY_IGUIDE_COLOUR ); 
            }
            plDraw::disk( plVector3( BOUNDARY_HORIZONTAL, INITIAL_VERTICAL - count*VERTICAL_SPACING, 0), CIRCLE_RADIUS );
            
            count++;
        }


    }
    plModelStack::pop();

}


