#pragma once

#include <SDL3/SDL.h>

class Renderer {
public:
    Renderer(SDL_Renderer* renderer) : mRenderer(renderer) {}
    ~Renderer() {
        // Incase you forget to call Shutdown
        Shutdown();
    }

    // Delete copy constructor and assignment operator
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    // Delete move constructor and assignment operator
    Renderer(Renderer&&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    // Get the underlying SDL_Renderer
    SDL_Renderer* GetRendererObject() const { return mRenderer; }

    void Shutdown() {
        if (mRenderer) {
            SDL_DestroyRenderer(mRenderer);
            mRenderer = nullptr;
        }
    }

    void Clear() const
    {
        if (mRenderer) {
            SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
            SDL_RenderClear(mRenderer);
        }
    }

    void Present() const
    {
        if (mRenderer) {
            SDL_RenderPresent(mRenderer);
        }
    }

    // Todo: create color class
    void SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
    {
        if (mRenderer) {
            SDL_SetRenderDrawColor(mRenderer, r, g, b, a);
        }
    }

    bool SetRenderDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
    {
        return mRenderer ? SDL_SetRenderDrawColor(mRenderer, r, g, b, a) : false;
    }

    bool RenderClear() const
    {
        return mRenderer ? SDL_RenderClear(mRenderer) : false;
    }

private:
    SDL_Renderer* mRenderer{ nullptr };
};