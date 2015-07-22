#include "debug.h"
#include "graphics/window.h"

graphics::Window::Window()
    : _style(sf::Style::Default)
{
}

graphics::Window::Window(const std::wstring & title, const sf::VideoMode & video_mode)
    : _title(title)
    , _video_mode(video_mode)
    , _style(sf::Style::Default)
{
}

graphics::Window::Window(const std::wstring & title, const sf::VideoMode & video_mode, int style)
    : _title(title)
    , _video_mode(video_mode)
    , _style(style)
{
}

graphics::Window::~Window()
{
}

