#include <vector>
#include "debug.h"
#include "graphics/engine.h"

graphics::Engine::Engine(graphics::ctx_t * ctx)
    : engine::Engine::Engine((engine::ctx_t *)ctx)
    , _window(NULL)
{
    std::vector<sf::VideoMode> fullscreen_modes = sf::VideoMode::getFullscreenModes();
    if (fullscreen_modes.empty())
    {
        DEBUG_ERROR(L"no fullscreen modes available\n");
        return;
    }

    this->_window = new sf::RenderWindow(fullscreen_modes[0], L"octo test", sf::Style::Fullscreen); // TODO temporary - allow specification from config file

    this->_initialized = true;
    DEBUG_INFO(L"graphics engine initialized\n");
}

graphics::Engine::~Engine()
{
    if (this->_window)
    {
        delete this->_window;
        this->_window = NULL;
    }
        
    DEBUG_INFO(L"graphics engine shut down successfully\n");
}

bool graphics::Engine::run(void)
{
    if (!this->_initialized)
    {
        DEBUG_ERROR(L"graphics engine is not initialized\n");
        return false;
    }

    DEBUG_INFO(L"running game engine\n");

    while (this->_window->isOpen())
    {
        sf::Event event;
        while (this->_window->pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    this->_window->close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape)
                        this->_window->close();
                    break;
                default:
                    break;
            }
        }

        this->_window->clear(sf::Color::Black);
        this->_window->display();
    }

    return true;
}

