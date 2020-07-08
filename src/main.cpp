#include "types.hpp"

#include "SDL.h"

#include "renderer.hpp"
#include <iostream>

class App {
    static constexpr int WIDTH = 1280;
    static constexpr int HEIGHT = 720;

    SDL_Window *m_window;
    Renderer *m_renderer;

public:
    bool init()
    {
        SDL_Init(SDL_INIT_VIDEO);
        m_window = SDL_CreateWindow("rojuide", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            WIDTH, HEIGHT, SDL_WINDOW_MOUSE_CAPTURE);

        if (!m_window) {
            std::cerr << "Failed to create window: " << SDL_GetError() << "\n";
            return false;
        }

        m_renderer = Renderer::createRenderer(m_window);
        if (!m_renderer) {
            std::cerr << "Failed to create renderer\n";
            return false;
        }

        return true;
    }

    void deinit()
    {
        Renderer::destroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    void run()
    {
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

            m_renderer->clear(91, 0, 176);

            m_renderer->drawText("Heljo world!", 24, 35, 200);
            m_renderer->drawText("Wassup world!", 24, 35, 250);
            m_renderer->drawText("Heljo world!", 32, 35, 350);
            m_renderer->drawText("Wassup world!", 32, 35, 400);
            m_renderer->drawText("Wazzzaaaa!", 32, 35, 500);
            m_renderer->drawText("Larger text?", 128, WIDTH / 2 - 100, HEIGHT / 2);

            m_renderer->present();
        }
    }
};

static App app;

int main(int argc, char **argv)
{
    if (!app.init()) {
        std::cerr << "Failed to init application\n";
        return EXIT_FAILURE;
    }
    app.run();
    app.deinit();
    return EXIT_SUCCESS;
}
