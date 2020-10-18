#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{
public:
    using clock = std::chrono::high_resolution_clock;
    using us = std::chrono::microseconds;
    using timePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

    void reset()
    {
        startPoint = clock::now();
    }

    int64_t getElapsedUS() // micro seconds
    { 
        auto endPoint = clock::now();
        int64_t elapsedUS = std::chrono::time_point_cast<us>(endPoint).time_since_epoch().count() - std::chrono::time_point_cast<us>(startPoint).time_since_epoch().count();

        return elapsedUS;
    }

    double getElapsedS() // seconds
    { 
        return double(getElapsedUS()) / 1000000.0;
    }

    double getElapsedHz()
    {
        return 1.0 / getElapsedS();
    }

private:
    timePoint startPoint;
    timePoint checkPoint;
};

#endif