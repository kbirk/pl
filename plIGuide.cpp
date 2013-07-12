#include "plIGuide.h" 

void plIGuide::readFromCSV( const plSeq<plString> &row, const plSeq<plBoneAndCartilage> &models )
{
    // Fill in the field
    plString subfield = row[2];
    
    if (subfield.compareCaseInsensitive( "model") )
    {
        _modelID = atof( row[3].c_str() );
        if (models.size() < (_modelID +1) )
        {
            std::cerr << "plDefectSite readFromCSV() error: model ID read before model data";
            exit(1);
        }
        _model = &models[_modelID];
    }                   
    else if (subfield.compareCaseInsensitive( "boundary") )
    {      
        if (_model == NULL)
        {
            std::cerr << "plDefectSite readFromCSV() error: spline data read before model ID";
            exit(1);
        }                 
        boundary.readFromCSV( row, *_model );        
    } 
    else if (subfield.compareCaseInsensitive( "graft indices") ) 
    {
        for (PLuint j=3; j < row.size(); j++)
        {
            graftIndices.add( atoi( row[j].c_str() ) );
        }  
    } 
    else
    {
        std::cerr << "Error importing plan,'iguide': Unrecognized word '" << subfield << "' in third column." << std::endl;
    }
}

void plIGuide::draw() 
{
    plPicking::value.type = PL_PICKING_TYPE_IGUIDE_BOUNDARY; 
    boundary.draw();
}
