#ifndef PL_SEQ_H
#define PL_SEQ_H

#include "plCommon.h"

#define PL_SEQ_INITIAL_SIZE 8

/*
template<class T> 
class std::vector 
{
	public:

		std::vector();
		std::vector( PLuint n );
		std::vector( PLuint n, const T &t );
		std::vector( const std::vector<T> &source );
		
		~std::vector();
	
		void push_back( const T& x );
		void push_back( T&& x );
		void push_back( const std::vector<T> &s );	
		void fill( PLuint n, const T &t );	
		void remove();
        void remove( PLuint i );
		void shift( PLuint i );
		void compress();
		void clear(); 
		void reserve(PLuint size);
        void resize(PLuint size);
		std::vector<T> extract( PLuint index, PLuint num ) const;
		PLuint size() const; 

        T & back();
		T & operator [] ( PLuint i ) const; 
		std::vector<T> &operator = (const std::vector<T> &source); 

		PLint findIndex( const T &x );
        bool exists( const T &x );

    private:
    
        PLuint    _storageSize;
	    PLuint    _numElements;
	    T  		 *_data;

};


template<class T>
std::vector<T>::std::vector() 
{			
	_storageSize = PL_SEQ_INITIAL_SIZE;
	_numElements = 0;
	_data = new T[ _storageSize ];
}


template<class T>
std::vector<T>::std::vector( PLuint n ) 
{		
	_storageSize = n;
	_numElements = 0;
	_data = new T[ _storageSize ];
}


template<class T>
std::vector<T>::std::vector( PLuint n, const T &t )
{
	_data = NULL; // fill() calls delete, will seq fault if this isn't set to NULL
	fill( n, t );	
}


template<class T>
std::vector<T>::std::vector( const std::vector<T> &source ) 
{ 
	_storageSize = source._storageSize;
	_numElements = source._numElements;
	_data = new T[ _storageSize ];
	
	for (PLuint i=0; i<_numElements; i++)
	{
		_data[i] = source._data[i];
	}		
}


template<class T>
std::vector<T>::~std::vector() 
{		
	delete [] _data;
}


template<class T>
void std::vector<T>::push_back( const T &x )
{
    // No storage left?  If so, double the storage
	if (_numElements == _storageSize) 
	{
	    reserve(_storageSize*2);
	}
	// Store the element in the next available position
	_data[ _numElements ] = x;
	_numElements++;
}


template<class T>
void std::vector<T>::push_back( T&& x )
{
    // No storage left?  If so, double the storage
	if ( _numElements == _storageSize ) 
	{
	    reserve( _storageSize*2 );
	}
	// Store the element in the next available position
	_data[ _numElements ] = std::forward<T>( x );
	_numElements++;

}


template<class T>
void std::vector<T>::push_back( const std::vector<T> &s )
{
    // No storage left?  If so, double the storage
	if (_numElements + s.size() >= _storageSize) 
	{
        reserve( _storageSize*2 + s.size() );	        
	}
	// Store the element in the next available position
	
	for (PLuint i=0; i<s.size(); i++)
	{
	    _data[ _numElements+i ] = s[i];	
	}
	_numElements+=s.size();	
}


template<class T>
void std::vector<T>::fill( PLuint n, const T &t )
{
    delete [] _data;
    _storageSize = n;
	_numElements = n;
	_data = new T[ _storageSize ];
	for (PLuint i=0; i<_numElements; i++)
	{
		_data[i] = t;
	}	
}

template<class T>	
void std::vector<T>::remove() 
{
	if (_numElements == 0) 
	{
		std::cerr << "std::vector remove: Tried to remove element from empty std::vectoruence\n";
	}
	_numElements -= 1;
}


// Shift a suffix of the std::vectoruence to the left by one
template<class T>
void std::vector<T>::remove( PLuint i )
{
	if (i < 0 || i >= _numElements) 
	{
		std::cerr << "std::vector remove: Tried to remove element " << i
				  << " from a std::vectoruence of " << _numElements << " elements \n";
	}

	for (PLuint j=i; j<_numElements-1; j++)
		_data[j] = _data[j+1];

	_numElements--;
}


// Shift a suffix of the std::vectoruence to the right by one
template<class T>
void std::vector<T>::shift( PLuint i )
{
	if (i < 0 || i >= _numElements) 
	{
		std::cerr << "std::vector shift: Tried to shift element " << i
				  << " from a std::vectoruence of " << _numElements << " elements \n";
	}

	if (_numElements == _storageSize) 
	{
        reserve(_storageSize*2);
	}

	for (PLuint j=_numElements; j>i; j--)
    {
		_data[j] = _data[j-1];
    }

	_numElements++;
}


// Compress the array
template<class T>
void std::vector<T>::compress()
{
	T *new_data;

	if (_numElements == _storageSize)
		return;

	new_data = new T[ _numElements ];
	for (PLint i=0; i<_numElements; i++)
	{
		new_data[i] = _data[i];
    }
	_storageSize = _numElements;
	delete [] _data;
	_data = new_data;
}


template<class T>
void std::vector<T>::clear() 
{
	delete [] _data;
	_storageSize = PL_SEQ_INITIAL_SIZE;
	_numElements = 0;
	_data = new T[ _storageSize ];
}


template<class T>
void std::vector<T>::reserve(PLuint size)
{
    if (size < _storageSize)
        return;
        
    T *new_data = new T[ size ];
    
	for (PLuint i=0; i<_numElements; i++)
	{
		new_data[i] = std::move( _data[i] );
    }
    
	_storageSize = size;
	delete [] _data;
	_data = new_data;
}


template<class T>
void std::vector<T>::resize( PLuint size )
{
    if (size < _numElements)
        return;

    fill( size, T() );
}

template<class T>
std::vector<T> std::vector<T>::extract( PLuint index, PLuint num ) const
{
    std::vector<T> ts( num );
    
    for (PLuint i=index; i < index+num; i++)
    {
        ts.push_back( _data[i] );
    }
    
    return ts;
}


template<class T>
PLuint std::vector<T>::size() const 
{
	return _numElements;
}


template<class T>
T & std::vector<T>::back()
{
    if (!(_numElements > 0)) 
	{
		std::cerr << "std::vector .back() error: zero elements in sequence\n";
	}
	return _data[ _numElements-1 ];
}


template<class T>
T & std::vector<T>::operator [] ( PLuint i ) const 
{
	if (i >= _numElements || i < 0) 
	{
		std::cerr << "std::vector [] error: Tried to access an element beyond the range of the std::vectoruence: "
				  << i << "(_numElements = " << _numElements << ")\n";
	}
	return _data[ i ];
}


template<class T>
std::vector<T> &std::vector<T>::operator = (const std::vector<T> &source) 
{ 
	_storageSize = source._storageSize;
	_numElements = source._numElements;
	delete [] _data;
	_data = new T[ _storageSize ];
		
	for (PLuint i=0; i<_numElements; i++)
	{
		_data[i] = source._data[i];
	}
	
	return *this;
}


// Find and return an element
template<class T>
PLbool std::vector<T>::exists( const T &x )
{
	for (PLint i=0; i<_numElements; i++)
	{
		if (_data[i] == x)
		{
		    return true;
		}
    }
	return false;
}



// Find and return index of the first instance of an element
// if it doesn't exist, return -1
template<class T>
PLint std::vector<T>::findIndex( const T &x )
{
	for (PLuint i=0; i<_numElements; i++)
		if (_data[i] == x)
			return i;

	return -1;
}
*/

#endif
