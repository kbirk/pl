#include "plUtility.h"

namespace plUtility
{
    void printProgressBar(float32_t percent, uint32_t length)
    {
        std::ios::fmtflags flags(std::cout.flags());
        uint32_t prevPrecision = std::cout.precision();
        std::cout << std::fixed << std::setprecision(2);

        std::cout << "\r\tProgress: [";

        for (uint32_t i=0; i < uint32_t(percent*length); i++)
            std::cout << "=";

        for (uint32_t i=0; i < length - uint32_t(percent*length); i++)
            std::cout << " ";

        std::cout << "]  " << percent*100.0f << "%";
        std::cout.flush();

        std::cout.flags(flags);
        std::cout.precision(prevPrecision);
    }
}
