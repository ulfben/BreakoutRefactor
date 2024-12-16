#pragma once
#include <vector>
#include <algorithm>
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "OwningTexture.hpp"
#include "MyWindow.hpp"
#include <algorithm>
class Wall final{
    static constexpr auto BRICK_COUNT = 13;
    static constexpr auto BRICK_WIDTH = 100;
    std::vector<sf::Sprite> bricks;
    using Iterator = std::vector<sf::Sprite>::iterator;
public:
    explicit Wall(const OwningTexture& texture){
        float x = 0;
        float y = 100;
        for(int i = 0; i < BRICK_COUNT; i++){
            auto& sprite = bricks.emplace_back(texture.get());
            sprite.setPosition(x, y);
            sprite.setColor(sf::Color::Red);
            x += BRICK_WIDTH;
        }
    }

    sf::FloatRect getBounds() const noexcept{
        if(bricks.empty()){
            return sf::FloatRect();
        }
        const auto [minX, maxX] = std::minmax_element(bricks.begin(), bricks.end(),
            [](const auto& a, const auto& b){
                return a.getGlobalBounds().left < b.getGlobalBounds().left;
            });        
        const auto [minY, maxY] = std::minmax_element(bricks.begin(), bricks.end(),
            [](const auto& a, const auto& b){
                return a.getGlobalBounds().top < b.getGlobalBounds().top;
            });        
        const auto& minXBounds = minX->getGlobalBounds();               
        const auto& maxXBounds = maxX->getGlobalBounds();
        const auto& minYBounds = minY->getGlobalBounds();
        const auto& maxYBounds = maxY->getGlobalBounds();                
        return sf::FloatRect(
            minXBounds.left,
            minYBounds.top,
            (maxXBounds.left + maxXBounds.width) - minXBounds.left, // width
            (maxYBounds.top + maxYBounds.height) - minYBounds.top  // height
        );
    }

    [[nodiscard]] bool empty() const noexcept{
        return bricks.empty();
    }

    auto begin() noexcept{
        return bricks.begin();
    }
    auto end() noexcept{
        return bricks.end();
    }

    auto erase(const sf::Sprite& brick) noexcept{
        return std::erase_if(bricks, [&brick](const auto& b){
            return &b == &brick;
            });
    }

    void render(MyWindow& w) const noexcept{
        w.draw(std::span(bricks));
    }
};