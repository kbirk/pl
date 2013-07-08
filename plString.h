#ifndef __PL_STRING_HELPER_H__
#define __PL_STRING_HELPER_H__

#include "pl.h"

bool plStringCompareCaseInsensitive     ( const plString &str1, const plString &str2, PLuint num);
bool plStringCompareCaseInsensitive     ( const plString &str1, const plString &str2);
void plStringStripQuotations            ( plString &s);
void plStringStripPreceedingWhitespace  ( plString &s);
void plStringStripPreceedingFilepath    ( plString &s);
bool plStringOnlyWhitespace             ( const plString &s);

plString plStringConcat( const plString &s1, const plString &s2);
plString plStringConcat( const plString &s1, const plString &s2,  const plString &s3);

template <class T>
plString plToString( T t )
{
    std::ostringstream ss;
    ss << t;
    return ss.str();
}

template <class T>
T plFromString( const plString &str )
{
    T val;
    std::istringstream ss(str);
    ss >> val;
    return val;
}

#endif
