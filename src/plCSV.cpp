#include "plCSV.h"

plCSV::plCSV(plString fn)
{
    filename = fn;
    _readFile(fn);
}


void plCSV::_readFile(plString filename, PLbool verbose)
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
        std::vector<plString> lineData;
        plString line, entry;

        std::getline(infile, line);
        std::stringstream lineStream(line);

        // parse each comma seperated value
        while(std::getline(lineStream, entry, ','))
        {
            entry.stripCharacter('\r');        // remove any carrage returns

            if (!entry.isOnlyWhitespace())     // ignore any lines consisting of only whitespace
            {
                entry.stripPreceedingWhitespace();
                lineData.push_back(entry);
            }
        }

        if (lineData.size() > 0)                    // ignore any empty rows
        {
            data.push_back(lineData);
        }
    }

    if (verbose)
    {
        for (PLuint i = 0; i < data.size(); i++)
        {
            for (PLuint j = 0; j < data[i].size(); j++)
            {
                std::cout << data[i][j] << ", ";
            }
            std::cout << "\n";
        }
    }

}
