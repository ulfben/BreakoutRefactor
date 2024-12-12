#pragma once
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include <vector>
class Stars{
    unsigned stageHeight;
    std::vector<sf::Sprite> stars;    
public:
    Stars(){};
    Stars(const sf::Texture& texture, unsigned stageHeight);      
    void update(float deltatime) noexcept;
    void render(sf::RenderWindow& window) noexcept;
};