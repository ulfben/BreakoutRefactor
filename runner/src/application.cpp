#include "application.hpp"
#include <iosfwd>
#include <iostream>
#include <fstream>
#include <format>
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
    Application::Application(){        
        loadHighScore();
    }

    void Application::run(){
        while(window.isOpen()){
            input();
            if(!update()){
                window.close();
            }
            render();
        }
    }

    void Application::input() noexcept{
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::KeyPressed){
                on_key_pressed(event.key.code);
            } else if(event.type == sf::Event::KeyReleased){
                on_key_released(event.key.code);
            } else if(event.type == sf::Event::Closed){
                window.close();
            }
        }
    }

    bool Application::update(){
        auto m_deltatime = m_clock.restart();
        if(m_state == State::running){
            stars.update(m_deltatime.asSeconds());
            m_player.PlayerUpdate(m_deltatime.asSeconds());
            m_ball.BallUpdate(m_deltatime.asSeconds());
            CollisionCheck();
        } else{
            highscoreText.setString(std::format("HighScore: {}", m_highScoreInt));
        }
        if(m_brick.m_brickObject.empty()){
            m_state = State::win;
        }
        return m_running;
    }

    void Application::render() noexcept{
        auto& w = window.get();
        w.clear(sf::Color{0x44, 0x55, 0x66, 0xff});
        if(m_state == State::pregame){
            w.draw(startMenuText);
        }
        if(m_state == State::running){
            stars.render(w);
            w.draw(scoreText);
            w.draw(m_player.m_playerSprite);
            w.draw(m_ball.m_ballSprite);

            for(int i = 0; i < m_brick.m_brickObject.size(); i++){
                w.draw(m_brick.m_brickObject[i].sprite);
            }
        }

        if(m_state == State::lose){
            w.draw(loseText);
            StoreHighScore();
        }
        if(m_state == State::win){
            w.draw(winText);
            StoreHighScore();
        }
        w.draw(highscoreText);
        w.display();
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
        if(AxisAlignedBoundingBox(m_player.m_playerSprite, m_ball.m_ballSprite)){            
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
        if(m_ball.m_ballSprite.getPosition().y >= static_cast<float>(window.height())){
            m_state = State::lose;
        }
    }

    void Application::doScore(){
        m_currentScore++;
        scoreText.setString(std::format("Score: {}", m_currentScore));
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