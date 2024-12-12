#include <stdexcept>
#include <string_view>
#include <format>
#include <SFML/Graphics/Font.hpp>
class OwningFont final{
    sf::Font font;
public: 
    explicit OwningFont(std::string_view path){
        if(!font.loadFromFile(path.data())){
            throw std::runtime_error(std::format("Failed to load font: {}", path));
        }
    }
    const sf::Font& get() const noexcept{
        return font;
    }
};