#ifndef __PL_STRING_H__
#define __PL_STRING_H__

#include "plCommon.h"

class plString : public std::string
{
    public:
    
        plString();
        plString(const std::string& str);
        plString(const char* s);
          
        bool importFile( const std::string &filename);          
           
        bool compare( const plString &str );
        bool compare( const plString &str, PLuint num );
        bool compare( const plString &str, PLuint index, PLuint num );       
                     
        bool compareCaseInsensitive   ( const plString &str2, PLuint num) const;
        bool compareCaseInsensitive   ( const plString &str2) const;               
        bool isOnlyWhitespace     () const;        
        plString withoutFilepath() const;
        
        void stripCharacter             ( char c);
        void stripPreceedingWhitespace  ();
        void toLower();
        
        plString operator + (const plString &s) const;  

        template <class T>
        static plString toString( const T &t );

        template <class T>
        static T fromString( const plString &str );
               
};


template <class T>
plString plString::toString( const T &t )
{
    std::ostringstream ss;
    ss << t;
    return ss.str();
}

template <class T>
T plString::fromString( const plString &str )
{
    T val;
    std::istringstream ss(str);
    ss >> val;
    return val;
}

#endif
