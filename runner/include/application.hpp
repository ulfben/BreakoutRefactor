#pragma once

#include "AssetsManagment.h"
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
        Application() = default;
        void run();

    private:
        enum class TheGamesStates{
            win, lose, running, pregame
        };
        enum class TextureName{
            Texture_player, Texture_ball, Texture_brick
        };
        bool enter();
        void exit();
        void SetUp();
        bool update();
        void render();
        void CollisionCheck();
        void Restart();    
        void on_mouse_move(const sf::Vector2f& position);
        void on_key_pressed(const sf::Keyboard::Key key);
        void on_key_released(const sf::Keyboard::Key key);
        void on_button_pressed(const sf::Mouse::Button button);
        void on_button_released(const sf::Mouse::Button button);
        void loadHighScore();
        void StoreHighScore();
        std::string intToString(int score);
        bool AxisAlignedBoundingBox(sf::Sprite& box1, sf::Sprite& box2);
            
        sf::RenderWindow m_window;
        PrimitiveBatch   m_batch;
        bool             m_running = true;
        sf::Clock        m_clock;
        sf::Time         m_deltatime;
        sf::Vector2f     m_mouse_position;
        sf::Sprite       m_sprite;
        PlayerCharacter  m_player;
        Ball             m_ball;
        Brick            m_brick;
        AssetsManagement m_AssetsManagement;
        TheGamesStates   m_CurrentGameState = TheGamesStates::pregame;
        parallaxBackground m_parallaxBackground;
        sf::Text m_startMainuText;
        sf::Text m_WinText;
        sf::Text m_LoseText;
        sf::Text m_ScoreText;
        sf::Text m_highScoreText;
        int m_highScoreInt = 0;
        int m_currentScore = 0;
        float m_minOfScreen = 0.0f;
    };
}