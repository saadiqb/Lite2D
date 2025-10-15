#pragma once
#include <SDL3/SDL.h>


class Timer
{
public:
    Timer() : _startTicks(0), _pausedTicks(0), _isPaused(false), _isStarted(false), _frequency(SDL_GetPerformanceFrequency()) {}

    void Start() {
        _isStarted = true;
        _isPaused = false;
        _startTicks = SDL_GetPerformanceCounter();
    }

    void Stop() {
        _isStarted = false;
        _isPaused = false;
        _startTicks = 0;
        _pausedTicks = 0;
    }
    
    void Pause() {
        if (_isStarted && !_isPaused) {
            _isPaused = true;
            _pausedTicks = SDL_GetPerformanceCounter() - _startTicks;
        }
    }

    void Unpause() {
        if (_isStarted && _isPaused) {
            _isPaused = false;
            _startTicks = SDL_GetPerformanceCounter() - _pausedTicks;
            _pausedTicks = 0;
        }
    }

    // Raw tick count
    Uint64 GetTicksRaw() const {
        if (_isStarted) {
            if (_isPaused) {
                return _pausedTicks;
            } else {
                return SDL_GetPerformanceCounter() - _startTicks;
            }
        }
        return 0;
    }

    double GetElapsedSeconds() const {
        Uint64 ticks = GetTicksRaw();
        if (_frequency == 0) {
            // SDL_Log("Performance frequency is zero, cannot calculate elapsed seconds.");
            return 0.0f; // Avoid division by zero
        }
        // SDL_Log("Performance frequency: %llu, Ticks: %llu", frequency, ticks);
        // SDL_Log("Elapsed seconds: %f", static_cast<double>(ticks) / static_cast<double>(frequency));
        return static_cast<double>(ticks) / static_cast<double>(_frequency);
    }

    Uint32 GetElapsedMilliseconds() const {
        return static_cast<Uint32>(GetElapsedSeconds() * 1000.0);
    }

    bool isStarted() const { return _isStarted; }
    bool isPaused() const { return _isPaused; }
    
private:
    Uint64 _frequency; /**< The frequency of the performance counter */

    Uint64 _startTicks;   /**< The clock time when the timer started */
    Uint64 _pausedTicks;  /**< The clock time when the timer was paused */
    bool _isPaused;       /**< The timer is paused */
    bool _isStarted;      /**< The timer is started */
};