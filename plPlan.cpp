#include "plPlan.h"

plPlan::plPlan() 
{
   defaultInit(); 
}

plPlan::plPlan( plString filename ) 
{      
    if (filename.compare(filename.size()-4, 4, ".csv") != 0)
    {
        std::cerr << "Unrecognized suffix on filename '" << filename 
                  << "'. plPlan filenames should have suffix .csv" << std::endl;                
        defaultInit();
        return;
    }

    readFile( filename );     
}

void plPlan::defaultInit()
{
    date = directory = "";   
}

void plPlan::draw()
{
    if (!_isVisible)
        return;
        
    // Draw defect boundary 
    for ( PLuint i = 0; i < _defectSplines.size(); i++)
    {
        _plPickingState->id = i; 
        _defectSplines[i].draw();
    }
   
    // Draw harvest boundaries   
    for ( PLuint i = 0; i < _donorRegions.size(); i++)
    {
        //_plPickingState->type = PL_PICKING_TYPE_DONOR_BOUNDARY; 
        _plPickingState->id = i;        
        _donorRegions[i].draw();            
    }    

    // Draw grafts
    for ( PLuint i = 0; i < _grafts.size(); i++)
    {       
        _plPickingState->type = PL_PICKING_TYPE_GRAFT; 
        _plPickingState->id = i; 
        _grafts[i].draw();
    }
    
    /*
    // Draw iGuides
    if (selectedIGuide != -1)
    {
        iGuides[selectedIGuide].draw(plVector3(IGUIDE_BOUNDARY_COLOUR), ctrlPointColourPickingValue, state.selectedCtrlPoint );
        ctrlPointColourPickingValue+=MAX_CTRL_POINTS;
    }
    */
}

// to remove the dependancy that plan elements are included in sequential order, these functions ensures that indices are
// consistant between plan files and runtime plans
plBoneAndCartilage &plGetModelReference( const plString &index )
{
    PLuint j = atoi( index.c_str() ); 
        
    while (plModelCount() < j+1)
    {
        // add new elements until index exists    
        plBoneAndCartilage *m = new plBoneAndCartilage();        
    }
    
    return plModelGet(j);

} 

template<class T>
T &plGetImportReference( plSeq<T> &ts,  const plString &index )
{
    PLuint j = atoi( index.c_str() );           
    while (ts.size() < j+1)
    {
        ts.add( T() );  // add new elements until index exists 
    }
    return ts.back();
}  

void plPlan::readFile( plString filename )
{
    plCSV csv( filename );
    //plBoneAndCartilage model;// = new plBoneAndCartilage();
    
    for ( PLuint i = 0; i < csv.data.size(); i++)
    {
        plString field = csv.data[i][0];

        if (plStringCompareCaseInsensitive(field, "date") )
        {
            date = csv.data[i][1];
        }
        else if (plStringCompareCaseInsensitive(field, "directory") ) 
        {
            directory = csv.data[i][1];
        } 
        else if (plStringCompareCaseInsensitive(field, "model") )     
        {
            // get reference to model
            plBoneAndCartilage &model = plGetModelReference( csv.data[i][1] ); 
            
            // read model attribute from current row
            model.readFromCSV( csv.data[i] );
        }
        
        else if (plStringCompareCaseInsensitive(field, "spline") ) // read before boundary
        {
            // get reference to spline
            plSpline &spline = plGetImportReference( _defectSplines, csv.data[i][1] );
            
            // read spline attribute from current row
            spline.readFromCSV( csv.data[i] );                    
        } 
        
        else if (plStringCompareCaseInsensitive(field, "donor region") ) // read before boundary
        {
            // get reference to donor region
            plDonorSite &donorRegion = plGetImportReference( _donorRegions, csv.data[i][1] ); 
            
            // read donor region attribute from current row
            donorRegion.readFromCSV( csv.data[i] );                  
        } 

        else if (plStringCompareCaseInsensitive(field, "graft" ) ) 
        {        
            // get reference to graft
            plGraft &graft = plGetImportReference( _grafts, csv.data[i][1] ); 
            
            // read graft attribute from row
            graft.readFromCSV( csv.data[i] );
        } 
        else if (plStringCompareCaseInsensitive(field, "iguide" ) ) 
        {       
             // get reference to iGuide
            plIGuide &iguide = plGetImportReference( _iGuides, csv.data[i][1] ); 

            // read iguide attribute from current row
            iguide.readFromCSV(csv.data[i]);
        } 
        else
        {
            std::cerr << "Error in '" << filename << "': Unrecognized word '" << field << "' in first column." << std::endl;
        }
    }

    // init grafts    
    for ( PLuint i = 0; i < _grafts.size(); i++) 
    {                              
        _grafts[i].init();        
    }

    // init donor regions
    for ( PLuint i = 0; i < _donorRegions.size(); i++) 
    {
        _donorRegions[i].init(); 
    }

    // init donor regions
    for ( PLuint i = 0; i < _defectSplines.size(); i++) 
    {        
        _defectSplines[i].init(); 
    }
}


plBoundary &plBoundaryGet(PLuint type, PLuint id)
{
    switch (type)
    {
        case PL_PICKING_TYPE_DEFECT_CORNERS:        return _plPlan->_defectSplines[id].corners;            
        case PL_PICKING_TYPE_DEFECT_BOUNDARY:       return _plPlan->_defectSplines[id].boundary;            
        case PL_PICKING_TYPE_DONOR_BOUNDARY:        return _plPlan->_donorRegions[id].boundary;            
        case PL_PICKING_TYPE_IGUIDE_BOUNDARY:       return _plPlan->_iGuides[id].boundary;
        default:                                    return _plPlan->_defectSplines[0].corners;  // default
    }
}


std::ostream& operator << ( std::ostream& out, const plPlan &p )
{
    // date / directory
    out << "date," << p.date << std::endl;
    out << "directory," << p.directory << std::endl;
    out << std::endl;

    // models
    for (PLuint i=0; i<_plBoneAndCartilageModels.size(); i++) 
    {
        out << "model," << i << ",bone file,"      << _plBoneAndCartilageModels[i]->_bone        << std::endl;
        out << "model," << i << ",cartilage file," << _plBoneAndCartilageModels[i]->_cartilage   << std::endl;
        out << std::endl;
    }
    
    // splines
    for (PLuint i=0; i<p._defectSplines.size(); i++) 
    {    
        plSpline &spline = p._defectSplines[i];
        
        out << "spline," << i << ",model,"  << spline._modelID << std::endl;        
        out << "spline," << i << ",corners" << spline.corners  << std::endl;  
        out << "spline," << i << ",boundary" << spline.boundary << std::endl;  
        out << std::endl;
    }

    // donor regions
    for (PLuint i=0; i<p._donorRegions.size(); i++) 
    {
        plDonorSite &donor = p._donorRegions[i];
        
        out << "donor region," << i << ",model,"   << donor._modelID << std::endl;
        out << "donor region," << i << ",boundary" << donor.boundary << std::endl;  
        out << std::endl;
    }

    // grafts
    for (PLuint i=0; i<p._grafts.size(); i++) 
    {
        plGraft &graft = p._grafts[i];
        
        out << "graft," << i << ",harvest model,"         << graft._harvestModelID                 << std::endl;
        out << "graft," << i << ",recipient model,"       << graft._recipientModelID               << std::endl;       
        out << "graft," << i << ",height offset,"         << graft._heightOffset                   << std::endl;
        out << "graft," << i << ",radius,"                << graft._radius                         << std::endl;
        out << "graft," << i << ",length,"                << graft._length                         << std::endl;
        out << "graft," << i << ",mark direction,"        << graft._markDirection                  << std::endl;
        out << "graft," << i << ",recipient origin,"      << graft.recipientTransform.origin()     << std::endl;
        out << "graft," << i << ",recipient x,"           << graft.recipientTransform.x()          << std::endl;
        out << "graft," << i << ",recipient y,"           << graft.recipientTransform.y()          << std::endl;       
        out << "graft," << i << ",harvest origin,"        << graft.harvestTransform.origin()       << std::endl;
        out << "graft," << i << ",harvest x,"             << graft.harvestTransform.x()            << std::endl;
        out << "graft," << i << ",harvest y,"             << graft.harvestTransform.y()            << std::endl;
        out << std::endl;
    }

    // iGuides
    for (PLuint i=0; i<p._iGuides.size(); i++) 
    {
        plIGuide &iguide = p._iGuides[i];

        out << std::endl;
        out << "iguide," << i << ", boundary" << iguide.boundary << std::endl;  
        out << "iguide," << i << ",graft indices";
        for (PLuint j=0; j<iguide.graftIndices.size(); j++)
        {
            out << "," << iguide.graftIndices[j];
        }
        out << std::endl;

        for (PLuint j=0; j<iguide.kwires.size(); j++)
        {
            out << "iguide," << i << ",kwire," << j << "," << iguide.kwires[j].position << "," << iguide.kwires[j].direction << std::endl;
        }
        out << std::endl;
    }
    
    return out;
}

/*
void plPlan::outputForManuela()
{
    std::plVector3<Graft>::const_iterator graft_itr = grafts.begin();
    std::plVector3<Graft>::const_iterator graft_end = grafts.end();
    int graftIndex = 0; 
    for ( ; graft_itr != graft_end; ++graft_itr)
    {

        // Open a file for this graft
        char filename[100];
        sprintf( filename, "graft_%d.dat", graftIndex );
        std::ofstream out( filename );
        if (!out) 
        {
            std::cerr << "Could not open '" << filename << "' to store plan." << std::endl;
            continue;
        }

        out << "Height: " << graft_itr->height << std::endl;
        out << "_radius: " << graft_itr->_radius << std::endl;
        out << "Bone height: " << graft_itr->boneHeight << std::endl;
        out << "LevelDirection: [" << graft_itr->_markDirection.x << ", " << graft_itr->_markDirection.y << ", " << graft_itr->_markDirection.z << "]" << std::endl;
        out << std::endl;
        out << std::endl;
        out << "RecipientSide" << std::endl;
        out << "Translation: [" << graft_itr->recipientTransform.translation.x << ", " << graft_itr->recipientTransform.translation.y << ", " << graft_itr->recipientTransform.translation.z << "]" << std::endl;
        out << "Direction: [" << graft_itr->recipientTransform.direction.x << ", " << graft_itr->recipientTransform.direction.y << ", " << graft_itr->recipientTransform.direction.z << "]" << std::endl;
        out << "UpDirection: [" << graft_itr->recipientTransform.upDirection.x << ", " << graft_itr->recipientTransform.upDirection.y << ", " << graft_itr->recipientTransform.upDirection.z << "]" << std::endl;
        out << std::endl;
        out << "HarvestSide" << std::endl;
        out << "Translation: [" << graft_itr->harvestTransform.translation.x << ", " << graft_itr->harvestTransform.translation.y << ", " << graft_itr->harvestTransform.translation.z << "]" << std::endl;
        out << "Direction: [" << graft_itr->harvestTransform.direction.x << ", " << graft_itr->harvestTransform.direction.y << ", " << graft_itr->harvestTransform.direction.z << "]" << std::endl;
        out << "UpDirection: [" << graft_itr->harvestTransform.upDirection.x << ", " << graft_itr->harvestTransform.upDirection.y << ", " << graft_itr->harvestTransform.upDirection.z << "]" << std::endl;
        out << std::endl;
        out << std::endl;
        out << "Difference of min and max heights: 0" << std::endl;
        out << "Min Height: 0" << std::endl;
        out << "Max Height: 0" << std::endl; 
        graftIndex++;
    }
}
*/

//////////////////////////////////////

void plPlanImport( plString plan_file )
{
    delete _plPlan;
    _plPlan = new plPlan( plan_file );
}

void plPlanExport ( plString plan_file )
{
    static PLint planNum = 1;    

    std::stringstream filename;
    filename << plan_file << "-" << planNum << ".csv";
    std::ofstream out( filename.str().c_str());
        
    if (!out)
    {
        std::cerr << "Could not open '" << filename.str() << "' to store the plan." << std::endl;
    }
    else 
    {
        out << *_plPlan;
        std::cout << "Saved plan in CSV format in '" << filename.str() << "'." << std::endl;
        planNum++;
    }
}

void plPlanCreateNew()
{
    delete _plPlan;
    _plPlan = new plPlan();
}

void plPlanToggleVisibility()
{
    _plPlan->toggleVisibility();
}

//////////////////////////////////////////////

void plBoundaryPointSelect(PLuint index)
{
    if (plErrorIsBoundarySelected("plBoundaryPointSelect"))
        return;

    _plState->selectBoundaryPoint(index);     
}

void plBoundaryPointMove(const plVector3 &point, const plVector3 &normal)
{
    if (plErrorIsBoundarySelected("plBoundaryPointMove"))
        return;

    if (_plState->boundarySelectedPointID < 0)   // wall is selected
        return;

    plBoundary &boundary = plBoundaryGet(_plState->boundarySelectedType, _plState->boundarySelectedID);
    boundary.movePointAndNormal(_plState->boundarySelectedPointID, point, normal);    
    boundary.updateMesh();
}

void plBoundaryPointMove( PLuint x, PLuint y )
{
    if (plErrorIsBoundarySelected( "plBoundaryPointMove" ) )
        return;
        
    if (_plState->boundarySelectedPointID < 0)   // wall is selected
        return;
        
    plVector3 mouseInWorld = plWindowGetMouseToWorldPos(x, y, 0);
    plVector3 rayOrigin = plCameraGetPosition();
    plVector3 rayDirection = (mouseInWorld - rayOrigin).normalize();

    plIntersection intersection = plModelCartilageIntersect(0, rayOrigin, rayDirection);

    if (intersection.exists) 
    {     
        plBoundaryPointMove(intersection.point, intersection.normal);
    }

    if (_plState->boundarySelectedType == PL_PICKING_TYPE_DEFECT_CORNERS && _plPlan->_defectSplines[_plState->boundarySelectedID].corners.size() == 4)
    {
        // recompute hermite spline
        _plPlan->_defectSplines[_plState->boundarySelectedID].computeHermiteSpline();
    }
}

PLint plBoundaryPointAdd( PLuint x, PLuint y )
{
    if (plErrorIsBoundarySelected( "plBoundaryPointAdd" ))
        return -1;

    plVector3 mouseInWorld = plWindowGetMouseToWorldPos(x, y,0);
    plVector3 rayOrigin = plCameraGetPosition();
    plVector3 rayDirection = (mouseInWorld - rayOrigin).normalize();

    plIntersection intersection = plModelCartilageIntersect(0, rayOrigin, rayDirection);

    if (intersection.exists) 
    {     
        return plBoundaryPointAdd(intersection.point, intersection.normal);
    }

    return -1;  // no cartilage at point
}


PLint plBoundaryPointAdd(const plVector3 &point, const plVector3 &normal)
{
    if (plErrorIsBoundarySelected( "plBoundaryPointAdd" ))
        return -1;
        
    plBoundary &boundary = plBoundaryGet(_plState->boundarySelectedType, _plState->boundarySelectedID);
    
    // limit spline corners to 4 points
    if (_plState->boundarySelectedType == PL_PICKING_TYPE_DEFECT_CORNERS && boundary.size() > 3)
    {   
        // already 4 corner points    
        return -1;
    }
    
    PLint ret = boundary.addPointAndNormal(point, normal);
    boundary.updateMesh();
    
    if (_plState->boundarySelectedType == PL_PICKING_TYPE_DEFECT_CORNERS && boundary.size() > 3)
    {
        // just added 4th point, compute spline    
        return -1;
    }
    return ret;
}

void plBoundaryPointRemove()
{
    plBoundaryPointRemove( _plState->boundarySelectedPointID );   
}

void plBoundaryPointRemove( PLuint point_index )
{
    if (plErrorIsBoundarySelected( "plBoundaryPointRemove" ))
        return; 
        
    plBoundary &boundary = plBoundaryGet(_plState->boundarySelectedType, _plState->boundarySelectedID);
    
    boundary.removePointAndNormal(point_index);
    boundary.updateMesh();
    
    _plState->boundarySelectedPointID = -1;  
}

PLuint plDonorRegionCount()
{
    return _plPlan->_donorRegions.size();
}

void plDonorRegionToggleVisibility( PLuint region_id )
{
    if (plErrorCheckDonorRegionBounds(region_id, "plDonorRegionToggleVisibility"))
        return;
        
    _plPlan->_donorRegions[region_id].toggleVisibility();    
}

PLuint plDefectSplineCount()
{
    return _plPlan->_donorRegions.size();
}


void plDonorRegionToggleVisibilityAll()
{
    for (PLuint i = 0; i < _plPlan->_donorRegions.size(); i++)
    {
        _plPlan->_donorRegions[i].toggleVisibility(); 
    }  
}

void plDefectSplineToggleVisibility( PLuint region_id )
{
    if (plErrorCheckDefectSplineBounds(region_id, "plDefectSplineToggleVisibility"))
        return;
    
    _plPlan->_defectSplines[region_id].toggleVisibility();
}

void plDefectSplineToggleVisibilityAll()
{
    for (PLuint i = 0; i < _plPlan->_donorRegions.size(); i++)
    {
        _plPlan->_defectSplines[i].toggleVisibility(); 
    }  
}

void plDefectSplineCornersToggleVisibility( PLuint region_id )
{
    if (plErrorCheckDefectSplineBounds(region_id, "plDefectSplineCornersToggleVisibility"))
        return;
        
    _plPlan->_defectSplines[region_id].corners.toggleVisibility(); 
}

void plDefectSplineCornersToggleVisibilityAll()
{
    for (PLuint i = 0; i < _plPlan->_defectSplines.size(); i++)
    {
        _plPlan->_defectSplines[i].corners.toggleVisibility(); 
    } 
}

void plDefectSplineBoundaryToggleVisibility( PLuint region_id )
{
    if (plErrorCheckDefectSplineBounds(region_id, "plDefectSplineBoundaryToggleVisibility"))
        return;
        
    _plPlan->_defectSplines[region_id].boundary.toggleVisibility(); 
}

void plDefectSplineBoundaryToggleVisibilityAll()
{
    for (PLuint i = 0; i < _plPlan->_defectSplines.size(); i++)
    {
        _plPlan->_defectSplines[i].boundary.toggleVisibility(); 
    } 
} 
 
////////////////////////////////

PLuint plGraftCount()
{
    return _plPlan->_donorRegions.size();
}

plGraft& plGraftGet( PLuint graft_id )
{
    if (plErrorCheckGraftBounds(graft_id, "plGraftGet"))
        return _plPlan->_grafts[0];
        
    return _plPlan->_grafts[graft_id];
}

plTransform plGraftGetHarvestTransform( PLuint graft_id )
{
    if (plErrorCheckGraftBounds(graft_id, "plGraftGetHarvestTransform"))
        return plTransform();
        
    return _plPlan->_grafts[graft_id].harvestTransform;
} 

plTransform plGraftGetRecipientTransform( PLuint graft_id )
{
    if (plErrorCheckGraftBounds(graft_id, "plGraftGetRecipientTransform"))
        return plTransform();
        
    return _plPlan->_grafts[graft_id].recipientTransform;
} 

plVector3 plGraftGetHarvestXAxis( PLuint graft_id )
{
    if (plErrorCheckGraftBounds(graft_id, "plGraftGetHarvestXAxis"))
        return plVector3();
        
    return _plPlan->_grafts[graft_id].harvestTransform.x();    
}

plVector3 plGraftGetHarvestYAxis( PLuint graft_id )
{
    if (plErrorCheckGraftBounds(graft_id, "plGraftGetHarvestYAxis"))
        return plVector3();
        
    return _plPlan->_grafts[graft_id].harvestTransform.y(); 
}

plVector3 plGraftGetHarvestZAxis( PLuint graft_id )
{
    if (plErrorCheckGraftBounds(graft_id, "plGraftGetHarvestZAxis"))
        return plVector3();
        
    return _plPlan->_grafts[graft_id].harvestTransform.z(); 
}

plVector3 plGraftGetHarvestOrigin( PLuint graft_id )
{
    if (plErrorCheckGraftBounds(graft_id, "plGraftGetHarvestOrigin"))
        return plVector3();
        
    return _plPlan->_grafts[graft_id].harvestTransform.origin();  
}

plVector3 plGraftGetRecipientXAxis( PLuint graft_id )
{
    if (plErrorCheckGraftBounds(graft_id, "plGraftGetRecipientXAxis"))
        return plVector3();
        
    return _plPlan->_grafts[graft_id].recipientTransform.x();
}

plVector3 plGraftGetRecipientYAxis( PLuint graft_id )
{
    if (plErrorCheckGraftBounds(graft_id, "plGraftGetRecipientYAxis"))
        return plVector3();
        
    return _plPlan->_grafts[graft_id].recipientTransform.y();
}

plVector3 plGraftGetRecipientZAxis( PLuint graft_id )
{
    if (plErrorCheckGraftBounds(graft_id, "plGraftGetRecipientZAxis"))
        return plVector3();
        
    return _plPlan->_grafts[graft_id].recipientTransform.z();
}

plVector3 plGraftGetRecipientOrigin( PLuint graft_id )
{
    if (plErrorCheckGraftBounds(graft_id, "plGraftGetRecipientOrigin"))
        return plVector3();
        
    return _plPlan->_grafts[graft_id].recipientTransform.origin();
}

void plGraftToggleVisibility( PLuint graft_id )
{
    if (plErrorCheckGraftBounds(graft_id, "plGraftGetRecipientXAxis"))
        return;
    
    _plPlan->_grafts[graft_id].toggleVisibility();       
}

void plGraftToggleVisibilityAll()
{
    for (PLuint i = 0; i < _plPlan->_grafts.size(); i++)
    {
        _plPlan->_grafts[i].toggleVisibility(); 
    }    
}


plVector3 _plGraftGetScreenAxis( const plVector3 &edit_axis, const plVector3 &world_position)
{
    plVector3 graftScreenOrigin = plWindowGetWorldToScreenPos( world_position.x, 
                                                             world_position.y, 
                                                             world_position.z );
    
    plVector3 graftScreenAxisTip = plWindowGetWorldToScreenPos( edit_axis.x + world_position.x, 
                                                              edit_axis.y + world_position.y, 
                                                              edit_axis.z + world_position.z );

    return (graftScreenAxisTip - graftScreenOrigin).normalize();
}

void plGraftSetDragOrigin ( PLint x, PLint y )
{
    if (plErrorIsGraftSelected("plGraftSetDragOrigin"))
        return;

    _plState->graftInitialDragPos = plVector3(x,y,0);
                
    if (PL_GRAFT_SELECTED_IS_DONOR)
    {
        _plState->graftInitialTransform = plGraftGetHarvestTransform(_plState->graftSelectedID);
    }
    else
    {
        _plState->graftInitialTransform = plGraftGetRecipientTransform(_plState->graftSelectedID);
    }  
                                                        
    PLint type = plPickingGetType(x,y); 
                                                                       
    switch (type) 
    {
        case PL_PICKING_TYPE_GRAFT_HANDLE_X: 
            
            _plState->graftEditAxis = _plState->graftInitialTransform.x(); 
            _plState->graftInitialPlaneNormal = _plState->graftInitialTransform.z();
            break;
            
        case PL_PICKING_TYPE_GRAFT_HANDLE_Y: 
        
            _plState->graftEditAxis = _plState->graftInitialTransform.y();             
            break;
            
        case PL_PICKING_TYPE_GRAFT_HANDLE_Z: 
            _plState->graftEditAxis = _plState->graftInitialTransform.z(); 
            _plState->graftInitialPlaneNormal = -_plState->graftInitialTransform.x();
            break;
    }                                                         
                                              
    _plState->graftScreenAxis = _plGraftGetScreenAxis(_plState->graftEditAxis, _plState->graftInitialTransform.origin());      
}

void plGraftDragEdit( PLint x, PLint y )
{
    if (plErrorIsGraftSelected("plGraftDragEdit"))
        return;

    plGraft &graft = _plPlan->_grafts[_plState->graftSelectedID];   

    plVector3 screenDragVector = plVector3(x,y,0) - _plState->graftInitialDragPos;
    
    switch (_plState->graftEditMode)
    {
        case PL_GRAFT_EDIT_MODE_TRANSLATE:
        {
            // translation
            
            plTransform &transform = (PL_GRAFT_SELECTED_IS_DONOR) ? graft.harvestTransform : graft.recipientTransform;           
            
            PLfloat distOnAxis = (screenDragVector * _plState->graftScreenAxis)/10.0f;

            if (_plState->graftEditAxis == transform.y())  
            {
                // translating along y
                graft.adjustHeightOffset(distOnAxis);  
                break;              
            }
            
            distOnAxis = plClamp(distOnAxis);

            plVector3 rayOrigin = _plState->graftInitialTransform.origin() + (distOnAxis * _plState->graftEditAxis);

            plIntersection intersection = plModelBoneIntersect(0, rayOrigin, -transform.y());

            if (intersection.exists)
            {   
                _plState->graftInitialTransform.origin(intersection.point); //update
                
                PLfloat normal_radius = 6.0f;
                plVector3 normal = plModelBoneGetAvgNormal( 0, normal_radius, transform.origin(), transform.y());
                
                plVector3 x,y,z;
                if (_plState->graftEditAxis == transform.x())  
                {
                    // translating along x                    
                    x = (normal ^ _plState->graftInitialPlaneNormal);                       
                    y = normal;                        
                    z = (x ^ y);
                    _plState->graftEditAxis = x;    
                }
                else 
                {
                    // translating along z
                    z = (normal ^ _plState->graftInitialPlaneNormal);                       
                    y = normal;                         
                    x = (y ^ z);     
                    _plState->graftEditAxis = z; 

                } 
                transform.set( x, y, z, intersection.point );                   

                if (PL_GRAFT_SELECTED_IS_DONOR)
                {
                    graft.setCaps();
                }
            }
                    
            break;
        }
        
        case PL_GRAFT_EDIT_MODE_ROTATE:
        {
            // rotation

            plVector3 screenAxisNormal(-_plState->graftScreenAxis.y, _plState->graftScreenAxis.x, 0 );
            PLfloat angle = -(screenDragVector * screenAxisNormal);
            plGraftRotate(_plState->graftSelectedID, _plState->graftEditAxis, angle);          
            break;
        }
        
        case PL_GRAFT_EDIT_MODE_LENGTH:
        {
            // _length
            
            PLfloat length_delta = screenDragVector * (_plState->graftScreenAxis).normalize(); 
            graft.adjustLength(-length_delta/10) ;                            
            graft.updateCartilageMesh();
            graft.updateBoneMesh();
            break;
        }
        
        case PL_GRAFT_EDIT_MODE_MARKER:
        {
            // marker
            
            plVector3 screenAxisNormal(-_plState->graftScreenAxis.y, _plState->graftScreenAxis.x, 0 );
            PLfloat angle = -(screenDragVector * screenAxisNormal);           
            plGraftSpinMarker( angle );
            break;
        }
    } 
    
    // update initial drag position
    _plState->graftInitialDragPos = plVector3(x,y,0);          
}


void plGraftSurfaceTranslate( const plVector3 &translation )
{
    if (plErrorIsGraftSelected("plGraftSurfaceTranslate"))
        return;
    
    plGraftSurfaceTranslate( _plState->graftSelectedID, _plState->graftSelectedIndex, translation);
}

void plGraftSurfaceTranslate( PLuint graft_id, PLuint graft_index, const plVector3 &translation )
{ 
    plGraft     &graft     = _plPlan->_grafts[graft_id];            
    plTransform &transform = (graft_index == PL_PICKING_INDEX_GRAFT_DONOR) ? graft.harvestTransform : graft.recipientTransform;

    plVector3 point, normal;
    plIntersection intersection = plModelBoneIntersect( 0, transform.origin() + translation, -transform.y());

    if (intersection.exists)
    {   
        PLfloat normal_radius = 6.0f;
        plVector3 normal = plModelBoneGetAvgNormal( 0, normal_radius, transform.origin(), transform.y());
        
        // translate
        plVector3 y = normal;
        plVector3 x = (transform.y() ^ transform.z());  
        plVector3 z = transform.z(); 
        transform.set ( x, y, z, intersection.point);    

        if (graft_index == PL_PICKING_INDEX_GRAFT_DONOR)
        {
            // harvest, re-compute cap  
            graft.setCaps();
        }
    }
}

void plGraftRotate( const plVector3 &axis, PLfloat angle_degrees )
{
    if (plErrorIsGraftSelected("plGraftRotate"))
        return;
    
    plGraftRotate( _plState->graftSelectedID, axis, angle_degrees);
}

void plGraftRotate( PLuint graft_id, const plVector3 &axis, PLfloat angle_degrees )
{
    plGraft     &graft     = _plPlan->_grafts[graft_id];      
    plTransform &transform = (PL_GRAFT_SELECTED_IS_DONOR) ? graft.harvestTransform : graft.recipientTransform;
  
    plMatrix44 rot; rot.setRotationD(angle_degrees,  axis);

    transform.set( rot * transform.x(), rot * transform.y() );  

    if (PL_GRAFT_SELECTED_IS_DONOR)
    {
        // harvest, re-compute cap      
        graft.setCaps();    
    }
}

void plGraftSpinMarker( PLfloat angle_degrees )
{
    if (plErrorIsGraftSelected("plGraftSpinMarker"))
        return;
    
    plGraftSpinMarker( _plState->graftSelectedID, angle_degrees);
}

void plGraftSpinMarker( PLuint graft_id, PLfloat angle_degrees )
{
    _plPlan->_grafts[graft_id].spinMark(angle_degrees);
}

//////////////////////////////////////////////////

void plDefectSplineSetDragOrigin ( PLint x, PLint y )
{
    /*
    if (plErrorIsDefectSplineSelected("plDefectSplineSetDragOrigin"))
        return;

    _plState->graftInitialDragPos = plVector3(x,y,0);
                
    if (PL_GRAFT_SELECTED_IS_DONOR)
    {
        _plState->graftInitialTransform = plGraftGetHarvestTransform(_plState->graftSelectedID);
    }
    else
    {
        _plState->graftInitialTransform = plGraftGetRecipientTransform(_plState->graftSelectedID);
    }  
                                                        
    PLint type = plPickingGetType(x,y); 
                                                                       
    switch (type) 
    {
        case PL_PICKING_TYPE_GRAFT_HANDLE_X: 
            
            _plState->graftEditAxis = _plState->graftInitialTransform.x; 
            _plState->graftInitialPlaneNormal = _plState->graftInitialTransform.z;
            break;
            
        case PL_PICKING_TYPE_GRAFT_HANDLE_Y: 
        
            _plState->graftEditAxis = _plState->graftInitialTransform.y;             
            break;
            
        case PL_PICKING_TYPE_GRAFT_HANDLE_Z: 
            _plState->graftEditAxis = _plState->graftInitialTransform.z; 
            _plState->graftInitialPlaneNormal = -_plState->graftInitialTransform.x;
            break;
    }                                                         
                                              
    _plState->graftScreenAxis = _plGraftGetScreenAxis(_plState->graftEditAxis, _plState->graftInitialTransform.origin);      
    */
}














