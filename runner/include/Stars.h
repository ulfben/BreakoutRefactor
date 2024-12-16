#pragma once
#pragma once
#include "SFML/Graphics/Sprite.hpp"
#include "OwningTexture.hpp"
#include "MyWindow.hpp"
#include <vector>

class Stars final{
    unsigned stageHeight;
    std::vector<sf::Sprite> stars;
public:
    static constexpr auto YELLOW_STAR_COUNT = 4;
    static constexpr auto YELLOW_SPEED = 25;
    static constexpr auto YELLOW_SCALE = 0.5f;
    static constexpr auto RED_STAR_COUNT = 3;
    static constexpr auto RED_SPEED = 7;
    static constexpr auto RED_SCALE = 0.3f;
    static constexpr auto FALLING_SPEED = 225;

    Stars(const OwningTexture& texture, unsigned stageHeight) noexcept{
        this->stageHeight = stageHeight;
        float x = 100;
        float y = -100;
        //TODO: 4 yellow, 3 red stars, different speeds for different colors, and randomize x and falling speed!
        // ensure we can do inline initialization of all stars so the constructor can be removed.
        for(int i = 0; i < YELLOW_STAR_COUNT; i++){
            auto& sprite = stars.emplace_back(texture.get());
            sprite.setColor(sf::Color::Yellow);
            sprite.setPosition(x, y);
            sprite.setScale(YELLOW_SCALE, YELLOW_SCALE);
            x += 100.0f;
        }
        y = -150;
        for(int i = 0; i < RED_STAR_COUNT; i++){
            auto& sprite = stars.emplace_back(texture.get());
            sprite.setColor(sf::Color::Red);
            sprite.setPosition(x, y);
            sprite.setScale(RED_SCALE, RED_SCALE);
            x += 100.0f;
        }
    }

    void update(float deltatime) noexcept{
        for(auto& sprite : stars){
            auto pos = sprite.getPosition();
            pos.y += YELLOW_SPEED * deltatime;
            if(pos.y > static_cast<float>(stageHeight)){
                pos.y = -100;
            }
            sprite.setPosition(pos);
        }
    }

    void render(MyWindow& window) const noexcept{
        window.draw(std::span(stars));
    }
};