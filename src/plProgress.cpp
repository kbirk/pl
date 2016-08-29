#include "plProgress.h"

namespace plProgress
{
    const uint32_t BAR_LENGTH = 50;

    std::time_t _start;
    std::time_t _end;

    void startProgress() {
        _start = plTimer::now();
        printProgress(0.0);
    }

    void endProgress() {
        _end = plTimer::now();
        printProgress(1.0);
        std::cout << ", elapsed time: " << ((_end - _start) / 1000.0f) << " sec" << std::endl;
    }

    void printProgress(float32_t percent) {
        std::ios::fmtflags flags(std::cout.flags());
        uint32_t prevPrecision = std::cout.precision();
        std::cout << std::fixed << std::setprecision(2);

        std::cout << "\r\tProgress: [";

        for (uint32_t i=0; i < uint32_t(percent*BAR_LENGTH); i++)
            std::cout << "=";

        for (uint32_t i=0; i < BAR_LENGTH - uint32_t(percent*BAR_LENGTH); i++)
            std::cout << " ";

        std::cout << "]  " << percent*100.0f << "%";
        std::cout.flush();

        std::cout.flags(flags);
        std::cout.precision(prevPrecision);
    }
}
