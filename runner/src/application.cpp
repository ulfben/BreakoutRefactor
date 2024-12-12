// application.cpp

#include "application.hpp"
#include <iosfwd>
#include <iostream>
#include <fstream>
#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <string>
#include <stdexcept>
namespace runner
{

    sf::Text createText(std::string_view s, const sf::Font& font, unsigned size, sf::Uint32 textStyle, float positionX, float positionY){
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(size);
        text.setStyle(textStyle);
        text.setPosition(positionX, positionY);
        text.setString(s.data());
        return text;
    }

    Application::Application(){
        const sf::VideoMode mode{1280, 720};
        const sf::Uint32 flags = sf::Style::Titlebar | sf::Style::Close;
        m_window.create(mode, "pineapple", flags);
        if(!m_window.isOpen()){
            throw std::runtime_error("Failed to create window");
        }
        m_window.setKeyRepeatEnabled(false);
        if(!m_font.loadFromFile("assets/sunny-spells-font/SunnyspellsRegular-MV9ze.otf")){
            throw std::runtime_error("Failed to load font!");
        }

        _player.loadFromFile("assets/player.png");//TODO check return values, and make RAII
        _ball.loadFromFile("assets/Ball.png");
        _brick.loadFromFile("assets/WhiteHitBrick.png");
        _star.loadFromFile("assets/FallingStar.png");

        m_startMainuText = createText("Press `space´ to start", m_font, 100u, sf::Text::Bold, 250, 250);
        m_WinText = createText("Winner", m_font, 50u, sf::Text::Bold, 550, 300);
        m_LoseText = createText("Game Over", m_font, 50u, sf::Text::Bold, 550, 300);
        m_ScoreText = createText("Score", m_font, 50u, sf::Text::Bold, 1100, 5);
        m_highScoreText = createText("", m_font, 50u, sf::Text::Bold, 0, 5);

        loadHighScore();

        m_player.SetUp(_player, m_minOfScreen, (float) m_window.getSize().x);
        m_ball.SetUp(_ball, m_window.getSize().x, m_window.getSize().y, (int) m_minOfScreen, (int) m_minOfScreen);
        m_brick.SetUp(_brick);
        m_parallaxBackground.SetUp(_star);
    }

    void Application::run(){
        while(m_window.isOpen()){
            input();
            if(!update()){
                m_window.close();
            }
            render();
        }
    }

    void Application::input() noexcept{
        sf::Event event;
        while(m_window.pollEvent(event)){
            if(event.type == sf::Event::KeyPressed){
                on_key_pressed(event.key.code);
            } else if(event.type == sf::Event::KeyReleased){
                on_key_released(event.key.code);
            } else if(event.type == sf::Event::Closed){
                m_window.close();
            }
        }
    }

    bool Application::update(){
        m_deltatime = m_clock.restart();
        if(m_state == State::running){
            m_parallaxBackground.Update(m_deltatime.asSeconds());            
            m_player.PlayerUpdate(m_deltatime.asSeconds());
            m_ball.BallUpdate(m_deltatime.asSeconds());
            CollisionCheck();
        } else{
            m_highScoreText.setString("HighScore: " + std::to_string(m_highScoreInt));
        }
        if(m_brick.m_brickObject.empty()){
            m_state = State::win;
        }
        return m_running;
    }

    void Application::render(){
        m_batch.clear();
        m_window.clear(sf::Color{0x44, 0x55, 0x66, 0xff});
        if(m_state == State::pregame){
            m_window.draw(m_startMainuText);
        }
        if(m_state == State::running){
            for(int i = 0; i < m_parallaxBackground.m_fallingStarYellow.size(); i++){
                m_window.draw(m_parallaxBackground.m_fallingStarYellow[i].sprite);
            }
            for(int i = 0; i < m_parallaxBackground.m_fallingStarRed.size(); i++){
                m_window.draw(m_parallaxBackground.m_fallingStarRed[i].sprite);
            }
            m_window.draw(m_ScoreText);
            m_window.draw(m_player.m_playerSprite);
            m_window.draw(m_ball.m_ballSprite);

            for(int i = 0; i < m_brick.m_brickObject.size(); i++){
                m_window.draw(m_brick.m_brickObject[i].sprite);
            }
        }

        if(m_state == State::lose){
            m_window.draw(m_LoseText);
            StoreHighScore();
        }
        if(m_state == State::win){
            m_window.draw(m_WinText);
            StoreHighScore();
        }
        m_window.draw(m_highScoreText);
        m_batch.present(m_window);
        m_window.display();
    }

    void Application::on_key_pressed(const sf::Keyboard::Key key){
        if(key == sf::Keyboard::Key::Escape){
            m_running = false;
        }
        if(key == sf::Keyboard::Key::Left){
            m_player.pressedLeft = true;
        } else{
            m_player.pressedLeft = false;
        }

        if(key == sf::Keyboard::Right){
            m_player.pressedRight = true;
        } else{
            m_player.pressedRight = false;
        }

        if(m_state == State::pregame){
            if(key == sf::Keyboard::Key::Space){
                m_state = State::running;
            }
        }

        if(m_state == State::lose || m_state == State::win){
            if(key == sf::Keyboard::Key::Space){
                m_state = State::running;
                Restart();
            }
        }
    }

    void Application::on_key_released(const sf::Keyboard::Key key){
        if(key == sf::Keyboard::Right){
            m_player.pressedRight = false;
        }
        if(key == sf::Keyboard::Key::Left){
            m_player.pressedLeft = false;
        }
    }

    void Application::Restart(){
        m_currentScore = 0;
        m_ball.Restart();
        m_player.Restart();
        m_brick.Restart();
    }

    void Application::CollisionCheck(){
        float r1RightEdge = m_player.m_playerSprite.getPosition().y + m_player.m_playerSprite.getTexture()->getSize().y;

        if(AxisAlignedBoundingBox(m_player.m_playerSprite, m_ball.m_ballSprite)){

            m_ball.hasCollided = true;
            m_ball.m_direction.y = -m_ball.m_direction.y;
        }

        for(int i = 0; i < m_brick.m_brickObject.size(); i++){
            if(AxisAlignedBoundingBox(m_brick.m_brickObject[i].sprite, m_ball.m_ballSprite)){
                m_ball.m_direction.y = -m_ball.m_direction.y;
                m_ball.m_speed += 10.0f;
                m_brick.m_brickObject.erase(m_brick.m_brickObject.begin() + i);
                doScore();
            }
        }
        for(int i = 0; i < m_parallaxBackground.m_fallingStarYellow.size(); i++){
            if(m_parallaxBackground.m_fallingStarYellow[i].positionY >= m_window.getSize().y){
                m_parallaxBackground.m_fallingStarYellow[i].positionY = -100;
            }
        }
        for(int i = 0; i < m_parallaxBackground.m_fallingStarRed.size(); i++){
            if(m_parallaxBackground.m_fallingStarRed[i].positionY >= m_window.getSize().y){
                m_parallaxBackground.m_fallingStarRed[i].positionY = -100;
            }
        }
        // If the player is out of bounds or edge of the bottom screen that should give trigger fail condition.
        if(m_ball.m_ballSprite.getPosition().y >= m_window.getSize().y){
            m_state = State::lose;
        }
    }

    void Application::doScore(){
        m_currentScore++;
        m_ScoreText.setString("Score: " + std::to_string(m_currentScore));

    }

    void Application::loadHighScore(){
        std::ifstream readFile;
        readFile.open("assets/HighScore.txt");
        if(readFile.is_open()){
            while(!readFile.eof()){
                readFile >> m_highScoreInt;
            }

        }
        readFile.close();
    };

    void Application::StoreHighScore(){
        std::ofstream writeFile("assets/HighScore.txt");
        if(writeFile.is_open()){
            if(m_currentScore > m_highScoreInt){
                m_highScoreInt = m_currentScore;
            }
            writeFile << m_highScoreInt;
        }
        writeFile.close();
    }

    bool Application::AxisAlignedBoundingBox(sf::Sprite& box1, sf::Sprite& box2){
        bool collisionX = box1.getPosition().x + box1.getTexture()->getSize().x >= box2.getPosition().x &&
            box2.getPosition().x + box2.getTexture()->getSize().x >= box1.getPosition().x;

        bool collisionY = box1.getPosition().y + box1.getTexture()->getSize().y >= box2.getPosition().y &&
            box2.getPosition().y + box2.getTexture()->getSize().y >= box1.getPosition().y;
        return collisionX && collisionY;
    }
}