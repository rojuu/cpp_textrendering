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
public:
    App() = default;

    bool init()
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            LOG_ERR() << fmt::format("Failed to init SDL: {}\n", SDL_GetError());
            return false;
        }

        m_sdlWindow = SDL_CreateWindow("rojueditor", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

        if (!m_sdlWindow) {
            LOG_ERR() << fmt::format("Failed to create window: {}\n", SDL_GetError());
            return false;
        }

        m_sdlRenderer = SDL_CreateRenderer(
            m_sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!m_sdlRenderer) {
            LOG_ERR() << fmt::format("Failed to initialize SDL_Renderer: {}\n", SDL_GetError());
            return false;
        }

        if (!m_renderer.init(m_sdlRenderer)) {
            LOG_ERR() << fmt::format("Failed to init Renderer\n");
            return false;
        }

        return true;
    }

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

            m_renderer.clear(255, 255, 255);
            m_renderer.drawText(fileContents.c_str(), 10, 15, Color32 { 0, 0, 0, 255 });
            m_renderer.present();
        }
    }

private:
    SDL_Window *m_sdlWindow { nullptr };
    SDL_Renderer *m_sdlRenderer { nullptr };
    Renderer m_renderer;
};

int run()
{
    App app;
    if (!app.init()) {
        LOG_ERR() << fmt::format("Failed to initialize App, exiting!\n");
        return EXIT_FAILURE;
    }
    app.run();

    return EXIT_SUCCESS;
}

int main(int /*argc*/, char ** /*argv*/)
{
    return run();
}

#ifdef WIN32
#include <windows.h>
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    return run();
}
#endif
