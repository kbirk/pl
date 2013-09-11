#include "plDonorSite.h" 

plDonorSite::plDonorSite() 
{
}


plDonorSite::plDonorSite( PLuint modelID, const plBoneAndCartilage &model )
    : plModelSpecific( modelID, model )
{
}


plDonorSite::plDonorSite( PLuint modelID, const plBoneAndCartilage &model, const plBoundary &b  )
    : plModelSpecific( modelID, model ), boundary( b )
{
}


/*
void plDonorSite::importCSV(const plSeq<plString> &row, const plSeq<plBoneAndCartilage*> &models )
{
    plString subfield = row[2];
            
    if (subfield.compareCaseInsensitive( "model") )
    {
        _modelID = plString::fromString<PLint>( row[3] ); 
        if (models.size() <= _modelID )
        {
            std::cerr << "plDonorSite importCSV() error: model ID read before model data";
            exit(1);
        }
        _model = models[_modelID];
    }                   
    else if (subfield.compareCaseInsensitive( "boundary") )   
    {           
        boundary.importCSV( row );  
    } 
    else
    {
        std::cerr << "Error importing plan, 'boundary': Unrecognized word '" << subfield << "' in third column." << std::endl;     
    }
}
*/


void plDonorSite::draw() const
{      
    if ( !_isVisible )
        return;
      
    // draw spline boundary 
    plPicking::value.type = PL_PICKING_TYPE_DONOR_BOUNDARY;
    boundary.draw();   
   
}











