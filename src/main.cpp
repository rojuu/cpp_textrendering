#include <cstdint>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>

#include "Utils.hpp"
#include "Renderer.hpp"

#include "SDL.h"
#include <fmt/format.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

class App {
    static constexpr int WIDTH = 1280;
    static constexpr int HEIGHT = 720;

    SDL_Window *m_window { [] {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            throw std::runtime_error(fmt::format("Failed to init SDL {}\n", SDL_GetError()));
        }
        SDL_Window *window = SDL_CreateWindow(
            "rojuide", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);

        if (!window) {
            throw std::runtime_error(fmt::format("Failed to create window: {}\n", SDL_GetError()));
        }
        return window;
    }() };
    Renderer m_renderer { m_window };

public:
    App() = default;
    ~App() noexcept
    {
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    App(const App &) = delete;
    App &operator=(const App &) = delete;
    App(App &&) = delete;
    App &operator=(App &&) = delete;

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

            m_renderer.clear(91, 0, 176);

            m_renderer.drawText("Heljo world!", 35, 200, 2);
            m_renderer.drawText("Wassup world!", 35, 250, 0.35);
            m_renderer.drawText("Heljo world!", 35, 350, 0.82);
            m_renderer.drawText("Wassup world!", 35, 400, 0.83);
            m_renderer.drawText("Wazzzaaaa!", 35, 500, 0.15);
            m_renderer.drawText("Larger text?", WIDTH / 2 - 100, HEIGHT / 2, 5.331);

            m_renderer.present();
        }
    }
};

int main(int argc, char **argv)
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
