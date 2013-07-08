#include "plDefectSite.h" 

plDefectSite::plDefectSite() 
{
}

void plDefectSite::computeSpline( const plSeq<plBoneAndCartilage> &models )
{
    if (corners.size() == 4)
    {
        spline.computeHermite( corners, models[_modelID].cartilage );
    }
}

void plDefectSite::readFromCSV(const plSeq<plString> &row)
{
    // Fill in the field            
    plString subfield = row[2];
    
    if (plStringCompareCaseInsensitive(subfield, "model") )
    {
        _modelID = atof( row[3].c_str() );
    }                   
    else if (plStringCompareCaseInsensitive(subfield, "corners") ) 
    {     
        corners.readFromCSV( row );  

        /*
        if (corners.size() == 4)
        {
            spline.computeHermite( corners, models[_modelID].cartilage );
        }
        */
    }
    else if (plStringCompareCaseInsensitive(subfield, "boundary") )   
    {            
        boundary.readFromCSV( row );  
    }  
    else
        std::cerr << "Error importing plan, 'spline': Unrecognized word '" << subfield << "' in third column." << std::endl;      
}


void plDefectSite::draw() const
{      
    if (!isVisible)
        return;
      
    // draw spline boundary 
    _plPickingState->type = PL_PICKING_TYPE_DEFECT_BOUNDARY;
    boundary.draw();   
     
    // draw spline corners
    _plPickingState->type = PL_PICKING_TYPE_DEFECT_CORNERS;
    corners.draw();
    
    // draw spline corner axes
    /*
    if (corners.isSelected && corners.isVisible)
    {
        _drawCornersSelectionInterface();
    }    
    */
    
    // draw spline surface
    if (corners.size() == 4)
    {
        spline.draw();          
    }
}

/*
void plSpline::_drawCornersSelectionInterface() const
{
    const plSeq<plVector3> &p = corners.points();
    plSeq<plVector3> n;
    n.add( plModelCartilageGetAvgNormal(0, 4.0f, p[0], corners.normals(0)) );
    n.add( plModelCartilageGetAvgNormal(0, 4.0f, p[1], corners.normals(1)) );
    n.add( plModelCartilageGetAvgNormal(0, 4.0f, p[2], corners.normals(2)) );
    n.add( plModelCartilageGetAvgNormal(0, 4.0f, p[3], corners.normals(3)) );
        
    plVector3 nm = corners.getAverageNormal();
    
    glColor3f( 1.0, 0.2, 0.2 );
    for (PLuint i = 0; i < _s.size(); i++)
    {
        glPushMatrix();
        glTranslatef( nm.x*6, nm.y*6, nm.z*6 );
        plDrawArrow(p[i], _s[i], 4.0f, 0.5f);
        glPopMatrix();
    }

    glColor3f( 0.2, 0.2, 1.0);
    for (PLuint i = 0; i < _t.size(); i++)
    {
        glPushMatrix();
        glTranslatef( nm.x*6, nm.y*6, nm.z*6 );
        plDrawArrow(p[i], _t[i], 4.0f, 0.5f);
        glPopMatrix();        
    }
  
    glColor3f( 0.2, 1.0, 0.2 );
    for (PLuint i = 0; i < n.size(); i++)
    {
        glPushMatrix();
        glTranslatef( nm.x*6, nm.y*6, nm.z*6 );
        plDrawArrow(p[i], n[i], 4.0f, 0.5f);
        glPopMatrix();        
    }   
}
*/

