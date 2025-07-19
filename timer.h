#pragma once
#include <SDL3/SDL.h>


class Timer
{
public:
    Timer() : _startTicks(0), _pausedTicks(0), _isPaused(false), _isStarted(false) {}

    void start() {
        _isStarted = true;
        _isPaused = false;
        _startTicks = SDL_GetTicks();
    }

    void stop() {
        _isStarted = false;
        _isPaused = false;
        _startTicks = 0;
        _pausedTicks = 0;
    }
    
    void pause() {
        if (_isStarted && !_isPaused) {
            _isPaused = true;
            _pausedTicks = SDL_GetTicks() - _startTicks;
            _startTicks = 0;
        }
    }

    void unpause() {
        if (_isPaused) {
            _isPaused = false;
            _startTicks = SDL_GetTicks() - _pausedTicks;
            _pausedTicks = 0;
        }
    }

    Uint64 getTicks() const {
        if (_isStarted) {
            if (_isPaused) {
                return _pausedTicks;
            } else {
                return SDL_GetTicks() - _startTicks;
            }
        }
        return 0;
    }

    bool isStarted() const { return _isStarted; }
    bool isPaused() const { return _isPaused; }
    
private:
    Uint64 _startTicks;   /**< The clock time when the timer started */
    Uint64 _pausedTicks;  /**< The clock time when the timer was paused */
    bool _isPaused;       /**< The timer is paused */
    bool _isStarted;      /**< The timer is started */
};