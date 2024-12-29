#pragma once
#include "Configs.hpp"
#include "OwningTexture.hpp"
#include <CodeAnalysis/warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#pragma warning(pop)
#include "MyWindow.hpp"

class Paddle final{
public:
    explicit Paddle(const OwningTexture& texture) {
        sprite.setTexture(texture.get());
        sprite.setPosition(PLAYER_STARTING_POS);
        sprite.setScale(PLAYER_SCALE);
    };
    void update(float deltatime) {
        const auto pressingLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        const auto pressingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);       
        const float dir = pressingLeft ? -PLAYER_SPEED : (pressingRight ? PLAYER_SPEED : 0.0f);
        sprite.move(dir * deltatime, 0.0f);
    }
    void render(MyWindow& w) const {
        w.draw(sprite);
    }
    float centerX() const {
        return sprite.getPosition().x + sprite.getGlobalBounds().width / 2.0f;
    }
    float top() const {
        return sprite.getGlobalBounds().top;
    }
    float bottom() const {
        return sprite.getPosition().y + sprite.getGlobalBounds().height;
    }
    float width() const {
        return sprite.getGlobalBounds().width;
    }
    void constrainTo(const sf::FloatRect& bounds) {
        sf::Vector2f position = sprite.getPosition();  
        const sf::FloatRect paddleBounds = sprite.getGlobalBounds();        
        position.x = std::clamp(position.x, bounds.left, bounds.left + bounds.width - paddleBounds.width);
        sprite.setPosition(position);
    }
    sf::Sprite sprite;
};