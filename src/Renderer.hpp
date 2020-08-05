class Renderer {
    SDL_Renderer *m_sdlRenderer;

    struct FontInfo {
        uint8_t *currentData;
        int currentSize = -1;
        std::string currentFileName;

        static constexpr int BufferPixelSize = 48;
        int bufferWidth;
        int bufferHeight;
        std::vector<uint8_t> pixels;
        std::vector<stbtt_bakedchar> charData;
        SDL_Texture *currentTexture;
    };
    FontInfo m_font;

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

        m_font.currentData = readEntireBinaryFile("Roboto-Regular.ttf");
        if (!m_font.currentData) {
            throw std::runtime_error("Failed to initialize SDL_Renderer");
        }

        m_font.bufferWidth = 512;
        m_font.bufferHeight = 512;
        m_font.pixels.reserve(m_font.bufferWidth * m_font.bufferHeight * sizeof(m_font.pixels[0]));
        m_font.charData.resize(512);
        // TODO: Use Improved 3D API (stbtt_PackBegin, etc, maybe even stb_rect_pack.h)
        const int bakeFontResult = stbtt_BakeFontBitmap(m_font.currentData, 0,
            FontInfo::BufferPixelSize, m_font.pixels.data(), m_font.bufferWidth,
            m_font.bufferHeight, 0, m_font.charData.size(), m_font.charData.data());

        if (bakeFontResult > 0) { // unused rows
        } else { // fit this many characters (abs)
            m_font.charData.resize(std::abs(bakeFontResult));
        }

        // Init font texture
        {
            int w = m_font.bufferWidth, h = m_font.bufferHeight;

            SDL_Surface *surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
            if (!surface) {
                throw std::runtime_error(
                    fmt::format("Failed to create SDL_Surface: {}\n", SDL_GetError()));
            }

            SDL_LockSurface(surface);
            for (int i = 0; i < surface->w && surface; ++i) {
                for (int j = 0; j < surface->h; ++j) {
                    uint8_t alpha = m_font.pixels[j * surface->w + i];
                    setSurfacePixelColor(surface, i, j, alpha, alpha, alpha, alpha);
                }
            }
            SDL_UnlockSurface(surface);

            m_font.currentTexture = SDL_CreateTextureFromSurface(m_sdlRenderer, surface);
            if (!m_font.currentTexture) {
                throw std::runtime_error(
                    fmt::format("Failed to create SDL_Texture: {}\n", SDL_GetError()));
            }

            SDL_SetTextureBlendMode(m_font.currentTexture, SDL_BLENDMODE_ADD);
        }
    }

    ~Renderer() noexcept
    {
        if (m_sdlRenderer) {
            SDL_DestroyRenderer(m_sdlRenderer);
        }
        freeBinaryFileContents(m_font.currentData);
    }

    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;
    Renderer(Renderer &&) = delete;
    Renderer &operator=(Renderer &&) = delete;

    void clear(uint8_t r, uint8_t g, uint8_t b) const noexcept
    {
        SDL_SetRenderDrawColor(m_sdlRenderer, r, g, b, 255);
        SDL_RenderClear(m_sdlRenderer);
    }

    void present() const noexcept { SDL_RenderPresent(m_sdlRenderer); }

    void drawText(const char *text, int x, int y, float scale = 1.f) noexcept
    {
#if 0
        SDL_Rect fontTexRect;
        fontTexRect.x = 720;
        fontTexRect.y = 70;
        fontTexRect.h = 512;
        fontTexRect.w = 512;
        SDL_RenderCopy(m_sdlRenderer, m_font.currentTexture, nullptr, &fontTexRect);
#endif

        for (int ch = 0; text[ch] != '\0'; ++ch) {
            const stbtt_bakedchar &bc = m_font.charData[text[ch]];

            const int w = bc.x1 - bc.x0;
            const int h = bc.y1 - bc.y0;
            SDL_Rect src { bc.x0, bc.y0, w, h };
            SDL_Rect dst {
                static_cast<int>(x + bc.xoff * scale),
                static_cast<int>(y + bc.yoff * scale),
                static_cast<int>(w * scale),
                static_cast<int>(h * scale),
            };
            SDL_RenderCopy(m_sdlRenderer, m_font.currentTexture, &src, &dst);

            x += bc.xadvance * scale;
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
