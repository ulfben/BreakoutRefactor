#pragma once
#include <vector>
#include <algorithm>
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "OwningTexture.hpp"
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

    void render(sf::RenderWindow& w) const noexcept{
        for(auto& sprite : bricks){
            w.draw(sprite);
        }
    }
};