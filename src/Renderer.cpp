#include "Renderer.hpp"
#include "Utils.hpp"

#include "SDL.h"

#include <iostream>

Renderer *Renderer::createRenderer(SDL_Window *window)
{
    Renderer *renderer = new Renderer;

    renderer->m_sdlRenderer
        = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer->m_sdlRenderer) {
        printErr("Render backend failed to initalize SDL_Renderer: %\n", SDL_GetError());
        delete renderer;
        return nullptr;
    }

    renderer->m_currentFontData = readEntireBinaryFile("Roboto-Regular.ttf");
    if (!renderer->m_currentFontData) {
        printErr("Failed to load default font\n");
        delete renderer;
        return nullptr;
    }

    stbtt_InitFont(&renderer->m_currentFont, renderer->m_currentFontData, 0);

    return renderer;
}

void Renderer::destroyRenderer(Renderer *renderer)
{
    if (!renderer) {
        return;
    }

    renderer->destroyFontCache();

    SDL_DestroyRenderer(renderer->m_sdlRenderer);

    freeBinaryFileContents(renderer->m_currentFontData);

    delete renderer;
}

void Renderer::destroyFontCache()
{
    // SDL_Textures need to be destroyed manually
    for (auto &it : m_glyphDataMap) {
        auto &glyphData = it.second;
        if (glyphData.texture) {
            SDL_DestroyTexture(glyphData.texture);
        }
    }
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

    float fontScale = stbtt_ScaleForPixelHeight(&m_currentFont, (float)pixelSize);

    int w, h, xoff, yoff;
    uint8_t *bmp = stbtt_GetCodepointBitmap(
        &m_currentFont, fontScale, fontScale, character, &w, &h, &xoff, &yoff);
    if (!w || !h) {
        stbtt_FreeBitmap(bmp, 0);

        int advance, leftSideBearing;
        stbtt_GetCodepointHMetrics(&m_currentFont, character, &advance, &leftSideBearing);

        result.width = (int)(fontScale * advance);

        return result;
    }

    SDL_Surface *surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    if (!surface) {
        printErr("Failed to create SDL_Surface: %\n", SDL_GetError());
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
        printErr("Failed to create SDL_Texture: %\n", SDL_GetError());
        return result;
    }
    SDL_FreeSurface(surface);

    result.texture = texture;
    result.width = w;
    result.height = h;
    result.xOffset = xoff;
    result.yOffset = yoff;

    stbtt_FreeBitmap(bmp, 0);

    return result;
}

Renderer::GlyphData Renderer::getOrCreateGlyphData(char character, int pixelSize)
{
    GlyphData result;

    const auto create = [&]() {
        result = createGlyphData(character, pixelSize);
        m_glyphDataMap[character] = result;
    };
    if (pixelSize != m_currentFontSize || m_currentFontSize == -1) {
        m_currentFontSize = pixelSize;
        destroyFontCache();
        create();
    } else {
        const auto it = m_glyphDataMap.find(character);
        if (it == m_glyphDataMap.end()) {
            create();
        } else {
            result = it->second;
        }
    }

    return result;
}

void Renderer::drawText(const char *text, int x, int y)
{
    DynArray<GlyphData> glyphDatas;
    for (int ch = 0; text[ch]; ++ch) {
        glyphDatas.emplace_back(getOrCreateGlyphData(text[ch], m_currentFontSize));
    }

    for (auto &gd : glyphDatas) {
        if (gd.texture) { // for spaces we have null texture, but have to advance x
            SDL_Rect dstRect { x + gd.xOffset, y + gd.yOffset, gd.width, gd.height };
            SDL_Rect srcRect { 0, 0, gd.width, gd.height };

            SDL_SetTextureBlendMode(gd.texture, SDL_BLENDMODE_ADD);
            SDL_RenderCopy(m_sdlRenderer, gd.texture, &srcRect, &dstRect);
        }
        x += gd.width + gd.xOffset;
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
