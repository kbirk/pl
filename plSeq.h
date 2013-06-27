#ifndef __PL_plSeq_H__
#define __PL_plSeq_H__

#include <iostream>
#include <algorithm>
#include "pl.h"

#define PL_SEQ_INITIAL_SIZE 8

template<class T> 
class plSeq 
{

	PLuint    storageSize;
	PLuint    numElements;
	T  		 *data;

	public:

		plSeq();
		plSeq( PLint n );
		plSeq( const plSeq<T> & source );
		
		~plSeq();
	
		void add( const T &x );
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

};


template<class T>
plSeq<T>::plSeq() 
{			
	storageSize = PL_SEQ_INITIAL_SIZE;
	numElements = 0;
	data = new T[ storageSize ];
}


template<class T>
plSeq<T>::plSeq( PLint n ) 
{		
	storageSize = n;
	numElements = 0;
	data = new T[ storageSize ];
}


template<class T>
plSeq<T>::plSeq( const plSeq<T> &source ) 
{ 
	storageSize = source.storageSize;
	numElements = source.numElements;
	data = new T[ storageSize ];
	
	for (PLint i=0; i<numElements; i++)
	{
		data[i] = source.data[i];
	}	
}


template<class T>
plSeq<T>::~plSeq() 
{		
	delete [] data;
}


template<class T>
void plSeq<T>::add( const T &x )
{
    // No storage left?  If so, double the storage
	if (numElements == storageSize) 
	{
	    reserve(storageSize*2);
	}
	// Store the element in the next available position
	data[ numElements ] = x;
	numElements++;
}

template<class T>	
void plSeq<T>::remove() 
{
	if (numElements == 0) 
	{
		std::cerr << "remove: Tried to remove element from empty plSequence\n";
	}
	numElements -= 1;
}


// Shift a suffix of the plSequence to the left by one
template<class T>
void plSeq<T>::remove( PLuint i )
{
	if (i < 0 || i >= numElements) 
	{
		std::cerr << "remove: Tried to remove element " << i
				  << " from a plSequence of " << numElements << " elements \n";
	}

	for (PLint j=i; j<numElements-1; j++)
		data[j] = data[j+1];

	numElements--;
}


// Shift a suffix of the plSequence to the right by one
template<class T>
void plSeq<T>::shift( PLuint i )
{
	if (i < 0 || i >= numElements) 
	{
		std::cerr << "remove: Tried to shift element " << i
				  << " from a plSequence of " << numElements << " elements \n";
	}

	if (numElements == storageSize) 
	{
        reserve(storageSize*2);
	}

	for (PLint j=numElements; j>i; j--)
		data[j] = data[j-1];

	numElements++;
}


// Compress the array
template<class T>
void plSeq<T>::compress()
{
	T *newData;

	if (numElements == storageSize)
		return;

	newData = new T[ numElements ];
	for (PLint i=0; i<numElements; i++)
		newData[i] = data[i];
	storageSize = numElements;
	delete [] data;
	data = newData;
}


template<class T>
void plSeq<T>::clear() 
{
	delete [] data;
	storageSize = PL_SEQ_INITIAL_SIZE;
	numElements = 0;
	data = new T[ storageSize ];
}


template<class T>
void plSeq<T>::reserve(PLuint size)
{
    if (size < storageSize)
    {
        std::cerr << "plSeq reserve: Attempting to reserve less than current size \n";
        return;
    }
    T *newData = new T[ size ];
    
	for (PLint i=0; i<numElements; i++)
	{
		newData[i] = data[i];
    }
	storageSize = size;
	delete [] data;
	data = newData;
}


template<class T>
PLuint plSeq<T>::size() const 
{
	return numElements;
}


template<class T>
T & plSeq<T>::back()
{
    if (!(numElements > 0)) 
	{
		std::cerr << "plSeq .back() error: zero elements in sequence\n";
	}
	return data[ numElements-1 ];
}


template<class T>
T & plSeq<T>::operator [] ( PLuint i ) const 
{
	if (i >= numElements || i < 0) 
	{
		std::cerr << "plSeq [] error: Tried to access an element beyond the range of the plSequence: "
				  << i << "(numElements = " << numElements << ")\n";
	}
	return data[ i ];
}


template<class T>
plSeq<T> &plSeq<T>::operator = (const plSeq<T> &source) 
{ 
	storageSize = source.storageSize;
	numElements = source.numElements;
	delete [] data;
	data = new T[ storageSize ];
	
	for (PLint i=0; i<numElements; i++)
	{
		data[i] = source.data[i];
	}
	
	return *this;
}


// Find and return an element
template<class T>
bool plSeq<T>::exists( const T &x )
{
	for (PLint i=0; i<numElements; i++)
	{
		if (data[i] == x)
		{
		    return true;
		}
    }
	return false;
}


// Find and return the *index* of an element
template<class T>
PLint plSeq<T>::findIndex( const T &x )
{
	for (PLint i=0; i<numElements; i++)
		if (data[i] == x)
			return i;

	return -1;
}


#endif
