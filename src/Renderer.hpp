#pragma once

#include "Common.hpp"
#include "SDL.h"
#include "stb_truetype.h"

class Renderer {
public:
    Renderer() = default;
    ~Renderer() noexcept;

    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;
    Renderer(Renderer &&) = delete;
    Renderer &operator=(Renderer &&) = delete;

    bool init(const char *windowName);

    void clear(uint8_t r, uint8_t g, uint8_t b) const noexcept;
    void present() const noexcept;
    void drawText(const char *text, int x, int y) noexcept;

private:
    static void setSurfacePixelColor(
        SDL_Surface *surface, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept;

    static constexpr int DefaultFontPixelSize = 24;

    SDL_Window *m_sdlWindow {};
    SDL_Renderer *m_sdlRenderer {};

    struct FontInfo {
        FontInfo() = default;
        static constexpr int BufferPixelSize = 18;

        int bufferWidth {};
        int bufferHeight {};

        stbtt_fontinfo info {};
        std::vector<uint8_t> currentData;
        std::vector<uint8_t> pixels;
        std::vector<stbtt_bakedchar> charData;
        SDL_Texture *currentTexture {};
    };
    FontInfo m_font {};
};
