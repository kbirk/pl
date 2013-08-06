#include "plIGuideSite.h" 

plIGuideSite::plIGuideSite() 
{
}

plIGuideSite::plIGuideSite( PLuint modelID, const plBoneAndCartilage &model )
    : plModelSpecific( modelID, model ), boundary( model )
{
}

void plIGuideSite::importCSV( const plSeq<plString> &row, const plSeq<plBoneAndCartilage*> &models )
{
    // Fill in the field
    plString subfield = row[2];
    
    if (subfield.compareCaseInsensitive( "model") )
    {
        _modelID = plString::fromString<PLint>( row[3] ); 
        if (models.size() <= _modelID )
        {
            std::cerr << "plDefectSite importCSV() error: model ID read before model data";
            exit(1);
        }
        _model = models[_modelID];
    }                   
    else if (subfield.compareCaseInsensitive( "boundary") )
    {      
        if (_model == NULL)
        {
            std::cerr << "plDefectSite importCSV() error: spline data read before model ID";
            exit(1);
        }                 
        boundary.importCSV( row, *_model );        
    } 
    else
    {
        std::cerr << "Error importing plan,'iguide': Unrecognized word '" << subfield << "' in third column." << std::endl;
    }
}

void plIGuideSite::draw() 
{
    plPicking::value.type = PL_PICKING_TYPE_IGUIDE_BOUNDARY; 
    boundary.draw();
}
