#pragma once

#include "plCommon.h"


class plUniform
{
    public:

        template< typename T >
        plUniform(uint32_t type, const T& t);

        plUniform(plUniform&& uniform);
        plUniform(const plUniform& uniform);

        plUniform& operator= (plUniform&& uniform);
        plUniform& operator= (const plUniform& uniform);

        ~plUniform();

        GLvoid* data()const { return (GLvoid*)(_data); }
        uint32_t type() const { return _type; }
        uint32_t numBytes() const { return _numBytes; }

    private:

        uint32_t _type;
        uint32_t _numBytes;
        uint8_t* _data;

        void _copy(const plUniform& uniform);
        void _move(plUniform&& uniform);

};


template< typename T >
plUniform::plUniform(uint32_t type, const T& t)
    : _type(type),
      _numBytes(sizeof(T))
{
    _numBytes = sizeof(T);
    _data = new uint8_t[_numBytes];
    memcpy(_data, reinterpret_cast<const uint8_t*>(&t), _numBytes);
}
