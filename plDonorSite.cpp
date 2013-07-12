#include "plDonorSite.h" 

plDonorSite::plDonorSite() 
{
}

void plDonorSite::readFromCSV(const plSeq<plString> &row, const plSeq<plBoneAndCartilage> &models )
{
    plString subfield = row[2];
            
    if (subfield.compareCaseInsensitive( "model") )
    {
        _modelID = atof( row[3].c_str() );
        if (models.size() < (_modelID +1) )
        {
            std::cerr << "plDonorSite readFromCSV() error: model ID read before model data";
            exit(1);
        }
        model = &models[_modelID];
    }                   
    else if (subfield.compareCaseInsensitive( "boundary") )   
    {       
        if (model == NULL)
        {
            std::cerr << "plDonorSite readFromCSV() error: boundary data read before model ID";
            exit(1);
        }         
        boundary.readFromCSV( row, *model );  
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
    plPicking::value.type = PL_PICKING_TYPE_DONOR_BOUNDARY;
    boundary.draw();   
   
}











