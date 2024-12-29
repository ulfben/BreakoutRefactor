#pragma once
#include <stdexcept>
#include <string_view>
#include <format>
#include <CodeAnalysis/warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "SFML/Graphics/Texture.hpp"
#pragma warning(pop)
class OwningTexture final{
    sf::Texture tex;
public: 
    explicit OwningTexture(std::string_view path){
        if(!tex.loadFromFile(path.data())){
            throw std::runtime_error(std::format("Failed to load texture: {}", path));
        }
    }
    const sf::Texture& get() const noexcept{
        return tex;
    }
};