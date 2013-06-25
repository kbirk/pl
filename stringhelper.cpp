#include "stringhelper.h"

bool compareCaseInsensitive(std::string str1, std::string str2, unsigned int num)
{
    // Convert both strings to upper case by transfrom() before compare.
    if (num > str1.length() || num > str2.length())
    {
        return false;
    }
    
    for (unsigned int i = 0; i < num; i++)
    {
        if (tolower(str1[i]) != tolower(str2[i]))
        {
            return false;
        }
    }
    
    return true; 
}

bool compareCaseInsensitive(std::string str1, std::string str2)
{
    if (str1.length() != str2.length())
    {
        return false;
    }

    for (unsigned int i = 0; i < str1.length(); i++)
    {
        if (tolower(str1[i]) != tolower(str2[i]))
        {
            return false;
        }
    }
    
    return true;   
}

void stripQuotations(std::string &s)
{
    s.erase( std::remove( s.begin(), s.end(), '\"'), s.end() ); 
}  

bool onlyWhitespace(std::string s)
{
    return (s.length() == 0) || (s.find_first_not_of("\t\n\r ") == std::string::npos);
} 

