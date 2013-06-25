#ifndef __PL_STRING_HELPER_H__
#define __PL_STRING_HELPER_H__

#include "pl.h"

bool plCompareCaseInsensitive( plString str1, plString str2, unsigned int num);
bool plCompareCaseInsensitive( plString str1, plString str2);
void plStripQuotations       ( plString &s);
bool plOnlyWhitespace        ( plString s);

#endif
