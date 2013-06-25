#include "plCSV.h"

plCSV::plCSV( plString fn ) 
{           
    filename = fn;
    readFile( fn );
}


void plCSV::readFile( plString filename, PLbool verbose )
{
    std::ifstream infile(filename);

    // make sure import file opens correctly
    if (!infile.good())
    {
        std::cerr << "Could not open '" << filename << "'." << std::endl;
        return;
    }
 
    // parse each line
    while (!infile.eof()) 
    {        
        plSeq<plString> lineData;
        plString line, entry;
        
        std::getline(infile, line);    
        std::stringstream lineStream(line);
        
        // parse each comma seperated value
        while(std::getline(lineStream, entry,','))
        {
            stripQuotations(entry);         // remove any quotation marks
            
            if (!onlyWhitespace(entry))     // ignore any lines consisting of only whitespace
            {
                lineData.add(entry);
            }
        }  
        if (lineData.size() > 0)            // ignore any empty rows
        {
            data.add(lineData);
        }
    }
     
    if (verbose)
    {           
        for (unsigned int i = 0; i < data.size(); i++)
        {
            for (unsigned int j = 0; j < data[i].size(); j++)
            {
                std::cout << data[i][j] << ", ";
            }
            std::cout << "\n";
        }
    }

}

