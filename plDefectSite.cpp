#include "plDefectSite.h" 

plDefectSite::plDefectSite() 
{
}


plDefectSite::plDefectSite( PLuint modelID, const plBoneAndCartilage &model )
    : plModelSpecific( modelID, model ), spline( model.cartilage )
{
}


plDefectSite::plDefectSite( PLuint modelID, const plBoneAndCartilage &model, const plSpline &s, const plBoundary &b )
    : plModelSpecific( modelID, model ), spline( s ), boundary( b )
{
}


/*
void plDefectSite::importCSV(const plSeq<plString> &row, const plSeq<plBoneAndCartilage*> &models )
{
    // Fill in the field            
    plString subfield = row[2];
    
    if (subfield.compareCaseInsensitive( "model") )
    {
        _modelID = plString::fromString<PLint>( row[3] ); 
        if (models.size() <= _modelID )
        {
            std::cerr << "plDefectSite importCSV() error: model ID read before model data";
            exit(1);
        }
        _model = models[_modelID];
    }                   
    else if (subfield.compareCaseInsensitive( "spline") ) 
    {     
        if (_model == NULL)
        {
            std::cerr << "plDefectSite importCSV() error: spline data read before model ID";
            exit(1);
        }    
        spline.importCSV( row, _model->cartilage );  
    }
    else if (subfield.compareCaseInsensitive( "boundary") )   
    {    
        boundary.importCSV( row );  
    }  
    else
        std::cerr << "Error importing plan, 'spline': Unrecognized word '" << subfield << "' in third column." << std::endl;      
}
*/

void plDefectSite::draw() const
{      
    if ( !_isVisible )
        return;
      
    // draw spline boundary 
    plPicking::value.type = PL_PICKING_TYPE_DEFECT_BOUNDARY;
    boundary.draw();   
     
    // draw spline corners
    spline.draw();
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
    
    plColourStack::load( 1.0, 0.2, 0.2 );
    for (PLuint i = 0; i < _s.size(); i++)
    {
        glPushMatrix();
        glTranslatef( nm.x*6, nm.y*6, nm.z*6 );
        plDraw::arrow(p[i], _s[i], 4.0f, 0.5f);
        glPopMatrix();
    }

    plColourStack::load( 0.2, 0.2, 1.0);
    for (PLuint i = 0; i < _t.size(); i++)
    {
        glPushMatrix();
        glTranslatef( nm.x*6, nm.y*6, nm.z*6 );
        plDraw::arrow(p[i], _t[i], 4.0f, 0.5f);
        glPopMatrix();        
    }
  
    plColourStack::load( 0.2, 1.0, 0.2 );
    for (PLuint i = 0; i < n.size(); i++)
    {
        glPushMatrix();
        glTranslatef( nm.x*6, nm.y*6, nm.z*6 );
        plDraw::arrow(p[i], n[i], 4.0f, 0.5f);
        glPopMatrix();        
    }   
}
*/

