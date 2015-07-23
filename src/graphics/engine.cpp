#include <vector>
#include "debug.h"
#include "game/module.h"
#include "game/version.h"
#include "graphics/engine.h"
#include "utils/color.h"
#include "utils/string.h"

graphics::Engine::Engine(std::shared_ptr<graphics::ctx_t> ctx)
    : engine::Engine::Engine(ctx)
{
    DEBUG_DEBUG(L"initializing graphics engine\n");

    // Set default window properties
    std::wstring title = utils::string::cstr_to_wstr(game::NAME.c_str()) + L" " + game::VERSION;
    this->_window = std::make_shared<graphics::Window>(title, sf::VideoMode::getDesktopMode(),
            sf::Style::Default);

    DEBUG_DEBUG(L"Video Mode: %ls%d%lsx%ls%d%ls@%ls%d%ls\n",
            COLOR_WHITE, this->_window->video_mode().width, COLOR_END,
            COLOR_WHITE, this->_window->video_mode().height, COLOR_END,
            COLOR_WHITE, this->_window->video_mode().bitsPerPixel, COLOR_END);

    // Check if the video mode is compatible
    if (!this->_window->video_mode().isValid())
    {
        DEBUG_ERROR(L"video mode is invalid\n");
        return;
    }

    this->_initialized = true;

    DEBUG_DEBUG(L"graphics engine initialized\n");
}

graphics::Engine::~Engine()
{
    DEBUG_DEBUG(L"shutting down graphics engine\n");
    this->_initialized = false;
    DEBUG_DEBUG(L"graphics engine shut down successfully\n");
}

bool graphics::Engine::run(void)
{
    if (!this->_initialized)
    {
        DEBUG_ERROR(L"graphics engine is not initialized\n");
        return false;
    }

    DEBUG_DEBUG(L"running graphics engine\n");
    
    sf::RenderWindow window(this->_window->video_mode(), this->_window->title(),
            this->_window->style());

    this->_initialized = true;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape)
                        window.close(); // TODO abstract out to key manager that specific game registers for
                    break;
                default:
                    break;
            }
        }

        window.clear(sf::Color::Black);
        window.display();
    }

    return true;
}

bool graphics::Engine::set_window_size(unsigned int width, unsigned int height) const
{
    if (!this->_initialized)
    {
        DEBUG_ERROR(L"graphics engine is not initialized\n");
        return false;
    }

    auto video_mode = this->_window->video_mode();
    this->_window->set_video_mode(sf::VideoMode(width, height, video_mode.bitsPerPixel));
    return true;
}

