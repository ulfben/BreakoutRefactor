#pragma once
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"

class PlayerCharacter
{
public:
	PlayerCharacter();
	~PlayerCharacter();
	void SetUp(const sf::Texture& texture, float max);
	void PlayerUpdate(float deltatime);
	void ProcessingInput(float deltatime);
	void WorldConstrainingOnPositionX();
	void Restart();
	sf::Sprite m_playerSprite;
	bool pressedLeft;
	bool pressedRight;
private:
	const float defualtMovmentSpeed;
	const float positionY;
	float positionX;
	float minPositionX, maxPositionX;
};