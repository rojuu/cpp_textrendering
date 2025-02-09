#include "Renderer.hpp"
#include "Utils.hpp"

Renderer::Renderer(SDL_Renderer *sdlRenderer)
{
    m_sdlRenderer = sdlRenderer;

    constexpr auto fontFile = "FiraMono.ttf";
    m_font.currentData = readEntireBinaryFile(fontFile);
    if (m_font.currentData.empty()) {
        throw std::runtime_error(fmt::format("Failed to load font file: {}\n", fontFile));
    }

    m_font.bufferWidth = 512;
    m_font.bufferHeight = 512;
    m_font.pixels.resize(m_font.bufferWidth * m_font.bufferHeight * sizeof(m_font.pixels[0]));
    m_font.charData.resize(512);
    // TODO: Use Improved 3D API (stbtt_PackBegin, etc, maybe even stb_rect_pack.h)
    const int bakeFontResult = stbtt_BakeFontBitmap(m_font.currentData.data(), 0,
        DefaultFontPixelSize, m_font.pixels.data(), m_font.bufferWidth, m_font.bufferHeight, 0,
        m_font.charData.size(), m_font.charData.data());

    if (bakeFontResult > 0) { // unused rows
    } else { // fit this many characters (abs)
        m_font.charData.resize(std::abs(bakeFontResult));
    }

    stbtt_InitFont(&m_font.info, m_font.currentData.data(), 0);

    // Init font texture
    {
        int w = m_font.bufferWidth;
        int h = m_font.bufferHeight;

        SDL_Surface *surface
            = SDL_CreateRGBSurface(0, w, h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
        if (!surface) {
            throw std::runtime_error(
                fmt::format("Failed to create SDL_Surface: {}\n", SDL_GetError()));
        }

        SDL_LockSurface(surface);
        for (int i = 0; i < surface->w && surface; ++i) {
            for (int j = 0; j < surface->h; ++j) {
                uint8_t alpha = m_font.pixels[j * surface->w + i];
                setSurfacePixelColor(surface, i, j, { 255, 255, 255, alpha });
            }
        }
        SDL_UnlockSurface(surface);

        m_font.currentTexture = SDL_CreateTextureFromSurface(m_sdlRenderer, surface);
        if (!m_font.currentTexture) {
            throw std::runtime_error(
                fmt::format("Failed to create SDL_Texture: {}\n", SDL_GetError()));
        }
    }
}

void Renderer::clear(Color32 color) const noexcept
{
    SDL_SetRenderDrawColor(m_sdlRenderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_sdlRenderer);
}

void Renderer::present() const noexcept
{
    SDL_RenderPresent(m_sdlRenderer);
}

void Renderer::drawText(const char *text, int _x, int _y, Color32 color) noexcept
{
#if 0
        SDL_Rect fontTexRect;
        fontTexRect.x = 720;
        fontTexRect.y = 70;
        fontTexRect.h = 512;
        fontTexRect.w = 512;
        SDL_RenderCopy(m_sdlRenderer, m_font.currentTexture, nullptr, &fontTexRect);
#endif

    int ascent = 0, descent = 0, lineGap = 0;
    stbtt_GetFontVMetrics(&m_font.info, &ascent, &descent, &lineGap);

    float scale = stbtt_ScaleForPixelHeight(&m_font.info, DefaultFontPixelSize);
    float yadvance = static_cast<float>(ascent - descent + lineGap) * scale;

    int x = _x;
    int y = _y;
    for (int ch = 0; text[ch] != '\0'; ++ch) {
        const stbtt_bakedchar &bc = m_font.charData[text[ch]];

        if (text[ch] == '\n') {
            x = _x;
            y += static_cast<int>(yadvance);
            continue;
        }

        const int w = bc.x1 - bc.x0;
        const int h = bc.y1 - bc.y0;
        SDL_Rect src { bc.x0, bc.y0, w, h };
        SDL_Rect dst { x + static_cast<int>(bc.xoff), y + static_cast<int>(bc.yoff), w, h };
        SDL_SetTextureColorMod(m_font.currentTexture, color.r, color.g, color.b);
        SDL_SetTextureAlphaMod(m_font.currentTexture, color.a);
        SDL_RenderCopy(m_sdlRenderer, m_font.currentTexture, &src, &dst);

        x += static_cast<int>(bc.xadvance);
    }
}

void Renderer::setSurfacePixelColor(SDL_Surface *surface, int x, int y, Color32 color) noexcept
{
    int off = y * surface->w + x;
    uint32_t *ptr = (static_cast<uint32_t *>(surface->pixels)) + off;
    *ptr = SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a);
}
