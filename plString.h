#ifndef __PL_STRING_HELPER_H__
#define __PL_STRING_HELPER_H__

#include "pl.h"

bool plStringCompareCaseInsensitive( const plString &str1, const plString &str2, unsigned int num);
bool plStringCompareCaseInsensitive( const plString &str1, const plString &str2);
void plStringStripQuotations       ( plString &s);
bool plStringOnlyWhitespace        ( const plString &s);

#endif
