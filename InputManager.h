#pragma once

#include <SDL3/SDL.h>   
#include <unordered_map>
#include <string>

enum KeyState
{
    None = 0,
    Pressed,
    Held,
    Released
};

class InputManager
{
public:
    InputManager() {}
    ~InputManager() {}

    void FeedEvent(SDL_Event event) {
        SDL_Keycode kc = event.key.key;

        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            // Ignore repeat events. Only process first key down.
            if (event.key.repeat == 0)
            {
                auto it = mKeysStateMap.find(kc);
                if (it == mKeysStateMap.end() || it->second == KeyState::None)
                {
                    mKeysStateMap[kc] = KeyState::Pressed;
                }
            }

        }
        else if (event.type == SDL_EVENT_KEY_UP)
        {
            auto it = mKeysStateMap.find(kc);

            if (it == mKeysStateMap.end())
            {
                // This is an anamoly
                SDL_Log("Error: Key UP detected but no DOWN");
            }

            if (it->second == KeyState::Pressed || it->second == KeyState::Held)
            {
                mKeysStateMap[kc] = KeyState::Released;
            }
        }

        else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            Uint8 button = event.button.button;
            if (event.button.down)
            {
                auto it = mMouseButtonStateMap.find(button);
                if (it == mMouseButtonStateMap.end() || it->second == KeyState::None)
                {
                    mMouseButtonStateMap[button] = KeyState::Pressed;
                }
            }
        }
        else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
        {
            Uint8 button = event.button.button;
            auto it = mMouseButtonStateMap.find(button);

            if (it == mMouseButtonStateMap.end())
            {
                SDL_Log("Error: Mouse UP detected but no DOWN");
            }

            if (it->second == KeyState::Pressed || it->second == KeyState::Held)
            {
                mMouseButtonStateMap[button] = KeyState::Released;
            }
        }
    }

    // Call this once per frame after processing all events
    void Update() {
        for (auto it = mKeysStateMap.begin(); it != mKeysStateMap.end(); ) {
            switch (it->second) {
                case KeyState::Pressed:
                    // After being pressed this frame, move to Held
                    it->second = KeyState::Held;
                    ++it;
                    break;

                case KeyState::Released:
                    // After released, reset to None or remove from map
                    // Here we remove the key to keep map clean
                    it = mKeysStateMap.erase(it);
                    break;

                default:
                    // Held or None, just keep as is
                    ++it;
                    break;
            }
        }

        for (auto it = mMouseButtonStateMap.begin(); it != mMouseButtonStateMap.end(); ) {
            switch (it->second) {
                case KeyState::Pressed:
                    // After being pressed this frame, move to Held
                    it->second = KeyState::Held;
                    ++it;
                    break;

                case KeyState::Released:
                    // After released, reset to None or remove from map
                    // Here we remove the key to keep map clean
                    it = mMouseButtonStateMap.erase(it);
                    break;

                default:
                    // Held or None, just keep as is
                    ++it;
                    break;
            }
        }


        SDL_MouseButtonFlags mouseButtonFlags {};

        float currX;
        float currY;
        mouseButtonFlags = SDL_GetMouseState(&currX, &currY);

        mMouseDeltaX = currX - mMouseXPosition;
        mMouseDeltaY = currY - mMouseYPosition;

        mMouseXPosition = currX;
        mMouseYPosition = currY;
    
    }

    // Query functions
    bool IsKeyPressed(SDL_Keycode sc) const {
        auto it = mKeysStateMap.find(sc);
        return it != mKeysStateMap.end() && it->second == KeyState::Pressed;
    }

    bool IsKeyHeld(SDL_Keycode sc) const {
        auto it = mKeysStateMap.find(sc);
        return it != mKeysStateMap.end() && (it->second == KeyState::Held || it->second == KeyState::Pressed);
    }

    bool IsKeyReleased(SDL_Keycode sc) const {
        auto it = mKeysStateMap.find(sc);
        return it != mKeysStateMap.end() && it->second == KeyState::Released;
    }

    void GetKeyState(SDL_Keycode sc, KeyState keystate)
    {
        auto it = mKeysStateMap.find(sc);
        if (it == mKeysStateMap.end())
        {
            keystate = KeyState::None;
        }
        keystate = it->second;
    }



    void GetMousePosition(int& x, int& y)
    {
        x = mMouseXPosition;
        y = mMouseYPosition;
    }

    void GetMouseDelta(int& dx, int& dy)
    {
        dx = mMouseDeltaX;
        dy = mMouseDeltaY;
    }

    bool IsMouseButtonPressed(Uint8 button)
    {
        auto it = mMouseButtonStateMap.find(button);
        return it != mMouseButtonStateMap.end() && it->second == KeyState::Pressed;
    }

    bool IsMouseButtonHeld(Uint8 button) {
        auto it = mMouseButtonStateMap.find(button);
        return it != mMouseButtonStateMap.end() && it->second == KeyState::Held;
    }

    bool IsMouseButtonReleased(Uint8 button) {
        auto it = mMouseButtonStateMap.find(button);
        return it != mMouseButtonStateMap.end() && it->second == KeyState::Released;
    }

    bool IsMouseInRect(int x, int y, int w, int h) {
        return (mMouseXPosition >= x) && (mMouseXPosition <= x + w) &&
               (mMouseYPosition >= y) && (mMouseYPosition <= y + h);

    }


    InputManager(const InputManager&) = delete;
    InputManager operator=(const InputManager&) = delete;


private:

    std::unordered_map<SDL_Keycode, KeyState> mKeysStateMap;

    std::unordered_map<Uint8, KeyState> mMouseButtonStateMap;

    float mMouseXPosition;
    float mMouseYPosition;
    float mMouseDeltaX;
    float mMouseDeltaY;

};