#ifndef PL_UTILITY_H
#define PL_UTILITY_H

#include "plCommon.h"
#include "plSeq.h"

namespace plUtility
{

    template <class T>
    void swap( T &a, T &b )
    {
        T temp = a;
        a = b; 
        b = temp;
    }

    template <class T>
    void shuffle( plSeq<T> &array )
    {   
        PLuint size = array.size();
        for (PLuint i = 0; i < size-1; i++) 
        {
            PLuint j = i + rand() / (RAND_MAX / (size - i) + 1);
            T t = array[j];
            array[j] = array[i];
            array[i] = t;
        }    
    }

};

#endif




