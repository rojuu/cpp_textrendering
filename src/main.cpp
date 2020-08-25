#include "Common.hpp"
#include "Renderer.hpp"
#include "SDL.h"
#include "SDL_error.h"
#include "Utils.hpp"
#include <ostream>

class App {
public:
    bool init() { return m_renderer.init("rojueditor"); }
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
    Renderer m_renderer;
};

namespace {

App app;

} // namespace

int run()
{
    // TODO: Once we can be sure we're not using exceptions from 3rd party code (e.g. STL),
    // use -fno-exceptions and don't have this try-catch here
    try {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            fmt::print(stderr, "Failed to init SDL: {}\n", SDL_GetError());
            return false;
        }

        if (!app.init()) {
            fmt::print(stderr, "Failed to initialize App, exiting!\n");
            return EXIT_FAILURE;
        }
        app.run();

        SDL_Quit();
    } catch (std::exception &e) {
        fmt::print(stderr, "Unexepcted error during runtime: {}\n", e.what());
        return EXIT_FAILURE;
    }

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
