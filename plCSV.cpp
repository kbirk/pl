#include "plCSV.h"

plCSV::plCSV( plString fn ) 
{           
    filename = fn;
    readFile( fn );
}


void plCSV::readFile( plString filename, PLbool verbose )
{
    std::ifstream infile(filename.c_str());

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
            plStripQuotations(entry);         // remove any quotation marks
            
            if (!plOnlyWhitespace(entry))     // ignore any lines consisting of only whitespace
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
        for (PLint i = 0; i < data.size(); i++)
        {
            for (PLint j = 0; j < data[i].size(); j++)
            {
                std::cout << data[i][j] << ", ";
            }
            std::cout << "\n";
        }
    }

}

