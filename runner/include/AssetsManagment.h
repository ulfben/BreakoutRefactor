#pragma once
#include <unordered_map>
#include "batch.hpp"
class AssetsManagement
{
// AssetsManagment uppgift är att äga och hantera resourscer.
public:	
	static std::vector<std::string> m_order;
	sf::Font m_font;
	bool LoadFontFile(const std::string& filePath);
	// It set the m_font by defualt. Need to use LoadFontFile first before using SetText;
	sf::Text SetText(std::string textSentence, int size, sf::Uint32 textStyle, float positionX, float positionY);	
};