#pragma once
#include "Ball.h"
#include "Configs.hpp"
#include "Highscore.hpp"
#include "MyWindow.hpp"
#include "OwningFont.hpp"
#include "OwningTexture.hpp"
#include "Paddle.h"
#include "pcg32.hpp"
#include "SFML/Graphics/Text.hpp"
#include "Stars.h"
#include "Wall.h"
#include <__msvc_string_view.hpp>
#include <format>
#include <memory>
#include <optional>
#include <SFML/Config.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowStyle.hpp>

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

    enum class StateType{
        Menu,
        Gameplay,
        GameLost,
        GameWon
    };

    class State{
    protected:
        MyWindow* window; //non-owning pointer
        OwningFont* font; //non-owning pointer        
    public:
        explicit State(MyWindow& window, OwningFont& font) noexcept : window(&window), font(&font){}
        virtual ~State() = default;
        virtual std::optional<StateType> update() noexcept = 0;
        virtual void render() noexcept = 0;
    };

    class GameplayState final : public State{
    private:
        Highscore highscore{"assets/HighScore.txt"};
        OwningTexture playerTex{"assets/player.png"};
        OwningTexture ballTex{"assets/Ball.png"};
        OwningTexture brickTex{"assets/WhiteHitBrick.png"};
        OwningTexture starTex{"assets/FallingStar.png"};
        Wall wall{brickTex};
        Paddle m_player{playerTex};
        Ball m_ball{ballTex};
        Stars stars;
        sf::Text scoreText;
        sf::Text highscoreText;
        sf::Clock m_clock;
        unsigned score = 0;

        std::optional<StateType> input() noexcept{
            sf::Event event;
            while(window->pollEvent(event)){
                if(event.type == sf::Event::Closed){
                    window->close();
                }
                if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape){
                    return StateType::Menu;
                }
            }
            return std::nullopt;
        }
    public:
        GameplayState(MyWindow& window, OwningFont& font) : State(window, font), stars(starTex, window){
            scoreText = createText("Score: 0", font, 50u, sf::Text::Bold, 1100, 5);
            highscoreText = createText(std::format("Highscore: {}", highscore.get_score()), font, 50u, sf::Text::Bold, 0, 5);
        }
        std::optional<StateType> update() noexcept override{
            auto newState = input();
            if(newState){
                return newState;
            }
            auto m_deltatime = m_clock.restart();
            stars.update(m_deltatime.asSeconds());
            m_player.update(m_deltatime.asSeconds());
            m_ball.update(m_deltatime.asSeconds());
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

        void render() noexcept override{
            window->clear(BG_COLOR);
            stars.render(*window);
            window->draw(scoreText);
            m_player.render(*window);
            m_ball.render(*window);
            wall.render(*window);
            window->draw(highscoreText);
            window->display();
        }

        void check_collisions() noexcept{
            const auto bounds = sf::FloatRect(0.0f, 0.0f, window->fwidth(), window->fheight());
            m_ball.constrainTo(bounds);
            m_player.constrainTo(bounds);
            m_ball.checkCollisionWith(m_player);
            if(m_ball.checkCollisionWith(wall)){
                do_score();
            }
        }
        void do_score() noexcept{
            score++;
            scoreText.setString(std::format("Score: {}", score));
        }
        void restart() noexcept{
            score = 0;
            m_ball = Ball{ballTex};
            m_player = Paddle(playerTex);
            wall = Wall(brickTex);
            stars = Stars(starTex, *window);
        }
    };

    class MenuState : public State{
        sf::Text startMenuText;
        Highscore highscore{"assets/HighScore.txt"};
        sf::Text highscoreText;
    public:
        MenuState(MyWindow& window, OwningFont& font) : State(window, font){
            startMenuText = createText("Press `space´ to start", font, 100u, sf::Text::Bold, 250, 250);
            highscoreText = createText(std::format("Highscore: {}", highscore.get_score()), font, 50u, sf::Text::Bold, 0, 5);
        }
        std::optional<StateType> update() noexcept override{
            sf::Event event;
            while(window->pollEvent(event)){
                if(event.type == sf::Event::Closed){
                    window->close();
                }
                if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space){
                    return StateType::Gameplay;
                }
            }
            return std::nullopt;
        }

        void render() noexcept override{
            window->clear(BG_COLOR);
            window->draw(startMenuText);
            window->draw(highscoreText);
            window->display();
        }
    };
    class GameOverState : public State{
        sf::Text gameOverText;
    protected:
        Highscore highscore{"assets/HighScore.txt"};
        sf::Text highscoreText;
    public:
        explicit GameOverState(MyWindow& window, OwningFont& font) : State(window, font){
            gameOverText = createText("Game Over! Press space.", font, 50u, sf::Text::Bold, 550, 300);
            highscoreText = createText(std::format("Highscore: {}", highscore.get_score()), font, 50u, sf::Text::Bold, 0, 5);
        }

        std::optional<StateType> update() noexcept override{
            sf::Event event;
            while(window->pollEvent(event)){
                if(event.type == sf::Event::Closed){
                    window->close();
                }
                if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space){
                    return StateType::Menu;
                }
            }
            return std::nullopt;
        }

        void render() noexcept override{
            window->clear(BG_COLOR);
            window->draw(gameOverText);
            window->draw(highscoreText);
            window->display();
        }
    };

    class WinState : public GameOverState{
        sf::Text winText;

    public:
        explicit WinState(MyWindow& window, OwningFont& font) : GameOverState(window, font){
            winText = createText("A winner is you! Press space to restart.", font, 50u, sf::Text::Bold, 550, 300);
        }

        void render() noexcept override{
            window->clear(BG_COLOR);
            window->draw(winText);
            window->draw(highscoreText);
            window->display();
        }
    };

    class Application final{
    public:
        Application(){
            currentState = createState(StateType::Menu);
        }

        void run() noexcept{
            rng().seed(seed::from_time());
            while(window.isOpen()){
                update();
                render();
            }
        }

    private:
        MyWindow window{"Breakout!", {1280, 720}, sf::Style::Titlebar | sf::Style::Close};
        OwningFont font{"assets/sunny-spells-font/SunnyspellsRegular-MV9ze.otf"};
        std::unique_ptr<State> currentState;
        std::unique_ptr<State> createState(StateType type){
            using enum StateType;
            switch(type){
            case Menu:
                return std::make_unique<MenuState>(window, font);
            case Gameplay:
                return std::make_unique<GameplayState>(window, font);
            case GameLost:
                return std::make_unique<GameOverState>(window, font);
            case GameWon:
                return std::make_unique<WinState>(window, font);
            default:
                return nullptr;
            }
        }
        void update() noexcept{
            if(auto nextState = currentState->update()){
                currentState = createState(*nextState);
                assert(currentState);
            }
        }
        void render() noexcept{
            currentState->render();
        }
    };
}