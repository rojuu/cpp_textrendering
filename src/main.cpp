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

            m_renderer.clear(0, 0, 0);
            m_renderer.drawText(fileContents.c_str(), 10, 15);
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
            std::cerr << "Failed to init SDL: " << SDL_GetError()
                      << std::endl; // intentionally flush with std::endl here, as we are exiting
            return false;
        }

        if (!app.init()) {
            std::cerr << "Failed to initialize App, exiting!"
                      << std::endl; // intentionally flush with std::endl here, as we are exiting
            return EXIT_FAILURE;
        }
        app.run();

        SDL_Quit();
    } catch (std::exception &e) {
        std::cerr << "Unexepcted error during runtime: " << e.what()
                  << std::endl; // intentionally flush with std::endl here, as we are exiting
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
