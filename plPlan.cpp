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

void plPlan::addGraft( const plPlug &h, const plPlug &r, PLfloat radius, PLfloat heightOffset, PLfloat length )
{
    _grafts.add( new plGraft( h, r, radius, heightOffset, length ) );
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


/*
plIGuide &plPlan::getIGuideImportReference( plSeq<plIGuide*> &ts,  const plString &index )
{
    PLuint j = plString::fromString<PLuint>( index );
    while (ts.size() < j+1)
    {
        ts.add( new plIGuide() );  // add new elements until index exists
        ts[ts.size()-1]->grafts = &_grafts;
        ts[ts.size()-1]->sites  = &_iGuideSites;
        ts[ts.size()-1]->kWires = &_kWires;
    }
    return *ts[j];
}
*/
/*
template<class T>
T &getImportReference( plSeq<T*> &ts,  const plString &index )
{
    PLuint j = plString::fromString<PLuint>( index );           
    while (ts.size() < j+1)
    {
        ts.add( new T() );  // add new elements until index exists 
    }
    return *ts[j];
}  

template<class T>
T *getElementPointer( plSeq<T*> &ts,  PLuint index )
{        
    while (ts.size() < index+1)
    {
        ts.add( NULL );  // add new elements until index exists 
    }
    return ts[index];
}

void plPlan::importFile( plString filename )
{
    plCSV csv( filename );

    for ( PLuint i = 0; i < csv.data.size(); i++)
    {
           
        plString field = csv.data[i][0];    // get field name
        PLuint   index = plString::fromString<PLuint>( csv.data[i][1] );   // get field index

        if (field.compareCaseInsensitive( "model") )     
        {
            // get pointer to model
            plBoneAndCartilage *model = getElementPointer( _models, index );
            
            _models[index] = new plBoneAndCartilage( csv.data[i][3], csv.data[i+1][3], csv.data[i+2][3] );
            i+=2; // increment pointer to pass next line since both are read
        }
        
        else if (field.compareCaseInsensitive( "defect site") ) // read before boundary
        {
            // get reference to defect site
            plDefectSite &defectSite = getImportReference( _defectSites, csv.data[i][1] );
            // read defect site attribute from current row
            defectSite.importCSV( csv.data[i], _models );                    
        } 
        
        else if (field.compareCaseInsensitive( "donor site") ) // read before boundary
        {
            // get reference to donor region
            plDonorSite &donorSite = getImportReference( _donorSites, csv.data[i][1] );
            // read donor region attribute from current row
            donorSite.importCSV( csv.data[i], _models );                  
        } 

        else if (field.compareCaseInsensitive( "graft" ) ) 
        {    
            // get reference to graft
            plGraft &graft = getImportReference( _grafts, csv.data[i][1] );
            // read graft attribute from row
            graft.importCSV( csv.data[i], _models );
        } 
        else if (field.compareCaseInsensitive( "iguide site" ) ) 
        {  
             // get reference to iGuideSite
            plIGuideSite &iguideSite = getImportReference( _iGuideSites, csv.data[i][1] );
            // read iguidesite attribute from current row
            iguideSite.importCSV( csv.data[i], _models );
        } 
        else if (field.compareCaseInsensitive( "iguide" ) ) 
        {
            // get reference to iGuide
            plIGuide &iGuide = getImportReference ( _iGuides, csv.data[i][1]);
            // read iguide attribute from current row
            iGuide.importCSV( csv.data[i], _models );
        } 
        else
        {
            std::cerr << "Error in '" << filename << "': Unrecognized word '" << field << "' in first column." << std::endl;
        }
    }

}
*/


void plPlan::exportFile( const plString &filename )
{
    std::ofstream out( (filename + plString(".csv")).c_str() );
        
    if (!out)
    {
        std::cerr << "Could not open '" << filename << "' to store the plan." << std::endl;
    }
    else 
    {
        //out << *this;
               
        // models
        for (PLuint i=0; i<_models.size(); i++) 
        {
            out << "model"                          << std::endl
                << i                                << std::endl
                << _models[i]->bone.filename()      << std::endl
                << _models[i]->cartilage.filename() << std::endl
                << _models[i]->combined.filename()  << std::endl     
                << std::endl;
        }

        // defect sites
        for (PLuint i=0; i<_defectSites.size(); i++) 
        {    
            out << "defect_site"                 << std::endl
                << _defectSites[i]->modelID()    << std::endl
                << _defectSites[i]->spline       << std::endl
                << _defectSites[i]->boundary     << std::endl   
                << std::endl;
        }

        // donor sites
        for (PLuint i=0; i<_donorSites.size(); i++) 
        {
            out << "donor_site"                 << std::endl
                << _donorSites[i]->modelID()    << std::endl
                << _donorSites[i]->boundary     << std::endl   
                << std::endl;
        }

        // iGuide sites
        for (PLuint i=0; i<_iGuideSites.size(); i++) 
        {
            out << "iGuide_site"                 << std::endl
                << _iGuideSites[i]->modelID()    << std::endl
                << _iGuideSites[i]->boundary     << std::endl   
                << std::endl;
        }

        // grafts
        for (PLuint i=0; i<_grafts.size(); i++) 
        {
            out << "graft"                           << std::endl
                << _grafts[i]->recipient.modelID()   << std::endl
                << _grafts[i]->recipient.transform   << std::endl
                << _grafts[i]->harvest.modelID()     << std::endl
                << _grafts[i]->radius()              << std::endl
                << _grafts[i]->length()              << std::endl
                << _grafts[i]->heightOffset()        << std::endl
                << _grafts[i]->markDirection()       << std::endl
                << std::endl;
        }
    
        /*
        // iGuides
        for (PLuint i=0; i<_grafts.size(); i++) 
        {
            out << "iGuide"                          << std::endl
                << _iGuides[i]->recipient.modelID()  << std::endl
                << _iGuides[i]->recipient.transform  << std::endl
                << _iGuides[i]->harvest.modelID()    << std::endl
                << _iGuides[i]->radius()             << std::endl
                << _iGuides[i]->length()             << std::endl
                << _iGuides[i]->heightOffset()       << std::endl
                << _iGuides[i]->markDirection()      << std::endl
                << std::endl;
        }
        */
        /*
        iguide
        0
        2, 0, 1, 0, 2
        0
        */
    
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

/*
std::ostream& operator << ( std::ostream& out, const plPlan &p )
{
    // models
    for (PLuint i=0; i<p.models().size(); i++) 
    {
        const plBoneAndCartilage &model = p.models(i);
    
        out << "model," << i << ",bone file,"      << model.bone.filename()        << std::endl;
        out << "model," << i << ",cartilage file," << model.cartilage.filename()   << std::endl;
        out << std::endl;
    }
    
    // defect sites
    for (PLuint i=0; i<p.defectSites().size(); i++) 
    {    
        const plDefectSite &defectSite = p.defectSites(i);
        
        out << "defect site," << i << ",model,"   << defectSite.modelID() << std::endl;        
        out << "defect site," << i << ",spline"   << defectSite.spline  << std::endl;  
        out << "defect site," << i << ",boundary" << defectSite.boundary << std::endl;  
        out << std::endl;
    }

    // donor sites
    for (PLuint i=0; i<p.donorSites().size(); i++) 
    {
        const plDonorSite &donor = p.donorSites(i);
        
        out << "donor site," << i << ",model,"   << donor.modelID() << std::endl;
        out << "donor site," << i << ",boundary" << donor.boundary << std::endl;  
        out << std::endl;
    }

    // grafts
    for (PLuint i=0; i<p.grafts().size(); i++) 
    {
        const plGraft &graft = p.grafts(i);
                                 
        out << "graft," << i << ",height offset,"        << graft.heightOffset()      << std::endl;
        out << "graft," << i << ",radius,"               << graft.radius()            << std::endl;
        out << "graft," << i << ",length,"               << graft.length()            << std::endl;
        out << "graft," << i << ",mark direction,"       << graft.markDirection()     << std::endl;
        out << "graft," << i << ",recipient,model,"      << graft.recipient.modelID() << std::endl; 
        out << "graft," << i << ",harvest,model,"        << graft.harvest.modelID()   << std::endl;
        out << "graft," << i << ",recipient,transform"   << graft.recipient.transform << std::endl;   
        out << "graft," << i << ",harvest,transform"     << graft.harvest.transform   << std::endl; // write/read this last to properly calculate plug meshes
        
        out << std::endl;
    }

    // iGuide sites
    for (PLuint i=0; i<p.iGuideSites().size(); i++) 
    {
        const plIGuideSite &iguideSite = p.iGuideSites(i);
        out << std::endl;
        out << "iguide site," << i << ",model,"   << iguideSite.modelID() << std::endl;
        out << "iguide site," << i << ",boundary" << iguideSite.boundary  << std::endl;  
        out << std::endl;
    }

    // iGuides
    for (PLuint i=0; i<p.iGuides().size(); i++)
    {   
        const plIGuide &iguide = p.iGuides(i);
        
        // site selection
        out << "iguide," << i << ",site," << iguide.siteIndex << std::endl;

        // recipient plugs
        for (PLuint j=0; j<iguide.recipientIndices.size(); j++)
        {
            out << "iguide," << i << ",recipient," << iguide.recipientIndices[j] << std::endl;
        }

        // harvest plugs
        for (PLuint j=0; j<iguide.harvestIndices.size(); j++)
        {
            out << "iguide," << i << ",harvest," << iguide.harvestIndices[j] << std::endl;
        }

        // kwires
        for (PLuint j=0; j<iguide.kWireIndices.size(); j++)
        {
            out << "iguide," << i << ",kwire," << iguide.kWireIndices[j] << std::endl;
        }
        
    }
    
    return out;
}
*/

void plPlan::importFile( const plString &filename )
{
    
    plCSV csv( filename );

    for ( PLuint i = 0; i < csv.data.size(); i++)
    {           
        plString field = csv.data[i][0];    // get field name

        if (field.compareCaseInsensitive( "model") )     
        {
            std::cout << "Loading model files... \n";
              
            //PLuint   modelID       ( std::stoi( csv.data[++i][0] ) );   
            plString modelBoneFile ( csv.data[++i][0] );
            plString modelCartFile ( csv.data[++i][0] );
            plString modelCombFile ( csv.data[++i][0] );
            
            _models.add( new plBoneAndCartilage( modelBoneFile, modelCartFile, modelCombFile ) );

        }        
        else if (field.compareCaseInsensitive( "defect_site") ) // read before boundary
        {
            std::cout << "Loading defect site... \n";  
            
            PLuint     modelID  ( std::stoi( csv.data[++i][0] ) ); 
            plSpline   spline   ( csv.data[++i], _models[ modelID ]->cartilage );
            plBoundary boundary ( csv.data[++i]    );
            
            _defectSites.add ( new plDefectSite( modelID, *_models[ modelID ], spline, boundary ) );

        }
        else if (field.compareCaseInsensitive( "donor_site") ) // read before boundary
        {
            std::cout << "Loading donor site... \n";
            
            PLuint     modelID  ( std::stoi( csv.data[++i][0] ) );        
            plBoundary boundary ( csv.data[++i]    );
            
            _donorSites.add ( new plDonorSite( modelID, *_models[ modelID ], boundary ) );
    
        }  
        else if (field.compareCaseInsensitive( "iGuide_site") ) // read before boundary
        {
            std::cout << "Loading iGuide site... \n";  
            
            PLuint     modelID  ( std::stoi( csv.data[++i][0] ) );    
            plBoundary boundary ( csv.data[++i]    );
            
            _iGuideSites.add ( new plIGuideSite( modelID, *_models[ modelID ], boundary ) );
    
        }      
        else if (field.compareCaseInsensitive( "graft" ) ) 
        {       
            std::cout << "Loading graft... \n";       
            
            PLuint      recipientModelID   ( std::stoi( csv.data[++i][0] ) );     
            plTransform recipientTransform ( csv.data[++i]    );
            PLuint      harvestModelID     ( std::stoi( csv.data[++i][0] ) );   
            plTransform harvestTransform   ( csv.data[++i]    );
            
            PLfloat     radius             ( std::stof( csv.data[++i][0] ) );   
            PLfloat     length             ( std::stof( csv.data[++i][0] ) );   
            PLfloat     heightOffset       ( std::stof( csv.data[++i][0] ) );             
            plVector3   markDirection      ( csv.data[++i][0] );
            
            plPlug      recipientPlug( recipientModelID, *_models[ recipientModelID ], recipientTransform );
            plPlug      harvestPlug  ( harvestModelID,   *_models[ harvestModelID   ], harvestTransform   );

            _grafts.add( new plGraft( harvestPlug, recipientPlug, radius, heightOffset, length, markDirection ) );
        }        
        else if (field.compareCaseInsensitive( "iguide" ) ) 
        {
			std::cout << "Loading iGuide... \n";   

			PLuint  siteID    ( std::stoi( csv.data[++i][0] ) );  			                 
            PLuint  plugCount ( std::stoi( csv.data[++i][0] ) );    
                                 
            plSeq<plPlugInfo> plugs;            
            for (PLuint j=0; j<plugCount; j++)   
            {   
                PLuint  graftID ( std::stoi( csv.data[i][1+(j*2)] ) ); 
                PLuint  type    ( std::stoi( csv.data[i][2+(j*2)] ) );
                plTransform *t = ( PL_PICKING_INDEX_GRAFT_DONOR == type ) ? &(_grafts[ graftID ]->harvest.transform) : &(_grafts[ graftID ]->recipient.transform); 
                
                plugs.add( plPlugInfo( t, &( _grafts[ graftID]->radius() ), type, graftID ) );
            }
            
            PLuint  kwireCount ( std::stoi( csv.data[++i][0] ) );  
            
            plSeq<plKWire*> kwires;            
            for (PLuint j=0; j<kwireCount; j++)   
            {  
                PLuint  kwireID ( std::stoi( csv.data[i][1+j] ) ); 
                // yadda yadda
                
                
            }
            _iGuides.add( new plIGuide( _iGuideSites[ siteID ], plugs, kwires ) );
        } 
        else
        {
            std::cerr << "Error in '" << filename << "': Unrecognized word '" << field << "' in first column." << std::endl;
        }
    }
}

    




















