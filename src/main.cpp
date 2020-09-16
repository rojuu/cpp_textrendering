#include "Common.hpp"
#include "Renderer.hpp"
#include "SDL.h"
#include "SDL_error.h"
#include "Utils.hpp"

namespace {

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

} // namespace

class App {
    SDL_Window *m_sdlWindow { initSDLAndCreateWindow() };
    SDL_Renderer *m_sdlRenderer { createSDLRenderer(m_sdlWindow) };
    Renderer m_renderer { m_sdlRenderer };

public:
    App() = default;
    DELETE_COPYABLE_AND_MOVEABLE(App);

    ~App()
    {
        SDL_DestroyRenderer(m_sdlRenderer);
        SDL_DestroyWindow(m_sdlWindow);
        SDL_Quit();
    }

    void run()
    {
        std::string fileContents = readEntireTextFile("src/Renderer.hpp");

        bool quit = false;
        while (!quit) {
            SDL_Event sdlEvent;
            while (SDL_PollEvent(&sdlEvent)) {
                uint32_t type = sdlEvent.type;
                switch (type) {
                case SDL_QUIT: {
                    quit = true;
                } break;

                case SDL_KEYDOWN: {
                    if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
                        quit = true;
                    }
                } break;

                default:
                    break;
                }
            }

            m_renderer.clear({ 255, 255, 255, 255 });
            m_renderer.drawText(fileContents.c_str(), 10, 15, Color32 { 0, 0, 0, 255 });
            m_renderer.present();
        }
    }

private:
    static SDL_Window *initSDLAndCreateWindow()
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            throw std::runtime_error(fmt::format("Failed to init SDL: {}\n", SDL_GetError()));
        }

        SDL_Window *sdlWindow = SDL_CreateWindow("cpp_textrendering", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

        if (!sdlWindow) {
            throw std::runtime_error(fmt::format("Failed to create window: {}\n", SDL_GetError()));
        }

        return sdlWindow;
    }

    static SDL_Renderer *createSDLRenderer(SDL_Window *sdlWindow)
    {
        SDL_Renderer *sdlRenderer = SDL_CreateRenderer(
            sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!sdlRenderer) {
            throw std::runtime_error(
                fmt::format("Failed to initialize SDL_Renderer: {}\n", SDL_GetError()));
        }
        return sdlRenderer;
    }
};

int run()
{
    try {
        App app;
        app.run();
    } catch (...) {
        // Let's crash if we have an unhandled exception
        throw;
    }

    return EXIT_SUCCESS;
}

int main(int /*argc*/, char ** /*argv*/)
{
    return run();
}

#ifdef WIN32
#include <windows.h>
int WinMain(
    HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
    return run();
}
#endif
