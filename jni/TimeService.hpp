#ifndef _TIMESERVICE_HPP_
#define _TIMESERVICE_HPP_

#include "Types.hpp"

#include <time.h>


    class TimeService {
    public:
        TimeService();

        void reset();
        void update();

        double now();
        float elapsed();

    private:
        float mElapsed;
        double mLastTime;
    };

#endif
