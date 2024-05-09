#ifndef BREAKOUT_CLOCK_H
#define BREAKOUT_CLOCK_H

#ifdef _WIN32

#include <SDL.h>

#else
#include <SDL2/SDL.h>
#endif


class Clock {
public:
    Clock();

    double tick(int fps);

    int get_fps() const;

    int getFrameCount() const {
        return frame_count;
    }

private:
    double last_time;
    double delta_time;
    double perf_freq;
    int frame_count;
};


#endif //BREAKOUT_CLOCK_H
