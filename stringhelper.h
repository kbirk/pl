#ifndef __STRING_HELPER_H__
#define __STRING_HELPER_H__

#include <string>
#include <algorithm>
#include <iostream>
#include <cctype>

bool compareCaseInsensitive(std::string str1, std::string str2, unsigned int num);
bool compareCaseInsensitive(std::string str1, std::string str2);
void stripQuotations(std::string &s);
bool onlyWhitespace(std::string s);

#endif
