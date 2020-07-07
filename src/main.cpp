#include "common.hpp"

#include "SDL.h"

#include "renderer.hpp"
#include <iostream>

class Main {
    static constexpr int WIDTH = 1280;
    static constexpr int HEIGHT = 720;

    SDL_Window *m_window;
    Renderer *m_renderer;

public:
    Main()
    {
        SDL_Init(SDL_INIT_VIDEO);
        m_window = SDL_CreateWindow("cplayground", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            WIDTH, HEIGHT, SDL_WINDOW_MOUSE_CAPTURE);

        if (!m_window) {
            std::cerr << "Failed to create window: ", SDL_GetError();
            exit(EXIT_FAILURE);
        }

        m_renderer = Renderer::createRenderer(m_window);
        if (!m_renderer) {
            std::cerr << "Failed to init renderer: ", SDL_GetError();
            exit(EXIT_FAILURE);
        }
    }

    ~Main()
    {
        Renderer::destroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    void run()
    {
        bool quit = false;
        while (!quit) {
            SDL_Event sdl_event;
            while (SDL_PollEvent(&sdl_event)) {
                uint32_t type = sdl_event.type;
                switch (type) {
                case SDL_QUIT: {
                    quit = true;
                } break;

                case SDL_KEYDOWN: {
                    if (sdl_event.key.keysym.sym == SDLK_ESCAPE) {
                        quit = true;
                    }
                } break;

                default:
                    break;
                }
            }

            m_renderer->clear(91, 0, 176);

            m_renderer->printText("Heljo world!", 24, 35, 200);
            m_renderer->printText("Wassup world!", 24, 35, 250);
            m_renderer->printText("Heljo world!", 32, 35, 350);
            m_renderer->printText("Wassup world!", 32, 35, 400);
            m_renderer->printText("Wazzzaaaa!", 32, 35, 500);
            m_renderer->printText("Larger text?", 128, WIDTH / 2 - 100, HEIGHT / 2);

            m_renderer->present();
        }
    }
};

static Main mainClass;

int main(int argc, char **argv)
{
    mainClass.run();
    return EXIT_SUCCESS;
}
