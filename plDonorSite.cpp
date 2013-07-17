#include "plDonorSite.h" 

plDonorSite::plDonorSite() 
{
}

plDonorSite::plDonorSite( PLuint modelID, const plBoneAndCartilage &model )
    : plModelSpecific( modelID, model ), boundary( model )
{
}

void plDonorSite::importCSV(const plSeq<plString> &row, const plSeq<plBoneAndCartilage*> &models )
{
    plString subfield = row[2];
            
    if (subfield.compareCaseInsensitive( "model") )
    {
        _modelID = atof( row[3].c_str() );
        if (models.size() < (_modelID +1) )
        {
            std::cerr << "plDonorSite importCSV() error: model ID read before model data";
            exit(1);
        }
        _model = models[_modelID];
    }                   
    else if (subfield.compareCaseInsensitive( "boundary") )   
    {       
        if (_model == NULL)
        {
            std::cerr << "plDonorSite importCSV() error: boundary data read before model ID";
            exit(1);
        }         
        boundary.importCSV( row, *_model );  
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











