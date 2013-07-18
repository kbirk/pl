#ifndef __PL_SEQ_H__
#define __PL_SEQ_H__

#include "plCommon.h"

#define PL_SEQ_INITIAL_SIZE 8

template<class T> 
class plSeq 
{
	public:

		plSeq();
		plSeq( PLuint n );
		plSeq( const T &t, PLuint n);
		plSeq( const plSeq<T> &source );
		
		~plSeq();
	
		void add( const T &x );
		void add( const plSeq<T> &s );		
		void remove();
		void remove( PLuint i );
		void shift( PLuint i );
		void compress();
		void clear(); 
		void reserve(PLuint size);
		PLuint size() const; 

        T & back();
		T & operator [] ( PLuint i ) const; 
		plSeq<T> &operator = (const plSeq<T> &source); 

		PLint findIndex( const T &x );
        bool exists( const T &x );

    private:
    
        PLuint    _storageSize;
	    PLuint    _numElements;
	    T  		 *_data;

};


template<class T>
plSeq<T>::plSeq() 
{			
	_storageSize = PL_SEQ_INITIAL_SIZE;
	_numElements = 0;
	_data = new T[ _storageSize ];
}


template<class T>
plSeq<T>::plSeq( PLuint n ) 
{		
	_storageSize = n;
	_numElements = 0;
	_data = new T[ _storageSize ];
}


template<class T>
plSeq<T>::plSeq( const T &t, PLuint n)
{
    _storageSize = n;
	_numElements = n;
	_data = new T[ _storageSize ];
	for (PLuint i=0; i<_numElements; i++)
	{
		_data[i] = t;
	}	
}


template<class T>
plSeq<T>::plSeq( const plSeq<T> &source ) 
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
plSeq<T>::~plSeq() 
{		
	delete [] _data;
}


template<class T>
void plSeq<T>::add( const T &x )
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
void plSeq<T>::add( const plSeq<T> &s )
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
void plSeq<T>::remove() 
{
	if (_numElements == 0) 
	{
		std::cerr << "plSeq remove: Tried to remove element from empty plSequence\n";
	}
	_numElements -= 1;
}


// Shift a suffix of the plSequence to the left by one
template<class T>
void plSeq<T>::remove( PLuint i )
{
	if (i < 0 || i >= _numElements) 
	{
		std::cerr << "plSeq remove: Tried to remove element " << i
				  << " from a plSequence of " << _numElements << " elements \n";
	}

	for (PLuint j=i; j<_numElements-1; j++)
		_data[j] = _data[j+1];

	_numElements--;
}


// Shift a suffix of the plSequence to the right by one
template<class T>
void plSeq<T>::shift( PLuint i )
{
	if (i < 0 || i >= _numElements) 
	{
		std::cerr << "plSeq shift: Tried to shift element " << i
				  << " from a plSequence of " << _numElements << " elements \n";
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
void plSeq<T>::compress()
{
	T *new_data;

	if (_numElements == _storageSize)
		return;

	new_data = new T[ _numElements ];
	for (PLint i=0; i<_numElements; i++)
		new_data[i] = _data[i];
	_storageSize = _numElements;
	delete [] _data;
	_data = new_data;
}


template<class T>
void plSeq<T>::clear() 
{
	delete [] _data;
	_storageSize = PL_SEQ_INITIAL_SIZE;
	_numElements = 0;
	_data = new T[ _storageSize ];
}


template<class T>
void plSeq<T>::reserve(PLuint size)
{
    if (size < _storageSize)
        return;
        
    T *new_data = new T[ size ];
    
	for (PLuint i=0; i<_numElements; i++)
	{
		new_data[i] = _data[i];
    }
	_storageSize = size;
	delete [] _data;
	_data = new_data;
}


template<class T>
PLuint plSeq<T>::size() const 
{
	return _numElements;
}


template<class T>
T & plSeq<T>::back()
{
    if (!(_numElements > 0)) 
	{
		std::cerr << "plSeq .back() error: zero elements in sequence\n";
	}
	return _data[ _numElements-1 ];
}


template<class T>
T & plSeq<T>::operator [] ( PLuint i ) const 
{
	if (i >= _numElements || i < 0) 
	{
		std::cerr << "plSeq [] error: Tried to access an element beyond the range of the plSequence: "
				  << i << "(_numElements = " << _numElements << ")\n";
	}
	return _data[ i ];
}


template<class T>
plSeq<T> &plSeq<T>::operator = (const plSeq<T> &source) 
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
PLbool plSeq<T>::exists( const T &x )
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
PLint plSeq<T>::findIndex( const T &x )
{
	for (PLint i=0; i<_numElements; i++)
		if (_data[i] == x)
			return i;

	return -1;
}


#endif
