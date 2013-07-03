#include "plIGuide.h" 

void plIGuide::readFromCSV( const plSeq<plString> &row )
{
    // Fill in the field
    plString subfield = row[2];
    
    if (plStringCompareCaseInsensitive(subfield, "boundary") )
    {                    
        for (PLuint j=3; j < row.size(); j+=2)
        {
            boundary.loadPointAndNormal( row[j], row[j+1] );
        }       
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
    boundary.draw();
}
