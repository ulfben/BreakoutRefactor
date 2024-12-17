#pragma once
#pragma once
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System.hpp"
#include "OwningTexture.hpp"
#include "MyWindow.hpp"
#include <vector>
#include "pcg32.hpp"

struct Star{
    sf::Sprite sprite;
    float speed;
    Star(const OwningTexture& texture, const StarConfig& config, float xPos, float speed)
        : sprite(texture.get()), speed(speed){
        sprite.setColor(config.color);
        sprite.setScale(config.scale, config.scale);
        sprite.setPosition(xPos, STAR_STARTING_Y);
    }
};
class Stars final{
    unsigned stageHeight{};
    PCG32 rng{};
    std::vector<Star> stars;

    void initializeStars(const OwningTexture& texture, const StarConfig& config){        
        for(unsigned i = 0; i < config.count; ++i){
            stars.emplace_back(texture, config,
                rng.between(0.0f, 600.0f),
                rng.between(25, STAR_SPEED_VARIATION)
            );
        }
    }
public:
    Stars(const OwningTexture& texture, unsigned stageHeight) noexcept
        : stageHeight(stageHeight){
        initializeStars(texture, YELLOW_CONFIG);
        initializeStars(texture, RED_CONFIG);
    }

    void update(float deltatime) noexcept{
        for(auto& star : stars){
            auto& sprite = star.sprite;
            auto pos = sprite.getPosition();
            pos.y += star.speed * deltatime;
            if(pos.y > static_cast<float>(stageHeight)){
                pos.y = STAR_STARTING_Y;
                pos.x = rng.between(0.0f, 600.0f); 
            }
            sprite.setPosition(pos);
        }
    }

    void render(MyWindow& window) const noexcept{
        for (const auto& star : stars) {
            window.draw(star.sprite);
        }
    }
};