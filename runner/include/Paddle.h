#pragma once
#include "Configs.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "OwningTexture.hpp"
#include "SFML/System/Vector2.hpp"
#include "MyWindow.hpp"

class Paddle final{
public:
    explicit Paddle(const OwningTexture& texture) noexcept{
        sprite.setTexture(texture.get());
        sprite.setPosition(PLAYER_STARTING_POS);
        sprite.setScale(PLAYER_SCALE);
    };
    void update(float deltatime) noexcept{
        float dir = pressedLeft ? -PLAYER_SPEED : (pressedRight ? PLAYER_SPEED : 0.0f);
        sprite.move(dir * deltatime, 0.0f);
    }
    void render(MyWindow& w) const noexcept{
        w.draw(sprite);
    };
    void constrainTo(const sf::FloatRect& bounds) noexcept{
        sf::Vector2f position = sprite.getPosition();  
        sf::FloatRect paddleBounds = sprite.getGlobalBounds();        
        if(position.x < bounds.left){
            position.x = bounds.left;
        } else if(position.x + paddleBounds.width > bounds.left + bounds.width){
            position.x = bounds.left + bounds.width - paddleBounds.width;
        }        
        sprite.setPosition(position);
    }
    sf::Sprite sprite;
    bool pressedLeft = false;
    bool pressedRight = false;
};