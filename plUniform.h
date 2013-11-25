#ifndef PL_UNIFORM_H
#define PL_UNIFORM_H

#include "plCommon.h"


class plUniform
{
    public:
    
        template< typename T >
        plUniform( PLuint type, const T& t );
            
        plUniform( plUniform&& uniform );
        plUniform( const plUniform& uniform );
        
        plUniform& operator= ( plUniform&& uniform );
        plUniform& operator= ( const plUniform& uniform );
        
        ~plUniform();
    
        GLvoid* data()     const { return (GLvoid*)(_data); }
        PLuint  type()     const { return _type;            }
        PLuint  numBytes() const { return _numBytes;        }
    
    private:

        PLuint  _type;
        PLuint  _numBytes;
        PLchar* _data;        

        void _copy( const plUniform& uniform );
        void _move( plUniform&& uniform );

};


template< typename T >
plUniform::plUniform( PLuint type, const T& t )
    :   _numBytes( sizeof( T ) ),
        _type( type )
{
    _numBytes = sizeof( T );
    _data = new PLchar[ _numBytes ];   
    memcpy( _data, reinterpret_cast<const PLchar*>( &t ), _numBytes );  
}


#endif 
