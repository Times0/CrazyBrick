#include "../include/clock.h"

Clock::Clock() {
    last_time = 0;
    delta_time = 0;
    perf_freq = SDL_GetPerformanceFrequency();
    frame_count = 0;
}

double Clock::tick(int fps) {
    frame_count++;
    double current_time = SDL_GetPerformanceCounter();
    delta_time = (current_time - last_time) / perf_freq;
    double target_frame_time = 1.0 / fps;
    double sleep_time = target_frame_time - delta_time;

    if (sleep_time > 0) {
        SDL_Delay(static_cast<Uint32>(sleep_time * 1000)); // Sleep in milliseconds
        current_time = SDL_GetPerformanceCounter();
        delta_time = static_cast<double>(current_time - last_time) / perf_freq;
    }

    last_time = current_time;
    return delta_time;
}

int Clock::get_fps() const {
    // only keep the integer part of the result
    return static_cast<int>(1.0 / delta_time);
}

