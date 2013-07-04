#include "plString.h"

bool plStringCompareCaseInsensitive(const plString &str1, const plString &str2, PLuint num)
{
    // Convert both strings to upper case by transfrom() before compare.
    if (num > str1.length() || num > str2.length())
    {
        return false;
    }
    
    for (PLuint i = 0; i < num; i++)
    {
        if (tolower(str1[i]) != tolower(str2[i]))
        {
            return false;
        }
    }
    
    return true; 
}

bool plStringCompareCaseInsensitive(const plString &str1, const plString &str2)
{
    if (str1.length() != str2.length())
    {
        return false;
    }

    for (PLuint i = 0; i < str1.length(); i++)
    {
        if (tolower(str1[i]) != tolower(str2[i]))
        {
            return false;
        }
    }
    
    return true;   
}

void plStringStripCharacter( plString &s, char c)
{
    s.erase( std::remove( s.begin(), s.end(), c), s.end() ); 
}  

void plStringStripPreceedingWhitespace( plString &s)
{
    size_t startpos = s.find_first_not_of(" \t");
    if (startpos < s.length())
        s = s.substr( startpos );
}  

void plStringStripPreceedingFilepath( plString &s)
{
    size_t startpos = s.find_last_of("/") + 1;
    if (startpos < s.length())
        s = s.substr( startpos );
}

bool plStringOnlyWhitespace( const plString &s)
{
    return (s.length() == 0) || (s.find_first_not_of("\t\n\r ") == plString::npos);
} 

