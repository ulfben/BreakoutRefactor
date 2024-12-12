#include "AssetsManagment.h"

std::vector<std::string> AssetsManagement::m_order;
std::unordered_map < std::string, sf::Text*> m_text;
bool AssetsManagement::LoadFontFile(const std::string& filePath)
{
    if(!m_font.loadFromFile(filePath))
    {
        //error
        return false;
    }
    return true;
}

sf::Text AssetsManagement::SetText(std::string textSentence, int size, sf::Uint32 textStyle, float positionX, float positionY)
{
    sf::Text text;
    //Have m_font as defualt font.
    text.setFont(m_font);
    text.setCharacterSize(size);
    text.setStyle(textStyle);
    text.setPosition(positionX, positionY);
    text.setString(textSentence);
    return text;
}