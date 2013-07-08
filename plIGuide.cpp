#include "plIGuide.h" 

void plIGuide::readFromCSV( const plSeq<plString> &row )
{
    // Fill in the field
    plString subfield = row[2];
    
    if (plStringCompareCaseInsensitive(subfield, "boundary") )
    {                    
        boundary.readFromCSV( row );        
    } 
    else if (plStringCompareCaseInsensitive(subfield, "graft indices") ) 
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
    _plPickingState->type = PL_PICKING_TYPE_IGUIDE_BOUNDARY; 
    boundary.draw();
}
