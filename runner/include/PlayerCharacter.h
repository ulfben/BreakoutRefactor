#pragma once
#include "SFML/Graphics/Sprite.hpp"
#include "OwningTexture.hpp"
#include "SFML/System/Vector2.hpp"

class PlayerCharacter
{
public:
	PlayerCharacter(const OwningTexture& texture, unsigned max);	
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