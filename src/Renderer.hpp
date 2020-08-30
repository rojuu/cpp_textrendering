#pragma once

#include "Common.hpp"
#include "SDL.h"
#include "stb_truetype.h"

struct Color32 {
    Color32(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
        : r(_r)
        , g(_g)
        , b(_b)
        , a(_a)
    {
    }
    uint8_t r, g, b, a;
};

class Renderer {
public:
    Renderer() = default;
    ~Renderer() = default;
    DELETE_COPYABLE_AND_MOVEABLE(Renderer);

    bool init(SDL_Renderer *sdlRenderer);

    void clear(uint8_t r, uint8_t g, uint8_t b) const;
    void present() const;
    void drawText(const char *text, int x, int y, Color32 color = { 0, 0, 0, 255 });

private:
    static void setSurfacePixelColor(SDL_Surface *surface, int x, int y, Color32 color);

    static constexpr int DefaultFontPixelSize = 20; ///< Specify font size in pixels

    SDL_Renderer *m_sdlRenderer {};

    struct FontInfo {
        FontInfo() = default;

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
