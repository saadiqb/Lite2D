#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>   
#include <SDL3_image/SDL_image.h>
#include <string>
#include <sstream>
#include <iostream>

#include "timer.h"
#include "ltexture.h"


/* Constants */
//Screen dimension constants
constexpr int kScreenWidth{ 640 };
constexpr int kScreenHeight{ 480 };
constexpr int kScreenFps { 60 };

static SDL_Window *window = nullptr;
static SDL_Renderer *renderer = nullptr;

//Global font
TTF_Font* gFont{ nullptr };

//The time text texture
LTexture gTimeTextTexture; 


bool loadMedia()
{
    //File loading flag
    bool success{ true };

    //Load scene font
    std::string fontPath{ "lazy.ttf" };
    if( gFont = TTF_OpenFont( fontPath.c_str(), 28 ); gFont == nullptr )
    {
        SDL_Log( "Could not load %s! SDL_ttf Error: %s\n", fontPath.c_str(), SDL_GetError() );
        success = false;
    }

    return success;
}


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

    gTimeTextTexture.destroy();

    TTF_CloseFont(gFont);
    gFont = nullptr;

    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
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

    loadMedia();

    std::stringstream fpsCounter;

    while (isRunning) {
        SDL_Log("--Run--");
        Timer frameTimer;
        frameTimer.start();

        SDL_Event event;

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render here

        // Frame rate control
        constexpr Uint64 nsPerFrameExpected = 1000000000 / kScreenFps; // 1 second in nanoseconds divided by FPS
        Uint64 nsPerFrameActual = frameTimer.getTicks();
        // std::cout << "Frame time: " << nsPerFrameActual << " ns" << std::endl;
        SDL_Log("Frame time: %llu ns", nsPerFrameActual);
        // fpsCounter.str("");
        // fpsCounter.precision(2);
        // fpsCounter << "FPS: " << std::fixed << (1000000000 / nsPerFrameActual);
        // std::cout << fpsCounter.str() << std::endl;  

        SDL_Color textColor = { 255, 255, 255, 255 };
        // bool rv = gTimeTextTexture.loadFromRenderedText(fpsCounter.str(), textColor);
        // if (!rv) {
        //     SDL_Log("Failed to render text: %s", SDL_GetError());
        // }

        // //Draw text
        // gTimeTextTexture.render( 100, 100, nullptr, 400, 100, 0.0, nullptr, SDL_FLIP_NONE );

        // Update the screen
        SDL_RenderPresent(renderer);

        if (nsPerFrameActual < nsPerFrameExpected) {
            SDL_DelayNS(nsPerFrameExpected - nsPerFrameActual);
        }
    }


    CleanupSDL();
    return 0;
}