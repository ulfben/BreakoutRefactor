#pragma once
#include "Configs.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "OwningTexture.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "MyWindow.hpp"

class Paddle final{
public:
    explicit Paddle(const OwningTexture& texture) noexcept{
        sprite.setTexture(texture.get());
        sprite.setPosition(PLAYER_STARTING_POS);
        sprite.setScale(PLAYER_SCALE);
    };
    void update(float deltatime) noexcept{
        const auto pressingLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        const auto pressingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);       
        float dir = pressingLeft ? -PLAYER_SPEED : (pressingRight ? PLAYER_SPEED : 0.0f);
        sprite.move(dir * deltatime, 0.0f);
    }
    void render(MyWindow& w) const noexcept{
        w.draw(sprite);
    }
    float centerX() const noexcept{
        return sprite.getPosition().x + sprite.getGlobalBounds().width / 2.0f;
    }
    float top() const noexcept{
        return sprite.getGlobalBounds().top;
    }
    float bottom() const noexcept{
        return sprite.getPosition().y + sprite.getGlobalBounds().height;
    }
    float width() const noexcept{
        return sprite.getGlobalBounds().width;
    }
    void constrainTo(const sf::FloatRect& bounds) noexcept{
        sf::Vector2f position = sprite.getPosition();  
        sf::FloatRect paddleBounds = sprite.getGlobalBounds();        
        position.x = std::clamp(position.x, bounds.left, bounds.left + bounds.width - paddleBounds.width);
        sprite.setPosition(position);
    }
    sf::Sprite sprite;
};