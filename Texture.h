#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>

class Texture
{
public:
    Texture(int width, int height): mTexture(nullptr), mWidth(width), mHeight(height) {}
    ~Texture() { Destroy(); }

    bool LoadFromFile(const std::string& path, SDL_Renderer* renderer)
    {
        if (mTexture) {
            Destroy();
        }

        mTexture = IMG_LoadTexture(renderer, path.c_str());
        if (!mTexture) {
            SDL_Log("Failed to load texture from file: %s", SDL_GetError());
            return false;
        }
        return true;
    }

    void Render(SDL_Renderer* renderer, float x, float y, SDL_FRect* clip = nullptr, float width = -1.f, float height = -1.f, double degrees = 0.0, SDL_FPoint* center = nullptr, SDL_FlipMode flipMode = SDL_FLIP_NONE)
    {
        if (!mTexture) return;

        SDL_FRect dstRect{ x, y, static_cast<float>(mWidth), static_cast<float>(mHeight) };

        if (clip) {
            dstRect.w = clip->w;
            dstRect.h = clip->h;
        }

        if (width > 0) {
            dstRect.w = width;
        }
        if (height > 0) {
            dstRect.h = height;
        }

        SDL_RenderTextureRotated(renderer, mTexture, clip, &dstRect, degrees, center, flipMode);
    }

    void Scale(float scaleX, float scaleY)
    {
        if (mTexture) {
            mWidth = static_cast<int>(mWidth * scaleX);
            mHeight = static_cast<int>(mHeight * scaleY);
        }
    }

    void SetColorModulation(Uint8 r, Uint8 g, Uint8 b)
    {
        if (mTexture) {
            SDL_SetTextureColorMod(mTexture, r, g, b);
        }
    }

    void SetAlphaModulation(Uint8 alpha)
    {
        if (mTexture) {
            SDL_SetTextureAlphaMod(mTexture, alpha);
        }
    }

private:
    void Destroy()
    {
        if (mTexture) {
            SDL_DestroyTexture(mTexture);
            mTexture = nullptr;
        }
        mWidth = 0;
        mHeight = 0;
    }

    //Contains texture data
    SDL_Texture* mTexture;

    //Metadata
    int mWidth;
    int mHeight;

    //Default size
    float defaultSize = -1.f;
};