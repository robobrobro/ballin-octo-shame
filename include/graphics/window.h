#ifndef __OCTO_GRAPHICS_WINDOW_H__
#define __OCTO_GRAPHICS_WINDOW_H__

#include <SFML/Graphics.hpp>
#include <string>

namespace graphics
{

class Window
{
    public:
        Window();
        Window(const std::wstring & title, const sf::VideoMode & video_mode);
        Window(const std::wstring & title, const sf::VideoMode & video_mode, int style);
        ~Window();

        std::wstring title() const { return _title; }
        sf::VideoMode video_mode() const { return _video_mode; }
        int style() const { return _style; }

        void set_title(const std::wstring & title) { _title = title; }
        void set_video_mode(sf::VideoMode video_mode) { _video_mode = video_mode; }
        void set_style(int style) { _style = style; }

    protected:
        std::wstring _title;
        sf::VideoMode _video_mode;
        int _style;
};

}   // namespace graphics

#endif

