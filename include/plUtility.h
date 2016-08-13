#pragma once

#include "plCommon.h"

namespace plUtility
{

    template <class T>
    void swap(T &a, T &b)
    {
        T temp = a;
        a = b;
        b = temp;
    }

    template <class T>
    void shuffle(std::vector<T> &array)
    {
        uint32_t size = array.size();
        for (uint32_t i = 0; i < size-1; i++)
        {
            uint32_t j = i + rand() / (RAND_MAX / (size - i) + 1);
            T t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }

    template< class Type1, class Type2 >
    bool removeIfExists(std::vector<Type1 > &vector, const Type2 &t)
    {
        auto itr = std::find(vector.begin(), vector.end(), t);
        if (itr != vector.end())
        {
            vector.erase(itr);
            return true;
        }
        return false;
    }


    template< class Type1, class Type2 >
    bool exists(const std::vector<Type1> &vector, const Type2 &t)
    {
        return std::find(vector.begin(), vector.end(), t) != vector.end();
    }


    void printProgressBar(float32_t percent, uint32_t length = 50);

}
