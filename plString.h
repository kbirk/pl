#ifndef __PL_STRING_H__
#define __PL_STRING_H__

#include "plCommon.h"

class plString : public std::string
{
    public:
    
        plString();
        plString(const std::string& str);
        plString(const char* s);
          
        bool importFile( const std::string &filename );          
           
        bool compare( const plString &str ) const;
        bool compare( const plString &str, PLuint num ) const;
        bool compare( const plString &str, PLuint index, PLuint num ) const;       
                     
        bool compareCaseInsensitive ( const plString &str2, PLuint num) const;
        bool compareCaseInsensitive ( const plString &str2) const;               
        bool isOnlyWhitespace       () const;        
        plString withoutFilepath() const;
        
        void stripCharacter             ( char c);
        void stripPreceedingWhitespace  ();
        void toLower();      
};


#endif
