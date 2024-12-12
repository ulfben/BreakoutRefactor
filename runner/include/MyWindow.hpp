#pragma once
#include <stdexcept>
#include <string_view>
#include <format>
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
    auto width() const noexcept{
        return w.getSize().x;
    }
    auto height() const noexcept{
        return w.getSize().y;
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

        