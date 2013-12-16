#include "plPlan.h"


plPlan::plPlan( plString filename )
{
    if (filename.compare(".csv", filename.size()-4, 4))
    {
        std::cerr << "Unrecognized suffix on filename '" << filename 
                  << "'. plPlan filenames should have suffix .csv" << std::endl;                
        exit(1);
    }
    importFile( filename );
}


plPlan::plPlan( int argc, char **argv )
{ 
    if (argc == 1)
    {
        // 0 arguments    
        std::cerr << "plPlan error: No arguments provided, an empty plan must be provided a bone and cartilage model\n";
        exit(1);
    }
    if (argc == 2)
    {
        // 1 arguments
        plString filename(argv[1]);
        if ( !filename.compare(".csv", filename.size()-4, 4) )
        {
            std::cerr << "Unrecognized suffix on filename '" << filename 
                      << "'. plPlan filenames should have suffix .csv" << std::endl;                
            exit(1);
        }
        importFile( filename );
    }
    /*
    else
    {
        // load models
        for (PLint i = 1; i < argc; i++)
        {
            // model input order: bone, cartilage, bone, cartilage, etc...
            _models.push_back( new plModel( argv[i] ) );
        }
    }
    */

}


plPlan::~plPlan()
{
    for ( PLuint i = 0; i < _defectSites.size(); i++) delete _defectSites[i];
    for ( PLuint i = 0; i < _donorSites.size();  i++) delete _donorSites[i];
    for ( PLuint i = 0; i < _grafts.size();      i++) delete _grafts[i];
    for ( PLuint i = 0; i < _iGuideSites.size(); i++) delete _iGuideSites[i];
    for ( PLuint i = 0; i < _models.size();      i++) delete _models[i];
}


void plPlan::toggleArthroView()
{ 
    plArthroViewable::toggleArthroView();
    for ( plGraft* graft : _grafts )
    {                   
        graft->toggleArthroView();
    }
    for ( plModel* model : _models )
    {             
        model->toggleArthroView(); 
    }
}   


void plPlan::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    if ( !_inArthroView )
    {  
        if ( !_isVisible )
            return;
        
        // Draw defect boundary 
        for ( PLuint i = 0; i < _defectSites.size(); i++)
        {
            plPickingStack::loadGreen( i );
            _defectSites[i]->extractRenderComponents( renderMap, technique );
        }
         
        // Draw harvest boundaries   
        for ( PLuint i = 0; i < _donorSites.size(); i++)
        {
            plPickingStack::loadGreen( i );      
            _donorSites[i]->extractRenderComponents( renderMap, technique );            
        }    

        // Draw iGuideSites
        for ( PLuint i = 0; i < _iGuideSites.size(); i++)
        {            
            plPickingStack::loadGreen( i );
            _iGuideSites[i]->extractRenderComponents( renderMap, technique );
        }

        // Draw iGuides
        for ( PLuint i = 0; i < _iGuides.size(); i++)
        {
            plPickingStack::loadGreen( i );
            _iGuides[i]->extractRenderComponents( renderMap, technique );
        }

        // Draw grafts
        for ( PLuint i = 0; i < _grafts.size(); i++)
        {                   
            plPickingStack::loadGreen( i );
            _grafts[i]->extractRenderComponents( renderMap, technique );
        }

        // draw models
        for (PLuint i =0; i < _models.size(); i++)
        {            
            plPickingStack::loadGreen( i );          
            plPickingStack::loadBlue( -1 ); // unused by models    
            _models[i]->extractRenderComponents( renderMap, technique ); 
        }
    }
    else
    {
        // Draw grafts
        for ( PLuint i = 0; i < _grafts.size(); i++)
        {                   
            plPickingStack::loadGreen( i );
            _grafts[i]->extractRenderComponents( renderMap, technique );
        }

        // draw models
        for (PLuint i =0; i < _models.size(); i++)
        {            
            plPickingStack::loadGreen( i );          
            plPickingStack::loadBlue( -1 ); // unused by models    
            _models[i]->extractRenderComponents( renderMap, technique ); 
        }
    }

}


void plPlan::extractRenderComponents( plRenderMap& renderMap ) const
{
    extractRenderComponents( renderMap, PL_PLAN_TECHNIQUE );
}


void plPlan::addDefectSite( PLuint modelIndex )
{
    if ( _models.size() < modelIndex+1 )
    {
        std::cerr << " plPlan addDonorSite() error: model ID does not exist\n";
        return;
    }
    _defectSites.push_back( new plDefectSite( _models[modelIndex]->mesh() ) );
}


void plPlan::addDonorSite( PLuint modelIndex )
{
    if ( _models.size() < modelIndex+1 )
    {
        std::cerr << " plPlan addDonorSite() error: model ID does not exist\n";
        return;
    }
    _donorSites.push_back( new plDonorSite(_models[modelIndex]->mesh() ) );
}


void plPlan::addIGuideSite( PLuint modelIndex )
{
    if ( _models.size() < modelIndex+1 )
    {
        std::cerr << " plPlan addIGuide() error: model ID does not exist\n";
        return;
    }
    _iGuideSites.push_back( new plIGuideSite( _models[modelIndex]->mesh() ) );
}


void plPlan::addGraft( const plPlug &harvest, const plPlug &recipient, PLfloat radius, PLfloat length )
{
    _grafts.push_back( new plGraft( harvest, recipient, radius, length ) );
}


void plPlan::removeDefectSite( PLuint index)
{
    delete _defectSites[index];
    _defectSites.erase( _defectSites.begin()+index );
}


void plPlan::removeDonorSite( PLuint index)
{
    delete _donorSites[index];
    _donorSites.erase( _donorSites.begin()+index );
}


void plPlan::removeIGuideSite( PLuint index)
{
    delete _iGuideSites[index];
    _iGuideSites.erase( _iGuideSites.begin()+index );
}


void plPlan::removeGraft( PLuint index )
{
    delete _grafts[index];
    _grafts.erase( _grafts.begin()+index );
}


void plPlan::importFile( const plString &filename )
{   
    plCSV csv( filename );

    for ( PLuint i = 0; i < csv.data.size(); i++)
    {           
        plString field = csv.data[i][0];    // get field name

        if (field.compareCaseInsensitive( "model") )     
        {
            std::cout << "Loading model files... \n";
              
            plString modelFile ( csv.data[++i][1] );
            
            _models.push_back( new plModel( modelFile ) );
        }        
        else if (field.compareCaseInsensitive( "defect_site") )
        {
            std::cout << "Loading defect site... \n";  
            
            PLuint     splineModelID  ( std::stoi( csv.data[++i][1] ) ); 
            plSpline   spline         ( _models[ splineModelID ]->mesh(), csv.data[++i] );
                        
            _defectSites.push_back( new plDefectSite( spline, csv.data[++i] ) );

        }
        else if (field.compareCaseInsensitive( "donor_site") )
        {
            std::cout << "Loading donor site... \n";
            
            PLuint     boundaryModelID( std::stoi( csv.data[++i][1] ) );        
            plBoundary boundary       ( PL_PICKING_TYPE_DONOR_BOUNDARY, _models[ boundaryModelID ]->mesh(), csv.data[++i] );
            
            _donorSites.push_back( new plDonorSite( boundary ) );
    
        }  
        else if (field.compareCaseInsensitive( "iGuide_site") )
        {
            std::cout << "Loading iGuide site... \n";  
            
            PLuint     boundaryModelID( std::stoi( csv.data[++i][1] ) );    
            plBoundary boundary       ( PL_PICKING_TYPE_IGUIDE_BOUNDARY, _models[ boundaryModelID ]->mesh(), csv.data[++i] );
            
            _iGuideSites.push_back( new plIGuideSite( boundary ) );
    
        }      
        else if (field.compareCaseInsensitive( "graft" ) ) 
        {       
            std::cout << "Loading graft... \n";       
            
            PLuint      recipientSiteID    ( std::stoi( csv.data[++i][1] ) );     
            plTransform recipientTransform (            csv.data[++i]      );
            plTransform recipientRotation  (            csv.data[++i]      );
            PLuint      harvestModelID     ( std::stoi( csv.data[++i][1] ) );   
            plTransform harvestTransform   (            csv.data[++i]      );
            plTransform harvestRotation    (            csv.data[++i]      );
            PLfloat     radius             ( std::stof( csv.data[++i][1] ) );  
            PLfloat     length             ( std::stof( csv.data[++i][1] ) );                            
            plVector3   markDirection      (            csv.data[++i][1]   );
                  
            plPlug recipientPlug ( _defectSites[ recipientSiteID ]->spline.surfaceMesh(), PL_PICKING_INDEX_GRAFT_DEFECT, recipientTransform, recipientRotation );
            plPlug harvestPlug   ( _models[ harvestModelID ]->mesh(),                     PL_PICKING_INDEX_GRAFT_DONOR,  harvestTransform,   harvestRotation );

            _grafts.push_back( new plGraft( harvestPlug, recipientPlug, radius, length, markDirection ) );
        }        
        else if (field.compareCaseInsensitive( "iguide" ) ) 
        {
			std::cout << "Loading iGuide... \n";   

			PLuint  siteID    ( std::stoi( csv.data[++i][1] ) );  			                 
            PLuint  plugCount ( std::stoi( csv.data[++i][1] ) );    
                                 
            std::vector<plPlugInfo> plugs;            
            for (PLuint j=0; j<plugCount; j++)   
            {   
                PLuint  graftID ( std::stoi( csv.data[i][2+(j*2)] ) ); 
                PLuint  type    ( std::stoi( csv.data[i][3+(j*2)] ) );
                //const plTransform *transform    ( &( _grafts[ graftID ]->transform( type ) ) );
                const PLfloat     *radius       ( &( _grafts[ graftID ]->radius() ) );
                const PLfloat     *length       ( &( _grafts[ graftID ]->length() ) );
                //plugs.push_back( plPlugInfo( transform, radius, length, type, graftID ) );
                plugs.push_back( plPlugInfo( NULL, radius, length, type, graftID ) );
            }
            
            PLuint  kWireCount ( std::stoi( csv.data[++i][1] ) );  
            
            std::vector<plKWire*> kWires;    
            std::vector<PLuint>   kWireIDs;         
            for (PLuint j=0; j<kWireCount; j++)   
            {  
                PLuint  kWireID ( std::stoi( csv.data[i][2+j] ) ); 
                // yadda yadda
                   
            }

            PLuint  defectSiteCount( std::stoi( csv.data[++i][1] ) );

            std::vector<const plSpline*> splines;
            std::vector<PLuint>          defectIDs;
            for (PLuint j=0; j<defectSiteCount; j++)
            {
                PLuint  defectID ( std::stoi( csv.data[i][2+(j)] ) );
                defectIDs.push_back( defectID );
                splines.push_back( &(_defectSites[defectID]->spline ) );
            }

            _iGuides.push_back( new plIGuide( _iGuideSites[ siteID ], siteID, plugs, kWires, kWireIDs, splines, defectIDs ) );
        } 
        else
        {
            std::cerr << "Error in '" << filename << "': Unrecognized word '" << field << "' in first column." << std::endl;
        }
    }
}


void plPlan::exportFile( const plString &filename )
{
    std::ofstream out( (filename + plString(".csv")).c_str() );
        
    if (!out)
    {
        std::cerr << "Could not open '" << filename << "' to store the plan." << std::endl;
    }
    else 
    { 
        // models
        for (PLuint i=0; i<_models.size(); i++) 
        {
            out << "model"              << std::endl
                << "    filename,      " << _models[i]->filename  << std::endl   
                << std::endl;
        }

        // defect sites
        for (PLuint i=0; i<_defectSites.size(); i++) 
        {    
            out << "defect_site"  << std::endl
                << "    spline_model_id,   " << _getModelIndex( _defectSites[i]->spline ) << std::endl
                << "    spline,            " << _defectSites[i]->spline             << std::endl
                << "    boundary,          " << _defectSites[i]->boundary           << std::endl   
                
                << std::endl;
        }

        // donor sites
        for (PLuint i=0; i<_donorSites.size(); i++) 
        {
            out << "donor_site"   << std::endl              
                << "    boundary_model_id, " << _getModelIndex( _donorSites[i]->boundary ) << std::endl 
                << "    boundary,          " << _donorSites[i]->boundary           << std::endl  
                << std::endl;
        }

        // iGuide sites
        for (PLuint i=0; i<_iGuideSites.size(); i++) 
        {
            out << "iguide_site"  << std::endl
                << "    boundary_model_id, " << _getModelIndex( _iGuideSites[i]->boundary ) << std::endl 
                << "    boundary,          " << _iGuideSites[i]->boundary           << std::endl   
                << std::endl;
        }

        // grafts
        for (PLuint i=0; i<_grafts.size(); i++) 
        {
            out << "graft"                          << std::endl
                << "    recipient_defect_site_id, " << _getDefectSiteIndex( _grafts[i]->recipient() )   << std::endl
                << "    recipient_transform,      " << _grafts[i]->recipient().transform()              << std::endl
                << "    recipient_rotation,       " << _grafts[i]->recipient().rotation()               << std::endl
                << "    harvest_model_id,         " << _getModelIndex( _grafts[i]->harvest() )          << std::endl
                << "    harvest_transform,        " << _grafts[i]->harvest().transform()                << std::endl
                << "    harvest_rotation,         " << _grafts[i]->harvest().rotation()                 << std::endl
                << "    radius,                   " << _grafts[i]->radius()                             << std::endl
                << "    length,                   " << _grafts[i]->length()                             << std::endl               
                << "    mark_direction,           " << _grafts[i]->markDirection()                      << std::endl
                << std::endl;
        }
         
        // iGuides
        for (PLuint i=0; i<_iGuides.size(); i++) 
        {
            out << "iguide"      << std::endl
                << "    site_id, " << _iGuides[i]->siteID  << std::endl
                << "    plugs,   " << _iGuides[i]->plugs.size();
                
            for (PLuint j=0; j<_iGuides[i]->plugs.size(); j++)
            {  
                out << ", " << _iGuides[i]->plugs[j].graftID() << ", " << _iGuides[i]->plugs[j].type();
            } 
               
            out << std::endl;
            out << "    kwires,  " << _iGuides[i]->kWires.size();
            
            for (PLuint j=0; j<_iGuides[i]->kWires.size(); j++)
            {  
                out << ", " << _iGuides[i]->kWireIDs[j];
            }

            out << std::endl;
            out << "    splines, " << _iGuides[i]->defectIDs.size();
            for (PLuint j=0; j<_iGuides[i]->defectIDs.size(); j++)
            {
                out << ", " << _iGuides[i]->defectIDs[j];
            }

            out << std::endl << std::endl;
        }
    
        std::cout << "Saved plan in CSV format in '" << filename << "'." << std::endl;
    }
}


void plPlan::updateGraftMarkerPositions()
{
    for ( plGraft* graft : _grafts )
    {
        graft->updateMarker();
    }
}


void plPlan::clear()
{
    for ( PLuint i = 0; i < _defectSites.size(); i++ ) delete _defectSites[i];
    for ( PLuint i = 0; i < _donorSites.size();  i++ ) delete _donorSites[i];
    for ( PLuint i = 0; i < _grafts.size();      i++ ) delete _grafts[i];
    for ( PLuint i = 0; i < _iGuideSites.size(); i++ ) delete _iGuideSites[i];
    for ( PLuint i = 0; i < _iGuides.size();     i++ ) delete _iGuides[i];
    _defectSites.clear();
    _donorSites.clear();
    _iGuideSites.clear();
    _grafts.clear();
    _iGuides.clear();
}


PLint plPlan::_getModelIndex( const plMeshSpecific& mesh ) const
{
    for ( PLuint i = 0; i < _models.size(); i++ )
    {
        if ( &_models[i]->mesh() == &mesh.mesh() )
            return i;        
    }
    return -1;
}


PLint plPlan::_getDefectSiteIndex( const plMeshSpecific& mesh ) const
{
    for ( PLuint i = 0; i < _defectSites.size(); i++ )
    {
        if ( &_defectSites[i]->spline.surfaceMesh() == &mesh.mesh() )
            return i;
    }
    return -1;    
}



