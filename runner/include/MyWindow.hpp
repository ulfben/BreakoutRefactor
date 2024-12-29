#pragma once
#include <stdexcept>
#include <string_view>
#include <format>
#include <span>
#include <CodeAnalysis/warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "SFML/Graphics/RenderWindow.hpp"
#pragma warning(pop)   
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
    void clear(sf::Color c){
        w.clear(c);
    }
    void draw(const sf::Drawable& o){
        w.draw(o);
    }
    template<std::derived_from<sf::Drawable> T>
    void draw(std::span<const T> objects) {
        for(const auto& i : objects){
            w.draw(i);
        }
    }
    void display() {
        w.display();
    }
    auto width() const {
        return w.getSize().x;
    }
    auto height() const {
        return w.getSize().y;
    }
    float fwidth() const {
        return static_cast<float>(w.getSize().x);
    }
    float fheight() const {
        return static_cast<float>(w.getSize().y);
    }
    auto isOpen() const {
        return w.isOpen();
    }
    void close() {
        w.close();
    }
    bool pollEvent(sf::Event& event) {
        return w.pollEvent(event);
    }
};

