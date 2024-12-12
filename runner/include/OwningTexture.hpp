#pragma once
#include <stdexcept>
#include <string_view>
#include <format>
#include <SFML/Graphics/Texture.hpp>
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