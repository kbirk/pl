#pragma once

#include "plCommon.h"

#include <epoxy/gl.h>

class plUniform
{
    public:

        template<typename T>
        plUniform(uint32_t type, const T& t);

        const uint8_t* data() const { return &_data[0]; }
        uint32_t type() const { return _type; }
        uint32_t numBytes() const { return _numBytes; }

    private:

        uint32_t _type;
        uint32_t _numBytes;
        std::vector<uint8_t> _data;
};


template<typename T>
plUniform::plUniform(uint32_t type, const T& t)
    : _type(type),
      _numBytes(sizeof(T))
{
    _numBytes = sizeof(T);
    _data = std::vector<uint8_t>(_numBytes, 0);
    memcpy(&_data[0], reinterpret_cast<const uint8_t*>(&t), _numBytes);
}
