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
    else
    {
        // 3+ arguments
        if ( (argc-1) % 3 != 0)  
        {
            std::cerr << "Model files must be submitted in twos (bone and cartilage)\n";
            exit(1);
        }
        
        // load models
        for (PLint i = 1; i < argc; i+=3)
        {
            // model input order: bone, cartilage, bone, cartilage, etc...
            _models.add( new plBoneAndCartilage( argv[i], argv[i+1], argv[i+2] ) );
        }
    }

}


plPlan::~plPlan()
{
    for ( PLuint i = 0; i < _defectSites.size(); i++) delete _defectSites[i];
    for ( PLuint i = 0; i < _donorSites.size();  i++) delete _donorSites[i];
    for ( PLuint i = 0; i < _grafts.size();      i++) delete _grafts[i];
    for ( PLuint i = 0; i < _iGuideSites.size(); i++) delete _iGuideSites[i];
    for ( PLuint i = 0; i < _models.size();      i++) delete _models[i];
}


void plPlan::drawElements() const
{
    if ( !_isVisible )
        return;

    // Draw defect boundary 
    for ( PLuint i = 0; i < _defectSites.size(); i++)
    {
        plPicking::value.id = i; 
        _defectSites[i]->draw();
    }
       
    // Draw harvest boundaries   
    for ( PLuint i = 0; i < _donorSites.size(); i++)
    {
        plPicking::value.id = i;        
        _donorSites[i]->draw();            
    }    

    // Draw grafts
    for ( PLuint i = 0; i < _grafts.size(); i++)
    {       
            
        plPicking::value.id = i; 
        _grafts[i]->draw();
    }
        
    // Draw iGuideSites
    for ( PLuint i = 0; i < _iGuideSites.size(); i++)
    {            
        plPicking::value.id = i; 
        _iGuideSites[i]->draw();
    }

    // Draw iGuides
    for ( PLuint i = 0; i < _iGuides.size(); i++)
    {
        plPicking::value.id = i;
        _iGuides[i]->draw();
    }
}

void plPlan::drawModels() const
{
    // draw models (draw last for proper transparency blending)
    for (PLuint i =0; i < _models.size(); i++)
    {            
        plPicking::value.id = i;    
        _models[i]->draw();
    }

}


void plPlan::addDefectSite( PLuint modelIndex )
{
    if ( _models.size() < modelIndex+1 )
    {
        std::cerr << " plPlan addDonorSite() error: model ID does not exist\n";
        return;
    }
    _defectSites.add( new plDefectSite( modelIndex, *_models[modelIndex] ) );
}

void plPlan::addDonorSite( PLuint modelIndex )
{
    if ( _models.size() < modelIndex+1 )
    {
        std::cerr << " plPlan addDonorSite() error: model ID does not exist\n";
        return;
    }
    _donorSites.add( new plDonorSite( modelIndex, *_models[modelIndex] ) );
}

void plPlan::addIGuideSite( PLuint modelIndex )
{
    if ( _models.size() < modelIndex+1 )
    {
        std::cerr << " plPlan addIGuide() error: model ID does not exist\n";
        return;
    }
    _iGuideSites.add( new plIGuideSite( modelIndex, *_models[modelIndex] ) );
}

void plPlan::addGraft( const plPlug &h, const plPlug &r, PLfloat radius, PLfloat cartilageThickness, PLfloat heightOffset, PLfloat length )
{
    _grafts.add( new plGraft( h, r, radius, cartilageThickness, heightOffset, length ) );
}


void plPlan::removeDefectSite( PLuint index)
{
    delete _defectSites[index];
    _defectSites.remove( index );
}


void plPlan::removeDonorSite( PLuint index)
{
    delete _donorSites[index];
    _donorSites.remove( index );
}


void plPlan::removeIGuideSite( PLuint index)
{
    delete _iGuideSites[index];
    _iGuideSites.remove( index );
}


void plPlan::removeGraft( PLuint index )
{
    delete _grafts[index];
    _grafts.remove( index );
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
              
            plString modelBoneFile ( csv.data[++i][1] );
            plString modelCartFile ( csv.data[++i][1] );
            plString modelCombFile ( csv.data[++i][1] );
            
            _models.add( new plBoneAndCartilage( modelBoneFile, modelCartFile, modelCombFile ) );
        }        
        else if (field.compareCaseInsensitive( "defect_site") )
        {
            std::cout << "Loading defect site... \n";  
            
            PLuint     modelID  ( std::stoi( csv.data[++i][1] ) ); 
            plSpline   spline   ( csv.data[++i], _models[ modelID ]->cartilage );
            plBoundary boundary ( csv.data[++i] );
            
            _defectSites.add ( new plDefectSite( modelID, *_models[ modelID ], spline, boundary ) );

        }
        else if (field.compareCaseInsensitive( "donor_site") )
        {
            std::cout << "Loading donor site... \n";
            
            PLuint     modelID  ( std::stoi( csv.data[++i][1] ) );        
            plBoundary boundary ( csv.data[++i] );
            
            _donorSites.add ( new plDonorSite( modelID, *_models[ modelID ], boundary ) );
    
        }  
        else if (field.compareCaseInsensitive( "iGuide_site") )
        {
            std::cout << "Loading iGuide site... \n";  
            
            PLuint     modelID  ( std::stoi( csv.data[++i][1] ) );    
            plBoundary boundary ( csv.data[++i] );
            
            _iGuideSites.add ( new plIGuideSite( modelID, *_models[ modelID ], boundary ) );
    
        }      
        else if (field.compareCaseInsensitive( "graft" ) ) 
        {       
            std::cout << "Loading graft... \n";       
            
            PLuint      recipientModelID   ( std::stoi( csv.data[++i][1] ) );     
            plTransform recipientTransform (            csv.data[++i]      );
            PLuint      harvestModelID     ( std::stoi( csv.data[++i][1] ) );   
            plTransform harvestTransform   (            csv.data[++i]      );            
            PLfloat     radius             ( std::stof( csv.data[++i][1] ) );  
            PLfloat     cartilageThickness ( std::stof( csv.data[++i][1] ) );  
            PLfloat     length             ( std::stof( csv.data[++i][1] ) );                
            PLfloat     heightOffset       ( std::stof( csv.data[++i][1] ) );             
            plVector3   markDirection      (            csv.data[++i][1]   );
            
            plPlug      recipientPlug ( recipientModelID, *_models[ recipientModelID ], recipientTransform );
            plPlug      harvestPlug   ( harvestModelID,   *_models[ harvestModelID   ], harvestTransform   );

            _grafts.add( new plGraft( harvestPlug, recipientPlug, radius, cartilageThickness, heightOffset, length, markDirection ) );
        }        
        else if (field.compareCaseInsensitive( "iguide" ) ) 
        {
			std::cout << "Loading iGuide... \n";   

			PLuint  siteID    ( std::stoi( csv.data[++i][1] ) );  			                 
            PLuint  plugCount ( std::stoi( csv.data[++i][1] ) );    
                                 
            plSeq<plPlugInfo> plugs;            
            for (PLuint j=0; j<plugCount; j++)   
            {   
                PLuint  graftID ( std::stoi( csv.data[i][2+(j*2)] ) ); 
                PLuint  type    ( std::stoi( csv.data[i][3+(j*2)] ) );
                plTransform *t = ( PL_PICKING_INDEX_GRAFT_DONOR == type ) ? &(_grafts[ graftID ]->harvest.transform) : &(_grafts[ graftID ]->recipient.transform); 
                
                plugs.add( plPlugInfo( t, &( _grafts[ graftID]->radius() ), type, graftID ) );
            }
            
            PLuint  kwireCount ( std::stoi( csv.data[++i][1] ) );  
            
            plSeq<plKWire*> kWires;    
            plSeq<PLuint>   kWireIDs;         
            for (PLuint j=0; j<kwireCount; j++)   
            {  
                PLuint  kWireID ( std::stoi( csv.data[i][2+j] ) ); 
                // yadda yadda
                   
            }

            PLuint  defectSiteCount( std::stoi( csv.data[++i][1] ) );

            plSeq<const plSpline*> splines;
            plSeq<PLuint>          defectIDs;
            for (PLuint j=0; j<defectSiteCount; j++)
            {
                PLuint  defectID ( std::stoi( csv.data[i][2+(j)] ) );
                defectIDs.add    ( defectID );
                splines.add      ( &(_defectSites[defectID]->spline ) );
            }

            _iGuides.add( new plIGuide( _iGuideSites[ siteID ], siteID, plugs, kWires, kWireIDs, splines, defectIDs ) );
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
                << "\tbone_file,      " << _models[i]->bone.filename()      << std::endl
                << "\tcartilage_file, " << _models[i]->cartilage.filename() << std::endl
                << "\tcombined_file,  " << _models[i]->combined.filename()  << std::endl     
                << std::endl;
        }

        // defect sites
        for (PLuint i=0; i<_defectSites.size(); i++) 
        {    
            out << "defect_site"  << std::endl
                << "\tmodel_id, " << _defectSites[i]->modelID()    << std::endl
                << "\tspline,   " << _defectSites[i]->spline       << std::endl
                << "\tboundary, " << _defectSites[i]->boundary     << std::endl   
                << std::endl;
        }

        // donor sites
        for (PLuint i=0; i<_donorSites.size(); i++) 
        {
            out << "donor_site"   << std::endl
                << "\tmodel_id, " << _donorSites[i]->modelID()    << std::endl
                << "\tboundary, " << _donorSites[i]->boundary     << std::endl   
                << std::endl;
        }

        // iGuide sites
        for (PLuint i=0; i<_iGuideSites.size(); i++) 
        {
            out << "iguide_site"  << std::endl
                << "\tmodel_id, " << _iGuideSites[i]->modelID()    << std::endl
                << "\tboundary, " << _iGuideSites[i]->boundary     << std::endl   
                << std::endl;
        }

        // grafts
        for (PLuint i=0; i<_grafts.size(); i++) 
        {
            out << "graft"                   << std::endl
                << "\trecipient_model_id,  " << _grafts[i]->recipient.modelID()   << std::endl
                << "\trecipient_transform, " << _grafts[i]->recipient.transform   << std::endl
                << "\tharvest_model_id,    " << _grafts[i]->harvest.modelID()     << std::endl
                << "\tharvest_transform,   " << _grafts[i]->harvest.transform     << std::endl
                << "\tradius,              " << _grafts[i]->radius()              << std::endl
                << "\tcartilage_thickness, " << _grafts[i]->cartilageThickness()  << std::endl
                << "\tlength,              " << _grafts[i]->length()              << std::endl               
                << "\theight_offset,       " << _grafts[i]->heightOffset()        << std::endl
                << "\tmark_direction,      " << _grafts[i]->markDirection()       << std::endl
                << std::endl;
        }
         
        // iGuides
        for (PLuint i=0; i<_iGuides.size(); i++) 
        {
            out << "iguide"      << std::endl
                << "\tsite_id, " << _iGuides[i]->siteID  << std::endl
                << "\tplugs,   " << _iGuides[i]->plugs.size();
                
            for (PLuint j=0; j<_iGuides[i]->plugs.size(); j++)
            {  
                out << ", " << _iGuides[i]->plugs[j].graftID() << ", " << _iGuides[i]->plugs[j].type();
            } 
               
            out << std::endl;
            out << "\tkwires,  " << _iGuides[i]->kWires.size();
            
            for (PLuint j=0; j<_iGuides[i]->kWires.size(); j++)
            {  
                out << ", " << _iGuides[i]->kWireIDs[j];
            }

            out << std::endl;
            out << "\tsplines, " << _iGuides[i]->defectIDs.size();
            for (PLuint j=0; j<_iGuides[i]->defectIDs.size(); j++)
            {
                out << ", " << _iGuides[i]->defectIDs[j];
            }

            out << std::endl << std::endl;
        }
    
        std::cout << "Saved plan in CSV format in '" << filename << "'." << std::endl;
    }
}


void plPlan::clear()
{
    for ( PLuint i = 0; i < _defectSites.size(); i++) delete _defectSites[i];
    for ( PLuint i = 0; i < _donorSites.size();  i++) delete _donorSites[i];
    for ( PLuint i = 0; i < _grafts.size();      i++) delete _grafts[i];
    for ( PLuint i = 0; i < _iGuideSites.size(); i++) delete _iGuideSites[i];
    for ( PLuint i = 0; i < _iGuides.size();     i++) delete _iGuides[i];
    _defectSites.clear();
    _donorSites.clear();
    _iGuideSites.clear();
    _grafts.clear();
    _iGuides.clear();
}
