#include "plCSV.h"

plCSVRow::plCSVRow()
{
}

plCSVRow::plCSVRow(const std::vector<plString>& cols)
    : _cols(cols)
{
}

plString plCSVRow::getCol(uint32_t index) const
{
    if (index > _cols.size()) {
        LOG_WARN("CSV column index " << index << " does not exist");
        return "";
    }
    return _cols[index];
}

std::vector<plString> plCSVRow::getCols() const
{
    return _cols;
}

plCSV::plCSV()
    : _good(false)
    , _rowIndex(0)
{
}

plCSV::plCSV(const plString& filename)
    : _good(false)
    , _rowIndex(0)
{
    _filename = filename;
    _readFile(filename);
}

bool plCSV::good() const
{
    return _good;
}

bool plCSV::eof() const
{
    return _rowIndex >= _rows.size();
}

plCSVRow plCSV::getRow()
{
    if (_rowIndex > _rows.size()) {
        LOG_WARN("CSV has no more rows");
        return plCSVRow();
    }
    return _rows[_rowIndex++];
}

void plCSV::_readFile(const plString& filename)
{
    std::ifstream infile(filename.c_str());

    // make sure import file opens correctly
    if (!infile.good()) {
        _good = false;
        return;
    }

    // parse each line
    while (!infile.eof()) {
        std::vector<plString> row;
        plString line, col;

        std::getline(infile, line);
        std::stringstream lineStream(line);

        // parse each comma seperated value
        while (std::getline(lineStream, col, ',')) {
            col.stripCharacter('\r'); // remove any carrage returns

            if (!col.isOnlyWhitespace()) // ignore any lines consisting of only whitespace
            {
                col.stripPreceedingWhitespace();
                row.push_back(col);
            }
        }

        if (row.size() > 0) // ignore any empty rows
        {
            _rows.push_back(plCSVRow(row));
        }
    }

    _good = true;
}
