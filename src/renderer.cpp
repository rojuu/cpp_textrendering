#include "renderer.hpp"
#include "utils.hpp"

#include "SDL.h"

#include <iostream>

Renderer *Renderer::createRenderer(SDL_Window *window)
{
    Renderer *renderer = new Renderer;

    renderer->m_sdlRenderer
        = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer->m_sdlRenderer) {
        std::cerr << "Render backend failed to initalize SDL_Renderer: " << SDL_GetError() << "\n";
        delete renderer;
        return nullptr;
    }

    renderer->m_defaultFontData = readEntireBinaryFile("Roboto-Regular.ttf");
    if (!renderer->m_defaultFontData) {
        std::cerr << "Failed to load default font\n";
        delete renderer;
        return nullptr;
    }

    stbtt_InitFont(&renderer->m_defaultFont, renderer->m_defaultFontData, 0);

    return renderer;
}

void Renderer::destroyRenderer(Renderer *renderer)
{
    if (!renderer) {
        return;
    }

    // SDL_Textures need to be destroyd manually
    for (auto &glyphDataIt : renderer->m_fontSizeToGlyphDataMap) {
        for (auto &gd : glyphDataIt.second) {
            if (gd.second.texture) {
                SDL_DestroyTexture(gd.second.texture);
            }
        }
    }

    SDL_DestroyRenderer(renderer->m_sdlRenderer);

    freeBinaryFileContents(renderer->m_defaultFontData);

    delete renderer;
}

void Renderer::clear(uint8_t r, uint8_t g, uint8_t b)
{
    SDL_SetRenderDrawColor(m_sdlRenderer, r, g, b, 255);
    SDL_RenderClear(m_sdlRenderer);
}

void Renderer::present()
{
    SDL_RenderPresent(m_sdlRenderer);
}

Renderer::GlyphData Renderer::createGlyphData(char character, int pixelSize)
{
    GlyphData result = {};

    float fontScale = stbtt_ScaleForPixelHeight(&m_defaultFont, (float)pixelSize);

    int w, h, xoff, yoff;
    uint8_t *bmp = stbtt_GetCodepointBitmap(
        &m_defaultFont, fontScale, fontScale, character, &w, &h, &xoff, &yoff);
    if (!w || !h) {
        stbtt_FreeBitmap(bmp, 0);

        int advance, leftSideBearing;
        stbtt_GetCodepointHMetrics(&m_defaultFont, character, &advance, &leftSideBearing);

        result.width = (int)(fontScale * advance);

        return result;
    }

    SDL_Surface *surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    if (!surface) {
        std::cerr << "Failed to create SDL_Surface: " << SDL_GetError() << "\n";
        return result;
    }

    SDL_LockSurface(surface);
    for (int i = 0; i < surface->w; ++i) {
        for (int j = 0; j < surface->h; ++j) {
            uint8_t alpha = bmp[j * surface->w + i];
            setSurfacePixelColor(surface, i, j, alpha, alpha, alpha, alpha);
        }
    }
    SDL_UnlockSurface(surface);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(m_sdlRenderer, surface);
    if (!texture) {
        std::cerr << "Failed to create SDL_Texturer: " << SDL_GetError() << "\n";
        return result;
    }
    SDL_FreeSurface(surface);

    result.texture = texture;
    result.width = w;
    result.height = h;
    result.offset_x = xoff;
    result.offset_y = yoff;

    stbtt_FreeBitmap(bmp, 0);

    return result;
}

Renderer::GlyphData Renderer::getOrCreateGlyphData(char character, int pixelSize)
{
    GlyphData result;

    const auto glyphMapIt = m_fontSizeToGlyphDataMap.find(pixelSize);
    if (glyphMapIt != m_fontSizeToGlyphDataMap.end()) {
        auto &glyphMap = glyphMapIt->second;
        const auto glyphDataIt = glyphMap.find(character);
        if (glyphDataIt != glyphMap.end()) {
            result = glyphDataIt->second;
        } else {
            result = createGlyphData(character, pixelSize);
            glyphMap[character] = result;
        }
    } else {
        result = createGlyphData(character, pixelSize);
        HashMap<char, GlyphData> glyphMap;
        glyphMap[character] = result;
        m_fontSizeToGlyphDataMap[pixelSize] = glyphMap;
    }

    return result;
}

void Renderer::printText(const char *text, int pixelSize, int x, int y)
{
    DynArray<GlyphData> glyphDatas;
    for (int ch = 0; text[ch]; ++ch) {
        glyphDatas.emplace_back(getOrCreateGlyphData(text[ch], pixelSize));
    }

    for (auto &gd : glyphDatas) {
        if (gd.texture) { // for spaces we have null texture, but have to advance x
            SDL_Rect dstRect { x + gd.offset_x, y + gd.offset_y, gd.width, gd.height };
            SDL_Rect srcRect { gd.width, gd.height, 0, 0 };

            SDL_SetTextureBlendMode(gd.texture, SDL_BLENDMODE_ADD);
            SDL_RenderCopy(m_sdlRenderer, gd.texture, &srcRect, &dstRect);
        }
        x += gd.width + gd.offset_x;
    }
}

void Renderer::setSurfacePixelColor(
    SDL_Surface *surface, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) const
{
    int off = y * surface->w + x;
    uint32_t *ptr = ((uint32_t *)surface->pixels) + off;
    uint32_t color = SDL_MapRGBA(surface->format, r, g, b, a);
    *ptr = color;
}
