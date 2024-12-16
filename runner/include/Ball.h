#pragma once
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"
#include "OwningTexture.hpp"
#include <cmath>

constexpr auto BALL_STARTING_X = 500.0f;
constexpr auto BALL_STARTING_Y = 500.0f;
constexpr auto BALL_STARTING_SPEED = 400.0f;
constexpr auto BALL_MAX_SPEED = 1000.0f;

static sf::Vector2f normalize(const sf::Vector2f& source) noexcept{
    const float length = std::sqrt((source.x * source.x) + (source.y * source.y));
    if(length == 0){
        return source;
    }
    return sf::Vector2(source.x / length, source.y / length);
}

class Ball final{
public:
    explicit Ball(const OwningTexture& texture) noexcept{      
        sprite.setTexture(texture.get());
        sprite.setPosition(BALL_STARTING_X, BALL_STARTING_Y);
    };

    void update(float deltatime){
        direction = normalize(direction);
        const auto vel = speed * deltatime;
        sprite.move(direction.x * vel, direction.y * vel);
    };

    bool isInside(sf::FloatRect bound) const noexcept{
        sf::FloatRect ballBounds = sprite.getGlobalBounds();
        return bound.contains(ballBounds.left, ballBounds.top) &&
            bound.contains(ballBounds.left + ballBounds.width, ballBounds.top + ballBounds.height);
    }

    sf::Sprite sprite;
    sf::Vector2f direction = sf::Vector2f(0.0f, -1.0f);
    float speed = BALL_STARTING_SPEED;
};