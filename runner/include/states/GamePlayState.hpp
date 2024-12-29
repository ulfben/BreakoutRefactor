#pragma once
#include "State.hpp"
#include "OwningTexture.hpp"
#include "Wall.h"
#include "Paddle.h"
#include "Ball.h"
#include "Stars.h"

class GameplayState final : public State{
private:
    OwningTexture playerTex{"assets/player.png"};
    OwningTexture ballTex{"assets/Ball.png"};
    OwningTexture brickTex{"assets/WhiteHitBrick.png"};
    OwningTexture starTex{"assets/FallingStar.png"};
    Wall wall{brickTex};
    Paddle m_player{playerTex};
    Ball m_ball{ballTex};
    Stars stars;
    sf::Text scoreText;
    sf::Clock m_clock;
    unsigned score = 0;

protected:
    std::optional<StateType> stateInput(const sf::Event& event) noexcept override{
        if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape){
            return StateType::Menu;
        }
        return std::nullopt;
    }

    void stateRender() override{
        stars.render(*window);
        window->draw(scoreText);
        m_player.render(*window);
        m_ball.render(*window);
        wall.render(*window);
    }

public:
    GameplayState(MyWindow& window, OwningFont& font)
        : State(window, font)
        , stars(starTex, window){
        scoreText = createText("Score: 0", font, 50u, sf::Text::Bold, 1100, 5);
    }

    std::optional<StateType> update() override{
        if(auto state = commonInput()){
            return state;
        }
        const auto deltaTime = m_clock.restart();
        stars.update(deltaTime.asSeconds());
        m_player.update(deltaTime.asSeconds());
        m_ball.update(deltaTime.asSeconds());
        check_collisions();

        if(wall.empty()){
            highscore.save(score);
            return StateType::GameWon;
        }
        if(m_ball.isBehind(m_player)){
            highscore.save(score);
            return StateType::GameLost;
        }
        return std::nullopt;
    }

    void check_collisions(){
        const auto bounds = sf::FloatRect(0.0f, 0.0f, window->fwidth(), window->fheight());
        m_ball.constrainTo(bounds);
        m_player.constrainTo(bounds);
        m_ball.checkCollisionWith(m_player);
        if(m_ball.checkCollisionWith(wall)){
            do_score();
        }
    }

    void do_score(){
        score++;
        scoreText.setString(std::format("Score: {}", score));
    }

    void restart(){
        score = 0;
        m_ball = Ball{ballTex};
        m_player = Paddle{playerTex};
        wall = Wall{brickTex};
        stars = Stars{starTex, *window};
    }
};