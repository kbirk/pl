#pragma once

#include "plCommon.h"

namespace plUtility
{

    template <typename T>
    void swap(T &a, T &b)
    {
        T temp = a;
        a = b;
        b = temp;
    }

    template <typename T>
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

    template<typename T1, typename T2>
    bool removeIfExists(std::vector<T1> &vector, const T2 &t)
    {
        auto itr = std::find(vector.begin(), vector.end(), t);
        if (itr != vector.end())
        {
            vector.erase(itr);
            return true;
        }
        return false;
    }


    template<typename T1, typename T2>
    bool exists(const std::vector<T1> &vector, const T2 &t)
    {
        return std::find(vector.begin(), vector.end(), t) != vector.end();
    }


    void printProgressBar(float32_t percent, uint32_t length = 50);

}
