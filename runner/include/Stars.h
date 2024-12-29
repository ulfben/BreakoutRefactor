#pragma once
#pragma once
#include "Configs.hpp"
#include "MyWindow.hpp"
#include "OwningTexture.hpp"
#include <CodeAnalysis/warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System.hpp"
#pragma warning(pop)
#include <vector>

struct Star final{
    sf::Sprite sprite;
    float speed;
    Star(const OwningTexture& texture, const StarConfig& config, float xPos, float speed)
        : sprite(texture.get()), speed(speed){
        sprite.setColor(config.color);
        sprite.setScale(config.scale, config.scale);        
        sprite.setPosition(xPos, STAR_STARTING_Y);
    }

    void update(float dt) noexcept{
        sprite.move(0.0f, speed * dt);
    }

    void maybeWrap(float stageHeight, float stageWidth) noexcept{
        if(auto pos = sprite.getPosition(); pos.y < stageHeight){
            return;
        }
        float xpos = rng().between(STAR_MARGIN_X, stageWidth - STAR_MARGIN_X);
        sprite.setPosition(xpos, STAR_STARTING_Y);
    }
};

class Stars final{
    float stageWidth{};
    float stageHeight{};    
    std::vector<Star> stars;
    void initializeStars(const OwningTexture& texture, const StarConfig& config){
        for(unsigned i = 0; i < config.count; ++i){
            float speed = rng().between(config.baseSpeed, STAR_SPEED_VARIATION);
            float xpos = rng().between(STAR_MARGIN_X, stageWidth - STAR_MARGIN_X);
            stars.emplace_back(texture, config, xpos, speed);
        }
    }
public:
    Stars(const OwningTexture& texture, const MyWindow& window) noexcept
        : stageWidth(window.fwidth()), stageHeight(window.fheight()){
        initializeStars(texture, YELLOW_CONFIG);
        initializeStars(texture, RED_CONFIG);
    }

    void update(float deltatime) noexcept{
        for(auto& star : stars){
            star.update(deltatime);
            star.maybeWrap(stageHeight, stageWidth);            
        }
    }

    void render(MyWindow& window) const noexcept{
        for(const auto& star : stars){
            window.draw(star.sprite);
        }
    }
};