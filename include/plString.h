#pragma once

#include "plCommon.h"

class plString : public std::string // technically unsafe as std::string does not have a virtual destructor
{
    public:

        plString();
        plString(const std::string& str);
        plString(const char* s);

        bool importFile(const std::string &filename);

        bool compare(const plString &str) const;
        bool compare(const plString &str, uint32_t num) const;
        bool compare(const plString &str, uint32_t index, uint32_t num) const;

        bool compareCaseInsensitive(const plString &str2, uint32_t num) const;
        bool compareCaseInsensitive(const plString &str2) const;
        bool isOnlyWhitespace() const;
        plString withoutFilepath() const;

        void stripCharacter(char c);
        void stripPreceedingWhitespace();
        void toLower();
};
