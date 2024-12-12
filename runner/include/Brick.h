#pragma once

#include "batch.hpp"
#include <vector>
struct BrickParts
{
	sf::Sprite sprite;
	sf::Color color;
	float positionX, positionY;
};

class Brick
{
public:
	Brick();
	~Brick();
	void SetUp(const sf::Texture& texture);
	void Update();
	void Restart();
	std::vector<BrickParts> m_brickObject;
};