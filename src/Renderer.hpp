// TODO: draw using font buffer instead of individual glyphs
class Renderer {
    SDL_Renderer *m_sdlRenderer;
    stbtt_fontinfo m_currentFont;
    uint8_t *m_currentFontData;
    int m_currentFontSize = -1;
    std::string m_currentFontFile;

    struct GlyphData {
        SDL_Texture *texture;
        int width, height;
        int xOffset, yOffset;
    };
    std::unordered_map<char, GlyphData> m_glyphDataMap;

    static constexpr int BufferFontPixelSize = 48;
    int m_fontBufferWidth;
    int m_fontBufferHeight;
    std::vector<uint8_t> m_pixels;
    std::vector<stbtt_bakedchar> m_charData;

public:
    static constexpr int DefaultFontPixelSize = 24;

    explicit Renderer(SDL_Window *window)
    {
        m_sdlRenderer
            = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!m_sdlRenderer) {
            throw std::runtime_error(
                fmt::format("Failed to initialize SDL_Renderer: {}", SDL_GetError()));
        }

        m_currentFontData = readEntireBinaryFile("Roboto-Regular.ttf");
        if (!m_currentFontData) {
            throw std::runtime_error("Failed to initialize SDL_Renderer");
        }

        m_fontBufferWidth = 512;
        m_fontBufferHeight = 512;
        m_pixels.reserve(m_fontBufferWidth * m_fontBufferHeight * sizeof(m_pixels[0]));
        m_charData.resize(512);
        const int bakeFontResult
            = stbtt_BakeFontBitmap(m_currentFontData, 0, BufferFontPixelSize, m_pixels.data(),
                m_fontBufferWidth, m_fontBufferHeight, 0, m_charData.size(), m_charData.data());

        if (bakeFontResult > 0) { // unused rows
        } else { // fit this many characters (abs)
            m_charData.resize(std::abs(bakeFontResult));
        }

        stbtt_InitFont(&m_currentFont, m_currentFontData, 0);
    }

    ~Renderer() noexcept
    {
        destroyFontCache();
        if (m_sdlRenderer) {
            SDL_DestroyRenderer(m_sdlRenderer);
        }
        freeBinaryFileContents(m_currentFontData);
    }

    void clear(uint8_t r, uint8_t g, uint8_t b) const noexcept
    {
        SDL_SetRenderDrawColor(m_sdlRenderer, r, g, b, 255);
        SDL_RenderClear(m_sdlRenderer);
    }

    void present() const noexcept
    {
        const int w = m_fontBufferWidth, h = m_fontBufferHeight;

        static bool initted = false;
        static SDL_Texture *texture = nullptr;
        if (!initted) {
            static SDL_Surface *surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
            if (!surface) {
                printErr("Failed to create SDL_Surface: {}\n", SDL_GetError());
                initted = true;
                goto end;
            }

            SDL_LockSurface(surface);
            for (int i = 0; i < surface->w && surface; ++i) {
                for (int j = 0; j < surface->h; ++j) {
                    uint8_t alpha = m_pixels[j * surface->w + i];
                    setSurfacePixelColor(surface, i, j, alpha, alpha, alpha, alpha);
                }
            }
            SDL_UnlockSurface(surface);

            texture = SDL_CreateTextureFromSurface(m_sdlRenderer, surface);
            if (!texture) {
                printErr("Failed to create SDL_Texture: {}\n", SDL_GetError());
            }
            initted = true;
        }
    end:

        if (texture) {
            SDL_Rect src;
            src.w = m_fontBufferWidth;
            src.h = m_fontBufferHeight;
            src.x = 0;
            src.y = 0;
            SDL_Rect dst;
            dst.h = m_fontBufferWidth;
            dst.w = m_fontBufferHeight;
            dst.x = 0;
            dst.y = 0;
            SDL_RenderCopy(m_sdlRenderer, texture, &src, &dst);
        }

        SDL_RenderPresent(m_sdlRenderer);
    }

    void drawText(const char *text, int x, int y)
    {
        std::vector<GlyphData> glyphDatas;
        for (int ch = 0; text[ch]; ++ch) {
            const int pixelSize
                = m_currentFontSize == -1 ? DefaultFontPixelSize : m_currentFontSize;
            glyphDatas.emplace_back(getOrCreateGlyphData(text[ch], pixelSize));
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

private:
    void destroyFontCache() const noexcept
    {
        // SDL_Textures need to be destroyed manually
        for (auto &it : m_glyphDataMap) {
            auto &glyphData = it.second;
            if (glyphData.texture) {
                SDL_DestroyTexture(glyphData.texture);
            }
        }
    }

    void setSurfacePixelColor(
        SDL_Surface *surface, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) const
    {
        int off = y * surface->w + x;
        uint32_t *ptr = ((uint32_t *)surface->pixels) + off;
        uint32_t color = SDL_MapRGBA(surface->format, r, g, b, a);
        *ptr = color;
    }

    GlyphData createGlyphData(char character, int pixelSize) const noexcept
    {
        GlyphData result = {};

        float fontScale = stbtt_ScaleForPixelHeight(&m_currentFont, static_cast<float>(pixelSize));

        int w, h, xoff, yoff;
        uint8_t *bmp = stbtt_GetCodepointBitmap(
            &m_currentFont, fontScale, fontScale, character, &w, &h, &xoff, &yoff);
        if (!w || !h) {
            stbtt_FreeBitmap(bmp, 0);

            int advance, leftSideBearing;
            stbtt_GetCodepointHMetrics(&m_currentFont, character, &advance, &leftSideBearing);

            result.width = static_cast<int>(fontScale * advance);

            return result;
        }

        SDL_Surface *surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
        if (!surface) {
            printErr("Failed to create SDL_Surface: {}\n", SDL_GetError());
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
            printErr("Failed to create SDL_Texture: {}\n", SDL_GetError());
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

    GlyphData getOrCreateGlyphData(char character, int pixelSize) noexcept
    {
        GlyphData result;

        const auto create = [&]() {
            m_currentFontSize = pixelSize;
            result = createGlyphData(character, pixelSize);
            m_glyphDataMap[character] = result;
        };
        if (pixelSize != m_currentFontSize || m_currentFontSize == -1) {
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
};
