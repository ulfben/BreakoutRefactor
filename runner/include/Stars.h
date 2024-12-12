#pragma once
#pragma once
#include "SFML/Graphics/Sprite.hpp"
#include "OwningTexture.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include <vector>
class Stars{
    unsigned stageHeight;
    std::vector<sf::Sprite> stars;    
public:    
    Stars(const OwningTexture& texture, unsigned stageHeight) noexcept;      
    void update(float deltatime) noexcept;
    void render(sf::RenderWindow& window) const noexcept;
};