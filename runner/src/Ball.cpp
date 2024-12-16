#include "Ball.h"
#include <cmath>
Ball::Ball(const OwningTexture& texture, unsigned rectWidth, unsigned rectHeight){
    m_speed = 400.0f;
    positionX = 500.0f;
    positionY = 400.0f;
    m_direction.x = positionX;
    m_direction.y = positionY;
    m_ballSprite.setTexture(texture.get());
    m_ballSprite.setPosition(positionX, positionY);
    m_ballSprite.setScale(1.0f, 1.0f);
    worldBounds.width = rectWidth;
    worldBounds.height = rectHeight;
    worldBounds.left = 0;
    worldBounds.top = 0;
};

void Ball::BallUpdate(float deltatime){
    WorldConstraining(positionX, positionY);
    m_direction = Normalized(m_direction);
    positionX += m_direction.x * m_speed * deltatime;
    positionY += m_direction.y * m_speed * deltatime;
    m_ballSprite.setPosition(positionX, positionY);
};

float Ball::Length(const sf::Vector2f& rhs){
    return std::sqrtf(rhs.x * rhs.x + rhs.y * rhs.y);
};

sf::Vector2f Ball::Normalized(const sf::Vector2f& rhs){
    float length = 1.0f / Length(rhs);
    float x = rhs.x * length;
    float y = rhs.y * length;
    return sf::Vector2f{x, y};
}

void Ball::WorldConstraining(float posX, float posY){
    if(posX < (float) worldBounds.left){
        m_direction.x = -m_direction.x;
    }
    if(posX >= (float) worldBounds.width - 50){
        m_direction.x = -m_direction.x;
    }
    if(posY < (float) worldBounds.top){
        m_direction.y = -m_direction.y;
    }
    /*if (posY >= (float)worldBounds.height -50)
    {
      m_direction.y = -m_direction.y;
    }*/

}

void Ball::Restart(){
    m_speed = 400.0f;
    positionX = 500.0f;
    positionY = 400.0f;
    m_direction.x = positionX;
    m_direction.y = positionY;
}