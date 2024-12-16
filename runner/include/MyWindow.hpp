#pragma once
#include <stdexcept>
#include <string_view>
#include <format>
#include <span>
#include <SFML/Graphics/RenderWindow.hpp>
class MyWindow final{
    sf::RenderWindow w;

public:
    explicit MyWindow(std::string_view title, sf::VideoMode resolution, sf::Uint32 flags){
        w.create(resolution, title.data(), flags);
        if(!w.isOpen()){
            throw std::runtime_error("Failed to create window");
        }
        w.setKeyRepeatEnabled(false);
    }
    auto& get() noexcept{
        return w;
    }
    void clear(sf::Color c) noexcept{
        w.clear(c);
    }
    void draw(const sf::Drawable& o) noexcept{
        w.draw(o);
    }
    template<std::derived_from<sf::Drawable> T>
    void draw(std::span<const T> objects) noexcept{
        for(const auto& i : objects){
            w.draw(i);
        }
    }
    void display() noexcept{
        w.display();
    }
    auto width() const noexcept{
        return w.getSize().x;
    }
    auto height() const noexcept{
        return w.getSize().y;
    }
    float fwidth() const noexcept{
        return static_cast<float>(w.getSize().x);
    }
    float fheight() const noexcept{
        return static_cast<float>(w.getSize().y);
    }
    auto isOpen() const noexcept{
        return w.isOpen();
    }
    void close() noexcept{
        w.close();
    }
    bool pollEvent(sf::Event& event) noexcept{
        return w.pollEvent(event);
    }
};

