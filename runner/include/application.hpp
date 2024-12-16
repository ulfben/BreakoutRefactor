#pragma once
#include "Configs.hpp"
#include "Ball.h"
#include "Wall.h"
#include "Stars.h"
#include <string_view>
#include "Paddle.h"
#include "OwningTexture.hpp"
#include "OwningFont.hpp"
#include "MyWindow.hpp"
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
#include <optional>
#include "Highscore.hpp"

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
        Application() = default;
        void run() noexcept;

    private:
        enum class State{
            win, lose, running, pregame
        };  
        void input() noexcept;
        bool update() noexcept;
        void render() noexcept; //can't be const due to SFML Window API
        void check_collisions() noexcept;
        void do_score() noexcept;
        void restart() noexcept ;        
        void on_key_released(const sf::Keyboard::Key key) noexcept;
        

        Highscore highscore{"assets/HighScore.txt"};
        MyWindow window{"Breakout!", {1280, 720}, sf::Style::Titlebar | sf::Style::Close};
        OwningFont m_font{"assets/sunny-spells-font/SunnyspellsRegular-MV9ze.otf"};
        sf::Text startMenuText = createText("Press `space´ to start", m_font, 100u, sf::Text::Bold, 250, 250);
        sf::Text winText = createText("Winner", m_font, 50u, sf::Text::Bold, 550, 300);
        sf::Text loseText = createText("Game Over", m_font, 50u, sf::Text::Bold, 550, 300);
        sf::Text scoreText = createText("Score", m_font, 50u, sf::Text::Bold, 1100, 5);
        sf::Text highscoreText = createText("", m_font, 50u, sf::Text::Bold, 0, 5);
        OwningTexture playerTex{"assets/player.png"};
        OwningTexture ballTex{"assets/Ball.png"};
        OwningTexture brickTex{"assets/WhiteHitBrick.png"};
        OwningTexture starTex{"assets/FallingStar.png"};
        Stars stars{starTex, window.height()};
        Wall wall{brickTex};
        Paddle m_player{playerTex};
        Ball m_ball{ballTex};
        State m_state = State::pregame;
        sf::Clock m_clock;
        bool m_running = true;
        unsigned score = 0; 
    };
}