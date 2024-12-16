#pragma once
#include "Configs.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"
#include "OwningTexture.hpp"
#include "MyWindow.hpp"
#include "Wall.h"

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
    void render(MyWindow& w) const noexcept{
        w.get().draw(sprite);
    };

    void checkCollisionWith(const sf::Sprite& other) noexcept{
        if(!is_colliding(other, sprite)){
            return;
        }
        const float ball_x = sprite.getPosition().x;
        const float paddleHalfWidth = sprite.getGlobalBounds().width / 2.0f;
        const float paddleLeft = other.getGlobalBounds().left;
        const float relativeIntersectX = std::clamp((ball_x - paddleLeft - paddleHalfWidth) / paddleHalfWidth, -1.0f, 1.0f);
        const float angle = relativeIntersectX * MAX_BOUNCE_ANGLE;
        const float vel = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        direction = sf::Vector2f(vel * std::sin(angle), -vel * std::cos(angle));
        sprite.setPosition(ball_x, other.getGlobalBounds().top - sprite.getGlobalBounds().height / 2); //move the ball out of the paddle    
    }

    bool checkCollisionWith(Wall& wall) noexcept{
        if(!is_overlapping(sprite, wall.getBounds())){
            return false;
        }
        for(const auto& brick : wall){
            const auto intersection = get_overlap(brick, sprite);
            if(!intersection){
                continue;
            }
            const bool isVerticalHit = intersection->width > intersection->height;
            const float displacement = isVerticalHit ? intersection->height : intersection->width;
            const float sign = (isVerticalHit ? direction.y : direction.x) < 0 ? 1.0f : -1.0f;

            if(isVerticalHit){
                direction.y *= -1.0f;
                sprite.move(0, sign * displacement);
            } else{
                direction.x *= -1.0f;
                sprite.move(sign * displacement, 0);
            }

            speed += PLAYER_SPEEDUP;
            wall.erase(brick);
            return true;
        }
        return false;
    }

    void constrainTo(const sf::FloatRect bounds) noexcept{
        const auto ballBounds = sprite.getGlobalBounds();
        const bool hitLeft = ballBounds.left < bounds.left;
        const bool hitRight = ballBounds.left + ballBounds.width > bounds.left + bounds.width;
        if(hitLeft || hitRight){
            direction.x = (hitLeft ? 1.0f : -1.0f) * std::abs(direction.x);
            const float newX = hitLeft ? bounds.left : bounds.left + bounds.width - ballBounds.width;
            sprite.setPosition(newX, ballBounds.top);
            return;
        }
        if(ballBounds.top < bounds.top){
            direction.y = std::abs(direction.y);
            sprite.setPosition(ballBounds.left, bounds.top);
        }
    }
    bool isBehind(const sf::Sprite& other) const noexcept{
        return sprite.getGlobalBounds().top > other.getGlobalBounds().top + other.getGlobalBounds().height;
    }

private:
    bool isInside(sf::FloatRect bound) const noexcept{
        sf::FloatRect ballBounds = sprite.getGlobalBounds();
        return bound.contains(ballBounds.left, ballBounds.top) &&
            bound.contains(ballBounds.left + ballBounds.width, ballBounds.top + ballBounds.height);
    }

    sf::Sprite sprite;
    sf::Vector2f direction = sf::Vector2f(0.0f, -1.0f);
    float speed = BALL_STARTING_SPEED;
};