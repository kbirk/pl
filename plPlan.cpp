#include "plPlan.h"

plPlan::plPlan( plString filename )
{
    if (filename.compare(filename.size()-4, 4, ".csv") != 0)
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
        if (filename.compare(filename.size()-4, 4, ".csv") != 0)
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
        
        // FIX THIS TO GET THE INPUT DIRECTORY OF MODEL FILES
        date = directory = "";  
    }

}


void plPlan::draw( PLbool picking )
{
    if (picking)
    {
        glEnable     ( GL_STENCIL_TEST );				      // need stencil testing enabled  					
	    glStencilFunc( GL_ALWAYS, 0xFF, 0xFF );               // replace where rendered 		
	    glStencilOp  ( GL_REPLACE, GL_REPLACE, GL_REPLACE );  // always replace previous bit	
	    glStencilMask( 0x01 );                                // only write to first bit
    } 

    if (isVisible)
    {
        // Draw defect boundary 
        for ( PLuint i = 0; i < _defectSites.size(); i++)
        {
            _plPickingState->id = i; 
            _defectSites[i].draw();
        }
       
        // Draw harvest boundaries   
        for ( PLuint i = 0; i < _donorSites.size(); i++)
        {
            _plPickingState->id = i;        
            _donorSites[i].draw();            
        }    

        // Draw grafts
        for ( PLuint i = 0; i < _grafts.size(); i++)
        {       
            
            _plPickingState->id = i; 
            _grafts[i].draw();
        }
        
        // Draw iGuides
        for ( PLuint i = 0; i < _iGuides.size(); i++)
        {            
            _plPickingState->id = i; 
            _iGuides[i].draw();
        }
    }      
    
    if (picking)
    {    
	    glStencilOp  ( GL_KEEP, GL_KEEP, GL_KEEP );	        // no longer modify the stencil bits  
        glStencilFunc( GL_EQUAL, 0x00, 0xFF);               // only render to bits = 0 (have not been written)
    }
    
    // draw models (draw last for proper transparency blending)
    for (PLuint i =0; i < _models.size(); i++)
    {            
        _plPickingState->id = i;    
        _models[i].draw();
    }
    
    if (picking)
    {            
        glDisable( GL_STENCIL_TEST );
    }
}


template<class T>
T &plGetImportReference( plSeq<T> &ts,  const plString &index )
{
    PLuint j = plFromString<PLuint>( index );           
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
            plBoneAndCartilage &model = plGetImportReference( _models, csv.data[i][1] ); //plGetModelReference( csv.data[i][1] ); 
            
            // read model attribute from current row
            model.readFromCSV( csv.data[i], directory );    // assumes directory attribute has been read by this point
        }
        
        else if (plStringCompareCaseInsensitive(field, "defect site") ) // read before boundary
        {
            // get reference to defect site
            plDefectSite &defectSite = plGetImportReference( _defectSites, csv.data[i][1] );
            
            // read defect site attribute from current row
            defectSite.readFromCSV( csv.data[i] );                    
        } 
        
        else if (plStringCompareCaseInsensitive(field, "donor site") ) // read before boundary
        {
            // get reference to donor region
            plDonorSite &donorSite = plGetImportReference( _donorSites, csv.data[i][1] ); 
            
            // read donor region attribute from current row
            donorSite.readFromCSV( csv.data[i] );                  
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
        _grafts[i].init( _models );       
    }

    // init deflect splines
    for ( PLuint i = 0; i < _defectSites.size(); i++) 
    {        
        _defectSites[i].computeSpline( _models ); 
    }

}

void plPlan::exportFile( plString filename )
{
    std::ofstream out( plStringConcat( filename, ".csv").c_str() );
        
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

plBoundary &plBoundaryGet(PLuint type, PLuint id)
{
    switch (type)
    {
        case PL_PICKING_TYPE_DEFECT_CORNERS:        return _plPlan->_defectSites[id].corners;            
        case PL_PICKING_TYPE_DEFECT_BOUNDARY:       return _plPlan->_defectSites[id].boundary;            
        case PL_PICKING_TYPE_DONOR_BOUNDARY:        return _plPlan->_donorSites[id].boundary;            
        case PL_PICKING_TYPE_IGUIDE_BOUNDARY:       return _plPlan->_iGuides[id].boundary;
        default:                                    return _plPlan->_defectSites[0].corners;  // default
    }
}


std::ostream& operator << ( std::ostream& out, const plPlan &p )
{
    // date / directory
    out << "date," << p.date << std::endl;
    out << "directory," << p.directory << std::endl;
    out << std::endl;

    // models
    for (PLuint i=0; i<p._models.size(); i++) 
    {
        out << "model," << i << ",bone file,"      << p._models[i].bone.getFilenameWithoutPath()        << std::endl;
        out << "model," << i << ",cartilage file," << p._models[i].cartilage.getFilenameWithoutPath()   << std::endl;
        out << std::endl;
    }
    
    // splines
    for (PLuint i=0; i<p._defectSites.size(); i++) 
    {    
        plDefectSite &defectSite = p._defectSites[i];
        
        out << "defect site," << i << ",model,"  << defectSite._modelID << std::endl;        
        out << "defect site," << i << ",corners" << static_cast<plBoundaryBase>(defectSite.corners)  << std::endl;  
        out << "defect site," << i << ",boundary" << static_cast<plBoundaryBase>(defectSite.boundary) << std::endl;  
        out << std::endl;
    }

    // donor regions
    for (PLuint i=0; i<p._donorSites.size(); i++) 
    {
        plDonorSite &donor = p._donorSites[i];
        
        out << "donor site," << i << ",model,"   << donor._modelID << std::endl;
        out << "donor site," << i << ",boundary" << static_cast<plBoundaryBase>(donor.boundary) << std::endl;  
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
        out << "iguide," << i << ", boundary" << static_cast<plBoundaryBase>(iguide.boundary) << std::endl;  
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

void plSet( plPlan &plan )
{
    _plPlan = &plan;
}


