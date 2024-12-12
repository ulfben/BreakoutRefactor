#pragma once
#include <vector>
#include "SFML/Graphics/Sprite.hpp"
#include "OwningTexture.hpp"

struct BrickParts
{
	sf::Sprite sprite;
	sf::Color color;
	float positionX, positionY;
};

class Brick final
{
public:
	explicit Brick(const OwningTexture& texture);			
	void Restart();
	std::vector<BrickParts> m_brickObject;
};