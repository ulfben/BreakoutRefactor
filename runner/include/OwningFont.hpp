#pragma once
#include <stdexcept>
#include <string_view>
#include <format>
#include <CodeAnalysis/warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "SFML/Graphics/Font.hpp"
#pragma warning(pop)
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