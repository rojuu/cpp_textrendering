#include "Renderer.hpp"
#include "SDL.h"
#include "Utils.hpp"

class App {
public:
    App();
    ~App() noexcept;

    App(const App &) = delete;
    App &operator=(const App &) = delete;
    App(App &&) = delete;
    App &operator=(App &&) = delete;

    void run();

private:
    static constexpr int WIDTH = 1280;
    static constexpr int HEIGHT = 720;

    static SDL_Window *initSDLWindow();

    SDL_Window *m_window { initSDLWindow() };
    Renderer m_renderer { m_window };
};

App::App() = default;
App::~App() noexcept
{
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void App::run()
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

SDL_Window *App::initSDLWindow()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error(fmt::format("Failed to init SDL {}\n", SDL_GetError()));
    }
    SDL_Window *window = SDL_CreateWindow(
        "rojuide", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);

    if (!window) {
        throw std::runtime_error(fmt::format("Failed to create window: {}\n", SDL_GetError()));
    }
    return window;
}

int run()
{
    try {
        const auto app = std::make_unique<App>();
        app->run();
    } catch (std::exception &e) {
        std::cerr << "Unexepcted error during runtime: " << e.what() << std::endl;
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