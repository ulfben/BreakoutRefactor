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
namespace runner{
    void Application::run() noexcept{
        rng().seed(seed::from_time());
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
            if(event.type == sf::Event::KeyReleased){
                on_key_released(event.key.code);
            } else if(event.type == sf::Event::Closed){
                window.close();
            }
        }
    }

    bool Application::update() noexcept{
        auto m_deltatime = m_clock.restart();
        if(m_state == State::running){
            stars.update(m_deltatime.asSeconds());
            m_player.update(m_deltatime.asSeconds());
            m_ball.update(m_deltatime.asSeconds());
            check_collisions();
        } else{
            highscoreText.setString(std::format("HighScore: {}", score));
        }
        if(wall.empty()){
            m_state = State::win;
        }
        return m_running;
    }

    void Application::render() noexcept{        
        window.clear(BG_COLOR);
        if(m_state == State::pregame){
            window.draw(startMenuText);
        }
        if(m_state == State::running){
            stars.render(window);
            window.draw(scoreText);
            m_player.render(window);
            m_ball.render(window);
            wall.render(window);
        }
        if(m_state == State::lose){
            window.draw(loseText);
            highscore.save(score);
        }
        if(m_state == State::win){
            window.draw(winText);
            highscore.save(score);
        }
        window.draw(highscoreText);
        window.display();
    }

    void Application::check_collisions() noexcept{
        if(m_ball.isBehind(m_player.sprite)){
            m_state = State::lose;
            return;
        }
        const auto bounds = sf::FloatRect(0.0f, 0.0f, window.fwidth(), window.fheight());
        m_ball.constrainTo(bounds);        
        m_player.constrainTo(bounds);
        m_ball.checkCollisionWith(m_player.sprite); 
        if(m_ball.checkCollisionWith(wall)){        
            do_score();            
        }
    }
  
    void Application::on_key_released(const sf::Keyboard::Key key) noexcept{
        if(key == sf::Keyboard::Key::Escape){
            m_running = false;
        }     
        if(key == sf::Keyboard::Key::Space && m_state == State::pregame){
            m_state = State::running;
        } else if(key == sf::Keyboard::Key::Space && (m_state == State::lose || m_state == State::win)){
            m_state = State::running;
            restart();
        }
    }

    void Application::restart() noexcept{
        score = 0;
        m_ball = Ball{ballTex};
        m_player = Paddle(playerTex);
        wall = Wall(brickTex);
        stars = Stars(starTex, window);
    }

    void Application::do_score() noexcept{
        score++;
        scoreText.setString(std::format("Score: {}", score));
    }
}