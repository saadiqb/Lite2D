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


template <typename K, typename V>
void logUnorderedMap(const std::unordered_map<K, V>& map, const std::string& label = "Map contents") {
    SDL_Log("%s (%zu items):", label.c_str(), map.size());
    for (const auto& pair : map) {
        // Note: K and V must be convertible to std::string or support streaming to std::ostringstream
        SDL_Log("  [%s] = %s", std::to_string(pair.first).c_str(), std::to_string(pair.second).c_str());
    }
}



class InputManager
{
public:
    InputManager(): 
        mCurrentEvent{}, mPrevEvent{}, noPrevEvent(true) {}
    ~InputManager() {}

    void FeedEvent(SDL_Event event) {
        mCurrentEvent = event;
        SDL_Keycode kc = event.key.key;

        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            SDL_Log("KEY_DOWN");
            // Ignore repeat events. Only process first key down.
            if (event.key.repeat == 0)
            {
                auto it = mKeysStateMap.find(kc);
                if (it == mKeysStateMap.end() || it->second == KeyState::None)
                {
                    SDL_Log("pressed mapped");
                    mKeysStateMap[kc] = KeyState::Pressed;
                }
            }

        }
        else if (event.type == SDL_EVENT_KEY_UP)
        {
            SDL_Log("KEY_UP");

            auto it = mKeysStateMap.find(kc);

            if (it == mKeysStateMap.end())
            {
                // This is an anamoly
                SDL_Log("Key UP detected but no DOWN");
            }

            if (it->second == KeyState::Pressed || it->second == KeyState::Held)
            {
                SDL_Log("released mapped");
                mKeysStateMap[kc] = KeyState::Released;
            }
        }
    }

    // Call this once per frame after processing all events
    void Update() {
        for (auto it = mKeysStateMap.begin(); it != mKeysStateMap.end(); ) {
            switch (it->second) {
                case KeyState::Pressed:
                    // After being pressed this frame, move to Held
                    SDL_Log("held mapped");

                    it->second = KeyState::Held;
                    ++it;
                    break;

                case KeyState::Released:
                    // After released, reset to None or remove from map
                    // Here we remove the key to keep map clean
                    
                    SDL_Log("released/erased");

                    it = mKeysStateMap.erase(it);
                    break;

                default:
                    // Held or None, just keep as is
                    ++it;
                    break;
            }
        }
    }

    // Query functions
    bool IsKeyPressed(SDL_Keycode sc) const {
        auto it = mKeysStateMap.find(sc);
        return it != mKeysStateMap.end() && it->second == KeyState::Pressed;
    }

    bool IsKeyHeld(SDL_Keycode sc) const {
        auto it = mKeysStateMap.find(sc);
        if (it == mKeysStateMap.end())
        {
            logUnorderedMap(mKeysStateMap, "key map");
        }
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

    InputManager(const InputManager&) = delete;
    InputManager operator=(const InputManager&) = delete;


private:

    std::unordered_map<SDL_Keycode, KeyState> mKeysStateMap;

    SDL_Event mCurrentEvent;
    SDL_Event mPrevEvent;

    bool noPrevEvent;

};