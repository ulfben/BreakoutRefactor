#pragma once
#include "pcg32.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include <cmath>
#include <numbers>
#include <optional>
#include <utility>

constexpr auto BALL_STARTING_X = 500.0f;
constexpr auto BALL_STARTING_Y = 500.0f;
constexpr auto BALL_STARTING_SPEED = 400.0f;
constexpr auto BALL_MAX_SPEED = 1000.0f;
constexpr auto PLAYER_STARTING_X = 500.0f;
constexpr auto PLAYER_STARTING_Y = 650.0f;
constexpr auto PLAYER_SPEED = 700.0f;
constexpr auto PLAYER_SPEEDUP = 100.0f;
static inline const auto PLAYER_SCALE = sf::Vector2f(1.0f, 0.5f);
static inline const auto PLAYER_STARTING_POS = sf::Vector2f(500.0f, 650.0f);
static inline const auto BG_COLOR = sf::Color{0x44, 0x55, 0x66, 0xff};
static constexpr auto TO_RAD = std::numbers::pi_v<float> / 180.0f;
static constexpr auto MAX_BOUNCE_ANGLE = 75.0f * TO_RAD;
struct StarConfig{
    sf::Color color;
    float scale;
    float baseSpeed;
    unsigned count;
};
inline static const StarConfig YELLOW_CONFIG{sf::Color(255, 255, 0), 0.5f, 50.0f, 4};
inline static const StarConfig RED_CONFIG{sf::Color(255, 0, 0), 0.3f, 14.0f, 3};
static constexpr float STAR_MARGIN_X = 50.0f;
static constexpr float STAR_STARTING_Y = -100.0f;
static constexpr float STAR_SPEED_VARIATION = 150.0f;

//don't be fooled. This is a global variable. Initialization is thread-safe, but 
// access (using the rng) is not. This is fine for our purposes.
static inline [[nodiscard]] PCG32& rng(){ 
    static PCG32 r{};
    return r;
};

static inline [[nodiscard]] std::optional<sf::FloatRect> get_overlap(const sf::Sprite& box1, const sf::Sprite& box2) noexcept{
    sf::FloatRect intersection;
    if(box1.getGlobalBounds().intersects(box2.getGlobalBounds(), intersection)){
        return intersection;
    }
    return std::nullopt;
}

static inline [[nodiscard]] bool is_colliding(const sf::Sprite& box1, const sf::Sprite& box2)  noexcept{
    return box1.getGlobalBounds().intersects(box2.getGlobalBounds());
}

static inline [[nodiscard]] bool is_colliding(const sf::Sprite& s, const sf::FloatRect& bounds)  noexcept{
    return s.getGlobalBounds().intersects(bounds);
}

static inline [[nodiscard]] bool is_inside(const sf::Sprite sprite, const sf::FloatRect& bound) noexcept{
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    return bound.contains(spriteBounds.left, spriteBounds.top) &&
        bound.contains(spriteBounds.left + spriteBounds.width, spriteBounds.top + spriteBounds.height);
}

static inline void constrainTo(sf::Sprite& sprite, const sf::FloatRect& bounds) noexcept{
    sf::Vector2f position = sprite.getPosition();
    sf::FloatRect paddleBounds = sprite.getGlobalBounds();
    position.x = std::clamp(position.x, bounds.left, bounds.left + bounds.width - paddleBounds.width);
    sprite.setPosition(position);
}

static inline sf::Vector2f normalize(const sf::Vector2f& source) noexcept{
    const float length = std::sqrt((source.x * source.x) + (source.y * source.y));
    if(length == 0){
        return source;
    }
    return sf::Vector2(source.x / length, source.y / length);
}