#include "plUniform.h"


plUniform::plUniform( plUniform&& uniform )
{
    _move( std::move( uniform ) );
}


plUniform::plUniform( const plUniform& uniform )
{
    _copy( uniform );
}


plUniform& plUniform::operator= ( plUniform&& uniform )
{
    _move( std::move( uniform ) );
    return *this;
}


plUniform& plUniform::operator= ( const plUniform& uniform )
{
    _copy( uniform );
    return *this;
}


plUniform::~plUniform()
{
    delete [] _data;
}


void plUniform::_copy( const plUniform& uniform )
{
    _numBytes = uniform._numBytes;
    _type = uniform._type;
    _data = new PLchar[ _numBytes ];
    memcpy( _data, uniform._data, _numBytes );     
}


void plUniform::_move( plUniform&& uniform )
{
    _numBytes = uniform._numBytes;
    _type = uniform._type;
    _data = uniform._data;
    
    uniform._numBytes = 0;
    uniform._type = 0;
    uniform._data = 0;
}


