#pragma once

#include "plCommon.h"
#include "plString.h"

class plCSVRow
{
    public:

        plCSVRow();
        plCSVRow(const std::vector<plString>& line);

        plString getCol(uint32_t) const;
        std::vector<plString> getCols() const;

    private:

        std::vector<plString> _cols;
};


class plCSV
{
    public:

        plCSV();
        plCSV(const plString& filename);

        plCSVRow getRow();

        bool good() const;
        bool eof() const;

    private:

        bool _good;
        uint32_t _rowIndex;
        std::vector<plCSVRow> _rows;
        plString _filename;

        void _readFile(const plString& filename);

};
