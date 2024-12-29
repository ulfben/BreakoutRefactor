#pragma once
#include "Configs.hpp"
#include <CodeAnalysis/warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"
#pragma warning(pop)
#include "OwningTexture.hpp"
#include "Paddle.h"
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
        w.draw(sprite);
    };

    float centerX() const noexcept{
        return sprite.getPosition().x + sprite.getGlobalBounds().width / 2.0f;
    }

    float top() const noexcept{
        return sprite.getGlobalBounds().top;
    }

    float height() const noexcept{
        return sprite.getGlobalBounds().height;
    }

    void checkCollisionWith(const Paddle& paddle) noexcept{
        if(!is_colliding(paddle.sprite, sprite)){
            return;
        }                                
        const float relativeIntersectX = (centerX() - paddle.centerX()) / (paddle.width() / 2.0f);
        const float normalizedIntersect = std::clamp(relativeIntersectX, -1.0f, 1.0f);        
        const float angle = normalizedIntersect * MAX_BOUNCE_ANGLE;        
        direction.x = std::sin(angle);
        direction.y = -std::abs(std::cos(angle));  // Always bounce upward        
        sprite.setPosition( // Ensure the ball is above the paddle
            sprite.getPosition().x,
            paddle.top() - height() - 1.0f
        );
    }

    bool checkCollisionWith(Wall& wall) noexcept{
        if(!is_colliding(sprite, wall.getBounds())){
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
            wall.erase(brick); //TODO: not happy about the ball erasing the brick, should be the wall's responsibility
            //consider an onCollision callback to let each entity handle their own collision responses.
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

    bool isBehind(const Paddle& other) const noexcept{
        return top() > other.bottom();
    }

private:
    sf::Sprite sprite;
    sf::Vector2f direction = sf::Vector2f(0.0f, -1.0f);
    float speed = BALL_STARTING_SPEED;
};
