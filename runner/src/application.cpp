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
#include <numbers>
namespace runner
{
    static constexpr auto TO_RAD = std::numbers::pi_v<float> / 180.0f;
    static constexpr auto MAX_BOUNCE_ANGLE = 75.0f * TO_RAD;
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
        if(wall.empty()){
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
            wall.render(w);
        }

        if(m_state == State::lose){
            w.draw(loseText);
            saveHighscore();
        }
        if(m_state == State::win){
            w.draw(winText);
            saveHighscore();
        }
        w.draw(highscoreText);
        w.display();
    }

    void Application::on_key_pressed(const sf::Keyboard::Key key){
        if(key == sf::Keyboard::Key::Escape){
            m_running = false;
        }
        m_player.pressedLeft = (key == sf::Keyboard::Key::Left);
        m_player.pressedRight = (key == sf::Keyboard::Right);

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
        wall = Wall(brickTex);
    }

    void Application::CollisionCheck(){
        handlePaddleBallCollision(m_player.m_playerSprite, m_ball.m_ballSprite, m_ball.m_direction);
        for(auto& brick : wall){
            if(is_colliding(brick, m_ball.m_ballSprite)){
                m_ball.m_direction.y = -m_ball.m_direction.y;
                m_ball.m_speed += 100.0f;
                wall.erase(brick); //TODO: this is incredibly dumb to do during iteration
                doScore();
                break;
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

    void Application::saveHighscore(){
        std::ofstream writeFile("assets/HighScore.txt");
        if(writeFile.is_open()){
            if(m_currentScore > m_highScoreInt){
                m_highScoreInt = m_currentScore;
            }
            writeFile << m_highScoreInt;
        }
        writeFile.close();
    }

    bool Application::is_colliding(const sf::Sprite& box1, const sf::Sprite& box2) const noexcept{
        return box1.getGlobalBounds().intersects(box2.getGlobalBounds());
    }

    void Application::handlePaddleBallCollision(const sf::Sprite& paddle, sf::Sprite& ball, sf::Vector2f& ballVelocity) const noexcept{
        if(!is_colliding(paddle, ball)){
            return;
        }
        const float ball_x = ball.getPosition().x;
        const float paddleHalfWidth = paddle.getGlobalBounds().width / 2.0f;        
        const float paddleLeft = paddle.getGlobalBounds().left;
        const float relativeIntersectX = std::clamp((ball_x - paddleLeft - paddleHalfWidth) / paddleHalfWidth, -1.0f, 1.0f);
        const float angle = relativeIntersectX * MAX_BOUNCE_ANGLE;
        const float speed = std::sqrt(ballVelocity.x * ballVelocity.x + ballVelocity.y * ballVelocity.y);
        ballVelocity.x = speed * std::sin(angle);
        ballVelocity.y = -speed * std::cos(angle);
        ball.setPosition(ball_x, paddle.getGlobalBounds().top - ball.getGlobalBounds().height / 2); //move the ball out of the paddle
    }

}