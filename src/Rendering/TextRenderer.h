#pragma once
#include <SDL3_ttf/SDL_ttf.h>   

#include "Texture.h"

constexpr int defaultFontSize = 28;

class TextRenderer
{
public:
    TextRenderer() : mWidth(0), mHeight(0), mFont(nullptr), mTextTexture(0, 0) {}

    TextRenderer(int width, int height) : mWidth(width), mHeight(height), mFont(nullptr), mTextTexture(width, height) {}
    
    ~TextRenderer() {
        if (mFont) {
            TTF_CloseFont(mFont);
            mFont = nullptr;
        }
    }

    TextRenderer(const TextRenderer&) = delete;
    TextRenderer& operator=(const TextRenderer&) = delete;

    bool LoadFont(const std::string& fontPath, int fontSize = defaultFontSize)
    {
        mFont = TTF_OpenFont(fontPath.c_str(), fontSize);
        if (!mFont) {
            SDL_Log("Failed to load font: %s", SDL_GetError());
            return false;
        }

        return true;
    }


    bool RenderText(std::string text, SDL_Color color, int x, int y, SDL_Renderer* renderer)
    {
        if (!mFont) {
            SDL_Log("Font not loaded");
            return false;
        }

        mFontColor = color;
        mActualText = text;

        int calculatedWidth;
        int calculatedHeight;

        if (TTF_GetStringSize(mFont, text.c_str(), text.length(), &calculatedWidth, &calculatedHeight) == false) {
            SDL_Log("Failed to get text size: %s", SDL_GetError());
            return false;
        }

        mWidth = calculatedWidth;
        mHeight = calculatedHeight;

        SDL_Surface* surface = TTF_RenderText_Blended(mFont, text.c_str(), text.length(), color);
        if (!surface) {
            SDL_Log("Failed to render text: %s", SDL_GetError());
            return false;
        }

        if (!mTextTexture.LoadFromSurface(surface, renderer)) {
            SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
            SDL_DestroySurface(surface);
            return false;
        }

        mTextTexture.Render(renderer, x, y, nullptr, static_cast<float>(calculatedWidth), static_cast<float>(calculatedHeight));

        return true;
    }

private:

    TTF_Font* mFont{ nullptr };
    Texture mTextTexture;
    
    int mDefaultFontSize { defaultFontSize };

    int mWidth{ 0 };
    int mHeight{ 0 };
    
    SDL_Color mFontColor { 255, 255, 255, 255 }; // Default white color
    std::string mActualText;                     // The actual text to render

};