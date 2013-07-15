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
        if (filename.compare(".csv", filename.size()-4, 4))
        {
            std::cerr << "Unrecognized suffix on filename '" << filename 
                      << "'. plPlan filenames should have suffix .csv" << std::endl;                
            exit(1);
        }
        importFile( filename );
    }
    else
    {
        // 2+ arguments
        if ( (argc-1) % 2 != 0)  
        {
            std::cerr << "Model files must be submitted in twos (bone and cartilage)\n";
            exit(1);
        }
        
        // load models
        for (PLint i = 1; i < argc; i+=2)
        {
            // model input order: bone, cartilage, bone, cartilage, etc...
            _models.add( plBoneAndCartilage( argv[i], argv[i+1] ) );
        }
    }

}


void plPlan::drawElements() const
{
    if (!isVisible)
        return;

    // Draw defect boundary 
    for ( PLuint i = 0; i < _defectSites.size(); i++)
    {
        plPicking::value.id = i; 
        _defectSites[i].draw();
    }
       
    // Draw harvest boundaries   
    for ( PLuint i = 0; i < _donorSites.size(); i++)
    {
        plPicking::value.id = i;        
        _donorSites[i].draw();            
    }    

    // Draw grafts
    for ( PLuint i = 0; i < _grafts.size(); i++)
    {       
            
        plPicking::value.id = i; 
        _grafts[i].draw();
    }
        
    // Draw iGuides
    for ( PLuint i = 0; i < _iGuides.size(); i++)
    {            
        plPicking::value.id = i; 
        _iGuides[i].draw();
    }
}

void plPlan::drawModels() const
{
    // draw models (draw last for proper transparency blending)
    for (PLuint i =0; i < _models.size(); i++)
    {            
        plPicking::value.id = i;    
        _models[i].draw();
    }

}


void plPlan::addDefectSite( PLuint modelIndex )
{
    if ( _models.size() < modelIndex+1 )
    {
        std::cerr << " plPlan addDonorSite() error: model ID does not exist\n";
        return;
    }
    _defectSites.add( plDefectSite( modelIndex, _models[modelIndex] ) );
}

void plPlan::addDonorSite( PLuint modelIndex )
{
    if ( _models.size() < modelIndex+1 )
    {
        std::cerr << " plPlan addDonorSite() error: model ID does not exist\n";
        return;
    }
    _donorSites.add( plDonorSite( modelIndex, _models[modelIndex] ) );
}

void plPlan::addIGuide( PLuint modelIndex )
{
    if ( _models.size() < modelIndex+1 )
    {
        std::cerr << " plPlan addIGuide() error: model ID does not exist\n";
        return;
    }
    _iGuides.add( plIGuide( modelIndex, _models[modelIndex] ) );
}


template<class T>
T &plGetImportReference( plSeq<T> &ts,  const plString &index )
{
    PLuint j = plString::fromString<PLuint>( index );           
    while (ts.size() < j+1)
    {
        ts.add( T() );  // add new elements until index exists 
    }
    return ts[j];
}  


void plPlan::importFile( plString filename )
{
    plCSV csv( filename );

    for ( PLuint i = 0; i < csv.data.size(); i++)
    {
        plString field = csv.data[i][0];

        if (field.compareCaseInsensitive( "model") )     
        {
            // get reference to model
            plBoneAndCartilage &model = plGetImportReference( _models, csv.data[i][1] );
            // read model attribute from current row
            model.importCSV( csv.data[i] );
        }
        
        else if (field.compareCaseInsensitive( "defect site") ) // read before boundary
        {
            // get reference to defect site
            plDefectSite &defectSite = plGetImportReference( _defectSites, csv.data[i][1] );
            // read defect site attribute from current row
            defectSite.importCSV( csv.data[i], _models );                    
        } 
        
        else if (field.compareCaseInsensitive( "donor site") ) // read before boundary
        {
            // get reference to donor region
            plDonorSite &donorSite = plGetImportReference( _donorSites, csv.data[i][1] ); 
            // read donor region attribute from current row
            donorSite.importCSV( csv.data[i], _models );                  
        } 

        else if (field.compareCaseInsensitive( "graft" ) ) 
        {        
            // get reference to graft
            plGraft &graft = plGetImportReference( _grafts, csv.data[i][1] );             
            // read graft attribute from row
            graft.importCSV( csv.data[i], _models );
        } 
        else if (field.compareCaseInsensitive( "iguide" ) ) 
        {       
             // get reference to iGuide
            plIGuide &iguide = plGetImportReference( _iGuides, csv.data[i][1] ); 
            // read iguide attribute from current row
            iguide.importCSV( csv.data[i], _models );
        } 
        else
        {
            std::cerr << "Error in '" << filename << "': Unrecognized word '" << field << "' in first column." << std::endl;
        }
    }

}

void plPlan::exportFile( plString filename )
{
    std::ofstream out( (filename + plString(".csv")).c_str() );
        
    if (!out)
    {
        std::cerr << "Could not open '" << filename << "' to store the plan." << std::endl;
    }
    else 
    {
        out << *this;
        std::cout << "Saved plan in CSV format in '" << filename << "'." << std::endl;
    }
}


std::ostream& operator << ( std::ostream& out, const plPlan &p )
{
    // models
    for (PLuint i=0; i<p._models.size(); i++) 
    {
        out << "model," << i << ",bone file,"      << p._models[i].bone.filename()        << std::endl;
        out << "model," << i << ",cartilage file," << p._models[i].cartilage.filename()   << std::endl;
        out << std::endl;
    }
    
    // splines
    for (PLuint i=0; i<p._defectSites.size(); i++) 
    {    
        plDefectSite &defectSite = p._defectSites[i];
        
        out << "defect site," << i << ",model,"   << defectSite.modelID() << std::endl;        
        out << "defect site," << i << ",spline"   << defectSite.spline  << std::endl;  
        out << "defect site," << i << ",boundary" << defectSite.boundary << std::endl;  
        out << std::endl;
    }

    // donor regions
    for (PLuint i=0; i<p._donorSites.size(); i++) 
    {
        plDonorSite &donor = p._donorSites[i];
        
        out << "donor site," << i << ",model,"   << donor.modelID() << std::endl;
        out << "donor site," << i << ",boundary" << donor.boundary << std::endl;  
        out << std::endl;
    }

    // grafts
    for (PLuint i=0; i<p._grafts.size(); i++) 
    {
        plGraft &graft = p._grafts[i];
                                 
        out << "graft," << i << ",height offset,"        << graft._heightOffset       << std::endl;
        out << "graft," << i << ",radius,"               << graft._radius             << std::endl;
        out << "graft," << i << ",length,"               << graft._length             << std::endl;
        out << "graft," << i << ",mark direction,"       << graft._markDirection      << std::endl;
        out << "graft," << i << ",recipient,model,"      << graft.recipient.modelID() << std::endl; 
        out << "graft," << i << ",harvest,model,"        << graft.harvest.modelID()   << std::endl;
        out << "graft," << i << ",recipient,transform"   << graft.recipient.transform << std::endl;   
        out << "graft," << i << ",harvest,transform"     << graft.harvest.transform   << std::endl; // write/read this last to properly calculate plug meshes
        
        out << std::endl;
    }

    // iGuides
    for (PLuint i=0; i<p._iGuides.size(); i++) 
    {
        plIGuide &iguide = p._iGuides[i];

        out << std::endl;
        out << "iguide," << i << ",model,"   << iguide.model() << std::endl;
        out << "iguide," << i << ",boundary" << iguide.boundary << std::endl;  
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

