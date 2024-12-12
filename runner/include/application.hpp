#pragma once
#include "Ball.h"
#include "Brick.h"
#include "Stars.h"
#include <string_view>
#include "PlayerCharacter.h"
#include "OwningTexture.hpp"
#include "OwningFont.hpp"
#include "SFML/Graphics/Text.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/Font.hpp>
#include <string>

namespace runner {
     static inline sf::Text createText(std::string_view s, const OwningFont& font, unsigned size, sf::Uint32 textStyle, float positionX, float positionY){
        sf::Text text;
        text.setFont(font.get());
        text.setCharacterSize(size);
        text.setStyle(textStyle);
        text.setPosition(positionX, positionY);
        text.setString(s.data());
        return text;
    }

    class Application final{
    public:
        Application();
        void run();

    private:
        enum class State{
            win, lose, running, pregame
        };
        enum class TextureName{
            Texture_player, Texture_ball, Texture_brick
        };
        void input() noexcept;
        bool update();
        void render() noexcept; //can't be const due to SFML Window API
        void CollisionCheck();
        void doScore();
        void Restart();
        void on_key_pressed(const sf::Keyboard::Key key);
        void on_key_released(const sf::Keyboard::Key key);
        void loadHighScore();
        void StoreHighScore();

        [[nodiscard]] bool AxisAlignedBoundingBox(sf::Sprite& box1, sf::Sprite& box2);

        sf::RenderWindow window;        
        bool             m_running = true;
        int m_highScoreInt = 0;
        int m_currentScore = 0;        
        sf::Clock        m_clock;                
        PlayerCharacter  m_player;
        Ball             m_ball;
        Brick            m_brick;        
        State   m_state = State::pregame;
        Stars stars;
        OwningFont m_font{"assets/sunny-spells-font/SunnyspellsRegular-MV9ze.otf"};
        sf::Text startMenuText = createText("Press `space´ to start", m_font, 100u, sf::Text::Bold, 250, 250);
        sf::Text winText = createText("Winner", m_font, 50u, sf::Text::Bold, 550, 300);
        sf::Text loseText = createText("Game Over", m_font, 50u, sf::Text::Bold, 550, 300);
        sf::Text scoreText = createText("Score", m_font, 50u, sf::Text::Bold, 1100, 5);
        sf::Text highscoreText = createText("", m_font, 50u, sf::Text::Bold, 0, 5);

        OwningTexture _player{"assets/player.png"};
        OwningTexture _ball{"assets/Ball.png"};
        OwningTexture _brick{"assets/WhiteHitBrick.png"};
        OwningTexture _star{"assets/FallingStar.png"};
    };
}