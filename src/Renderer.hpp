class Renderer {
    SDL_Renderer *m_sdlRenderer;
    stbtt_fontinfo m_currentFont;
    uint8_t *m_currentFontData;
    int m_currentFontSize = -1;
    std::string m_currentFontFile;

    static constexpr int BufferFontPixelSize = 48;
    int m_fontBufferWidth;
    int m_fontBufferHeight;
    std::vector<uint8_t> m_pixels;
    std::vector<stbtt_bakedchar> m_charData;
    SDL_Texture *m_currentFontTexture;

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

        // Init font texture
        {
            int w = m_fontBufferWidth, h = m_fontBufferHeight;

            SDL_Surface *surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
            if (!surface) {
                throw std::runtime_error(
                    fmt::format("Failed to create SDL_Surface: {}\n", SDL_GetError()));
            }

            SDL_LockSurface(surface);
            for (int i = 0; i < surface->w && surface; ++i) {
                for (int j = 0; j < surface->h; ++j) {
                    uint8_t alpha = m_pixels[j * surface->w + i];
                    setSurfacePixelColor(surface, i, j, alpha, alpha, alpha, alpha);
                }
            }
            SDL_UnlockSurface(surface);

            m_currentFontTexture = SDL_CreateTextureFromSurface(m_sdlRenderer, surface);
            if (!m_currentFontTexture) {
                throw std::runtime_error(
                    fmt::format("Failed to create SDL_Texture: {}\n", SDL_GetError()));
            }
        }
    }

    ~Renderer() noexcept
    {
        if (m_sdlRenderer) {
            SDL_DestroyRenderer(m_sdlRenderer);
        }
        freeBinaryFileContents(m_currentFontData);
    }

    Renderer(const Renderer &) = delete;
    auto operator=(const Renderer &) -> Renderer & = delete;
    Renderer(Renderer &&) = delete;
    auto operator=(Renderer &&) -> Renderer & = delete;

    void clear(uint8_t r, uint8_t g, uint8_t b) const noexcept
    {
        SDL_SetRenderDrawColor(m_sdlRenderer, r, g, b, 255);
        SDL_RenderClear(m_sdlRenderer);
    }

    void present() const noexcept { SDL_RenderPresent(m_sdlRenderer); }

    void drawText(const char *text, int x, int y) noexcept
    {
        for (int ch = 0; text[ch] != '\0'; ++ch) {
            const float fontScale = stbtt_ScaleForPixelHeight(&m_currentFont, BufferFontPixelSize);

            int advance, leftSideBearing;
            stbtt_GetCodepointHMetrics(&m_currentFont, text[ch], &advance, &leftSideBearing);

            float xamt = static_cast<int>(fontScale * advance);

            int x0, y0, x1, y1;
            stbtt_GetCodepointBox(&m_currentFont, text[ch], &x0, &y0, &x1, &y1);

            int w = x1 - x0;
            int h = y1 - y0;

            print("ch={}, x0={}, y0={}, x1={}, y1={}, advance={}, leftSideBearing={}\n", text[ch],
                x0, y0, x1, y1, advance, leftSideBearing);

            SDL_Rect src;
            src.x = x0;
            src.y = y0;
            src.w = w;
            src.h = h;

            SDL_Rect dst;
            dst.w = w * fontScale;
            dst.h = h * fontScale;
            dst.x = x + xamt;
            dst.y = y;

            // SDL_SetRenderDrawBlendMode(m_sdlRenderer, SDL_BLENDMODE_ADD);
            SDL_RenderCopy(m_sdlRenderer, m_currentFontTexture, &src, &dst);

            x += xamt;
        }
    }

private:
    static void setSurfacePixelColor(
        SDL_Surface *surface, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept
    {
        int off = y * surface->w + x;
        uint32_t *ptr = ((uint32_t *)surface->pixels) + off;
        uint32_t color = SDL_MapRGBA(surface->format, r, g, b, a);
        *ptr = color;
    }
};
