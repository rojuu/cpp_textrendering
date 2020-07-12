#pragma once

#include "Types.hpp"
#include "Utils.hpp"
#include "stb_truetype.h"
#include "SDL.h"

class Renderer {
    SDL_Renderer *m_sdlRenderer;
    stbtt_fontinfo m_defaultFont;
    uint8_t *m_defaultFontData;

    struct GlyphData {
        SDL_Texture *texture;
        int width, height;
        int xOffset, yOffset;
    };
    HashMap<int, HashMap<char, GlyphData>> m_fontSizeToGlyphDataMap;

public:
    static Renderer *createRenderer(SDL_Window *window);
    static void destroyRenderer(Renderer *renderer);

    void clear(uint8_t r, uint8_t g, uint8_t b);
    void present();

    void drawText(const char *text, int pixelSize, int x, int y);

private:
    void setSurfacePixelColor(
        SDL_Surface *surface, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) const;

    GlyphData createGlyphData(char character, int pixelSize);
    GlyphData getOrCreateGlyphData(char character, int pixelSize);
};
