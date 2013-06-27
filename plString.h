#ifndef __PL_STRING_HELPER_H__
#define __PL_STRING_HELPER_H__

#include "pl.h"

bool plStringCompareCaseInsensitive( plString str1, plString str2, unsigned int num);
bool plStringCompareCaseInsensitive( plString str1, plString str2);
void plStringStripQuotations       ( plString &s);
bool plStringOnlyWhitespace        ( plString s);

#endif
