#include "plUtility.h"

namespace plUtility
{
    void printProgressBar(float percent, PLuint length)
    {
        std::ios::fmtflags flags(std::cout.flags());
        PLuint prevPrecision = std::cout.precision();
        std::cout << std::fixed << std::setprecision(2);

        std::cout << "\r\tProgress: [";

        for (PLuint i=0; i < PLuint(percent*length); i++)
            std::cout << "=";

        for (PLuint i=0; i < length - PLuint(percent*length); i++)
            std::cout << " ";

        std::cout << "]  " << percent*100.0f << "%";
        std::cout.flush();

        std::cout.flags(flags);
        std::cout.precision(prevPrecision);
    }
}
