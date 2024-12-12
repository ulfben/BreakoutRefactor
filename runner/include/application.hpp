#pragma once
#include "Ball.h"
#include "batch.hpp"
#include "Brick.h"
#include "ParallaxBackground.h"
#include "PlayerCharacter.h"
#include "SFML/Graphics/Text.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <string>

namespace runner {
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
        void render();
        void CollisionCheck();
        void doScore();
        void Restart();
        void on_key_pressed(const sf::Keyboard::Key key);
        void on_key_released(const sf::Keyboard::Key key);
        void loadHighScore();
        void StoreHighScore();

        [[nodiscard]] bool AxisAlignedBoundingBox(sf::Sprite& box1, sf::Sprite& box2);

        sf::RenderWindow m_window;
        PrimitiveBatch   m_batch;
        bool             m_running = true;
        sf::Clock        m_clock;
        sf::Time         m_deltatime;
        sf::Sprite       m_sprite;
        PlayerCharacter  m_player;
        Ball             m_ball;
        Brick            m_brick;        
        State   m_state = State::pregame;
        parallaxBackground m_parallaxBackground;
        sf::Font m_font;
        sf::Text m_startMainuText;
        sf::Text m_WinText;
        sf::Text m_LoseText;
        sf::Text m_ScoreText;
        sf::Text m_highScoreText;
        int m_highScoreInt = 0;
        int m_currentScore = 0;
        float m_minOfScreen = 0.0f;
        sf::Texture _player;
        sf::Texture _ball;
        sf::Texture _brick;
        sf::Texture _star;
    };
}