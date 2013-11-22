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


/*
class plUniformBase
{
    public:

        plUniformBase() 
            :   _type( -1 )
        {}
        
        virtual ~plUniformBase() 
        {}
        
        virtual GLvoid* data() const = 0;
        virtual PLuint  type() const { return _type; }

    protected:
    
        PLint _type;    
        
};


template< typename T >
class plUniform : public plUniformBase
{
    public:

        plUniform( PLuint type, const T& data );
        plUniform( const plUniform& uniform );

        GLvoid* data() const;     
        
    private:    
        
        T _data;
};


template< typename T >
plUniform<T>::plUniform( const plUniform<T>& uniform )
    :   _data( uniform._data )  
{
    _type = uniform._type;
} 
  

template< typename T >
plUniform<T>::plUniform( PLuint type, const T& data )
    :   _data( data )
{
    _type = type;
} 
  
        
template< typename T >
GLvoid* plUniform<T>::data() const 
{ 
    return ( GLvoid* )( &_data ); 
} 
*/

#endif 
