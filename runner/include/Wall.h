#pragma once
#include <vector>
#include <algorithm>
#include <CodeAnalysis/warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#pragma warning(pop)
#include "OwningTexture.hpp"
#include "MyWindow.hpp"

class Wall final{
    static constexpr auto BRICK_COUNT = 13;
    static constexpr auto BRICK_WIDTH = 100;
    static constexpr auto WALL_START_Y = 100.0f; 
    std::vector<sf::Sprite> bricks;
    using Iterator = std::vector<sf::Sprite>::iterator;
public:
    explicit Wall(const OwningTexture& texture){
        float x = 0;
        for(int i = 0; i < BRICK_COUNT; i++){
            auto& sprite = bricks.emplace_back(texture.get());
            sprite.setPosition(x, WALL_START_Y);
            sprite.setColor(sf::Color::Red);
            x += BRICK_WIDTH;
        }
    }

    sf::FloatRect getBounds() const {
        if(bricks.empty()){
            return sf::FloatRect();
        }
        const auto [minX, maxX] = std::ranges::minmax_element(bricks,
            [](const auto& a, const auto& b){
                return a.getGlobalBounds().left < b.getGlobalBounds().left;
            });
        const auto [minY, maxY] = std::ranges::minmax_element(bricks,
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

    void render(MyWindow& w) const {
        w.draw(std::span(bricks));
    }
};