#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>   
#include <SDL3_image/SDL_image.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "timer.h"
#include "Texture.h"
#include "TextRenderer.h"
#include "Renderer.h"
#include "InputManager.h"

/* Constants */
//Screen dimension constants
constexpr int kScreenWidth{ 1920 };
constexpr int kScreenHeight{ 1080 };
constexpr int kScreenFps { 10 };

static SDL_Window *window = nullptr;

static SDL_Renderer *renderer = nullptr;

//Global font
// TTF_Font* gFont{ nullptr };

bool InitSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return false;
    }

    // Todo: Api to title
    if (SDL_CreateWindowAndRenderer("Lite2D", kScreenWidth, kScreenHeight, 0, &window, &renderer) < 0) {
        SDL_Log("Window could not be created! SDL_Error: %s", SDL_GetError());
        return false;
    }

    if (!SDL_SetRenderVSync(renderer, true)) {
        SDL_Log("VSync could not be enabled! SDL_Error: %s", SDL_GetError());
        return false;
    }

    if (!TTF_Init()) {
        SDL_Log("SDL_ttf could not initialize! TTF_Error: %s", SDL_GetError());
        return false;
    }

    return true;
}

void CleanupSDL()
{
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    TTF_Quit();
    SDL_Quit();
}

static bool isRunning = true;

int main( int argc, char* args[] )
{
    bool initSuccess = InitSDL();
    if (!initSuccess) {
        CleanupSDL();
        return -1;
    }

    InputManager inputManager;

    Renderer rendererObj(renderer);
    TextRenderer gTimeTextTexture;

    Texture ratTexture(200, 200);
    ratTexture.LoadFromFile("rat.png", renderer);

    gTimeTextTexture.LoadFont("lazy.ttf", 28);

    std::stringstream fpsCounter;

    Timer frameTimer;
    
    Timer fpsTimer;
    fpsTimer.Start();

    double fps = 0.0;
    const int fpsUpdateIntervalMs = 500; // Update FPS every 500 milliseconds

    double lastMeasuredFps = 0.0;

    while (isRunning) {
        // SDL_Log("New frame...");
        frameTimer.Start();

        SDL_Event event;

        //------------------------------------------------------------------
        // Input Handling
        while (SDL_PollEvent(&event)) {
            // SDL_Log("Event type: %d", event.type);

            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }

            if (event.type == SDL_EVENT_WINDOW_MOVED || event.type == SDL_EVENT_WINDOW_RESIZED) {
                SDL_Log("Window moved or resized: %d, %d", event.window.data1, event.window.data2);
            }     
            

            inputManager.FeedEvent(event);
            inputManager.Update();

            if (inputManager.IsKeyHeld(SDLK_SPACE))
            {
                SDL_Log("----space -----");
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, NULL);
            }

        }
        //------------------------------------------------------------------


        //------------------------------------------------------------------
        // Clear the screen
        rendererObj.SetDrawColor(0, 0, 0, 255);
        rendererObj.Clear();



        //------------------------------------------------------------------
        // Render here
        fpsCounter.str(""); // Clear the stringstream
        fpsCounter << "FPS: " << std::fixed << std::setprecision(2) << lastMeasuredFps;
        
        SDL_Color white = { 255, 255, 255, 255 };
        gTimeTextTexture.RenderText(fpsCounter.str(), white, 10, 10, renderer);


        ratTexture.Render(renderer, 200, 200, nullptr, 200.0f, 200.0f);

        
        if (inputManager.IsKeyHeld(SDLK_SPACE))
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, NULL);
        }


        //------------------------------------------------------------------
        // Frame rate control
        double frameTimeSeconds = frameTimer.GetElapsedSeconds();
        if (frameTimeSeconds <= 0.0) {
            // SDL_Log("Frame time is zero or negative, skipping frame rate control.");
            continue; // Skip frame rate control if frame time is invalid
        }

        // Update the screen
        rendererObj.Present();

        if (frameTimeSeconds > (1.0 / kScreenFps)) {
            SDL_Log("Frame rate is lower than desired");
        }
        else
        {
            // SDL_Log("Frame too fast. Need delay");
            SDL_Delay(static_cast<Uint32>((((1.0 / kScreenFps) - frameTimeSeconds) * 1000 )));
        }

        double totalFrameTime = frameTimer.GetElapsedSeconds();
        lastMeasuredFps = (totalFrameTime > 0.0) ? (1.0 / totalFrameTime) : 0.0;
        //------------------------------------------------------------------

    }

    SDL_Log("Exiting main loop...");

    rendererObj.Shutdown();
    CleanupSDL();
    return 0;
}