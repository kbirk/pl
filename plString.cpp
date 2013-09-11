#include "plString.h"

plString::plString() 
    : std::string()
{

}


plString::plString (const std::string &str)
    : std::string( str ) 
{

}


plString::plString (const char* s)
    : std::string( s ) 
{

}


bool plString::importFile( const std::string &filename)      
{
    std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        this->resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&(*this)[0], this->size());
        in.close();
    }  
    return true;
}  
  

void plString::toLower()
{
    for (PLuint i = 0; i < length(); i++)
    {
        (*this)[i] = tolower((*this)[i]);
    }
}


bool plString::compare( const plString &str ) const
{
    if (length() != str.length())
    {
        return false;
    }

    for (PLuint i = 0; i < length(); i++)
    {
        if ( (*this)[i] != str[i] )
        {
            return false;
        }
    }
    
    return true;  
}


bool plString::compare( const plString &str, PLuint num  ) const
{
    if (num > this->length() || num > str.length())
    {
        return false;
    }
    
    for (PLuint i = 0; i < num; i++)
    {
        if ( (*this)[i] != str[i] )
        {
            return false;
        }
    }
    
    return true; 
}


bool plString::compare( const plString &str, PLuint index, PLuint num ) const
{
    if ( (index+num) > this->length() || num > str.length() )
    {
        return false;
    }
    
    for (PLuint i = 0; i < num; i++)
    {
        if ( (*this)[i+index] != str[i] )
        {
            return false;
        }
    }
    
    return true; 

} 

bool plString::compareCaseInsensitive( const plString &str, PLuint num) const
{
    if (num > this->length() || num > str.length())
    {
        return false;
    }
    
    for (PLuint i = 0; i < num; i++)
    {
        if (tolower((*this)[i]) != tolower(str[i]))
        {
            return false;
        }
    }
    
    return true; 
}


bool plString::compareCaseInsensitive( const plString &str) const
{
    if (length() != str.length())
    {
        return false;
    }

    for (PLuint i = 0; i < length(); i++)
    {
        if (tolower((*this)[i]) != tolower(str[i]))
        {
            return false;
        }
    }
    
    return true;   
}

/*
plString plString::operator + ( const plString &s ) const
{
    std::stringstream str;
    str << *this << s << "\0";
    return str.str();
}
*/

void plString::stripCharacter( char c)
{
    erase( std::remove( begin(), end(), c), end() ); 
}  


void plString::stripPreceedingWhitespace()
{
    size_t startpos = find_first_not_of(" \t");
    if (startpos < length())
        *this = substr( startpos );
}  


plString plString::withoutFilepath() const
{
    size_t startpos = find_last_of("/") + 1;
    if (startpos < length())
        return substr( startpos );
    else
        return *this;    
}


bool plString::isOnlyWhitespace() const
{
    return (length() == 0) || (find_first_not_of("\t\n\r ") == plString::npos);
} 





