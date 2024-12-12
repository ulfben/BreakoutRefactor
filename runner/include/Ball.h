#pragma once
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"
#include "OwningTexture.hpp"

class Ball final
{
public:
	Ball(const OwningTexture& texture, unsigned rectWidth, unsigned rectHeight);	
	void BallUpdate(float deltatime);
	float Length(const sf::Vector2f& rhs);
	void WorldConstraining(float posX, float posY);
	void Restart();
	sf::IntRect worldBounds;
	sf::Vector2f Normalized(const sf::Vector2f& rhs);
	sf::Sprite m_ballSprite;
	sf::Vector2f m_direction;	
	float m_speed;
private:
	float positionX, positionY;
};