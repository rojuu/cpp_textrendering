#include "Renderer.hpp"
#include "SDL.h"
#include "Utils.hpp"

class App {
public:
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
    Renderer m_renderer { "rojueditor" };
};

int run()
{
    try {
        auto app = std::make_unique<App>();
        app->run();
    } catch (std::exception &e) {
        std::cerr << "Unexepcted error during runtime: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
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