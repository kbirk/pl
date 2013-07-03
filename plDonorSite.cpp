#include "plDonorSite.h" 

plDonorSite::plDonorSite() 
{
}

void plDonorSite::init()
{
    boundary.updateMesh();
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
        for ( PLuint j = 3; j < row.size(); j+=2)
        {      
            boundary.loadPointAndNormal( row[j], row[j+1] );                         
        }
    } 
    else
    {
        std::cerr << "Error importing plan, 'boundary': Unrecognized word '" << subfield << "' in third column." << std::endl;     
    }
}

void plDonorSite::draw() const
{      
    if (!_isVisible)
        return;
      
    // draw spline boundary 
    _plPickingState->type = PL_PICKING_TYPE_DONOR_BOUNDARY;
    boundary.draw();   
   
}











