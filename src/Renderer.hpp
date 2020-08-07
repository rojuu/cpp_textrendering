#pragma once

#include "SDL.h"
#include "stb_truetype.h"
#include <string>
#include <vector>

class Renderer {
public:
    explicit Renderer(SDL_Window *window);
    ~Renderer() noexcept;

    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;
    Renderer(Renderer &&) = delete;
    Renderer &operator=(Renderer &&) = delete;

    void clear(uint8_t r, uint8_t g, uint8_t b) const noexcept;
    void present() const noexcept;
    void drawText(const char *text, int x, int y, float scale = 1.f) noexcept;

private:
    static void setSurfacePixelColor(
        SDL_Surface *surface, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept;

    static constexpr int DefaultFontPixelSize = 24;

    SDL_Renderer *m_sdlRenderer;

    struct FontInfo {
        std::vector<uint8_t> currentData;
        int currentSize = -1;
        std::string currentFileName;

        static constexpr int BufferPixelSize = 48;
        int bufferWidth {};
        int bufferHeight {};
        std::vector<uint8_t> pixels;
        std::vector<stbtt_bakedchar> charData;
        SDL_Texture *currentTexture {};
    };
    FontInfo m_font;
};
