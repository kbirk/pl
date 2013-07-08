#include "plDonorSite.h" 

plDonorSite::plDonorSite() 
{
    boundary
}

void plDonorSite::readFromCSV(const plSeq<plString> &row)
{
    plString subfield = row[2];
            
    if (plStringCompareCaseInsensitive(subfield, "model") )
    {
        _modelID = atof( row[3].c_str() );
    }                   
    else if (plStringCompareCaseInsensitive(subfield, "boundary") )   
    {            
        boundary.readFromCSV( row );  
    } 
    else
    {
        std::cerr << "Error importing plan, 'boundary': Unrecognized word '" << subfield << "' in third column." << std::endl;     
    }
}

void plDonorSite::draw() const
{      
    if (!isVisible)
        return;
      
    // draw spline boundary 
    _plPickingState->type = PL_PICKING_TYPE_DONOR_BOUNDARY;
    boundary.draw();   
   
}











