#pragma once

#include "plCommon.h"

#include "plString.h"

class plCSV
{
    public:

        std::vector<std::vector<plString>> data;
        plString filename;

        plCSV(plString fn);

        bool good() const;

    private:

        bool _good;

        void _readFile(plString filename, bool verbose = false);

};
