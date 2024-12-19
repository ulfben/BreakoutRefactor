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
#include <format>
#include <memory>
#include <optional>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include "State.hpp"
namespace runner {
    class GameplayState final : public State {
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
        std::optional<StateType> stateInput(const sf::Event& event) noexcept override {
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape) {
                return StateType::Menu;
            }
            return std::nullopt;
        }

        void stateRender() noexcept override {
            stars.render(*window);
            window->draw(scoreText);
            m_player.render(*window);
            m_ball.render(*window);
            wall.render(*window);
        }

    public:
        GameplayState(MyWindow& window, OwningFont& font) 
            : State(window, font)
            , stars(starTex, window) {
            scoreText = createText("Score: 0", font, 50u, sf::Text::Bold, 1100, 5);
        }

        std::optional<StateType> update() noexcept override {
            if (auto state = commonInput()) {
                return state;
            }
            auto deltaTime = m_clock.restart();
            stars.update(deltaTime.asSeconds());
            m_player.update(deltaTime.asSeconds());
            m_ball.update(deltaTime.asSeconds());
            check_collisions();

            if (wall.empty()) {
                highscore.save(score);
                return StateType::GameWon;
            }
            if (m_ball.isBehind(m_player)) {
                highscore.save(score);
                return StateType::GameLost;
            }
            return std::nullopt;
        }

        void check_collisions() noexcept {
            const auto bounds = sf::FloatRect(0.0f, 0.0f, window->fwidth(), window->fheight());
            m_ball.constrainTo(bounds);
            m_player.constrainTo(bounds);
            m_ball.checkCollisionWith(m_player);
            if (m_ball.checkCollisionWith(wall)) {
                do_score();
            }
        }

        void do_score() noexcept {
            score++;
            scoreText.setString(std::format("Score: {}", score));
        }

        void restart() noexcept {
            score = 0;
            m_ball = Ball{ballTex};
            m_player = Paddle{playerTex};
            wall = Wall{brickTex};
            stars = Stars{starTex, *window};
        }
    };

    class MenuState final : public State {
    private:
        sf::Text startMenuText;

    protected:
        std::optional<StateType> stateInput(const sf::Event& event) noexcept override {
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
                return StateType::Gameplay;
            }
            return std::nullopt;
        }

        void stateRender() noexcept override {
            window->draw(startMenuText);
        }

    public:
        MenuState(MyWindow& window, OwningFont& font) 
            : State(window, font) {
            startMenuText = createText(
                "Press `space´ to start", 
                font, 100u, sf::Text::Bold, 250, 250
            );
        }
    };

    class EndGameState : public State {
    protected:
        sf::Text endGameText;

        std::optional<StateType> stateInput(const sf::Event& event) noexcept override {
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
                return StateType::Menu;
            }
            return std::nullopt;
        }

        void stateRender() noexcept override {
            window->draw(endGameText);
        }

    public:
        EndGameState(MyWindow& window, OwningFont& font, const std::string& message) 
            : State(window, font) {
            endGameText = createText(message, font, 50u, sf::Text::Bold, 550, 300);
        }
    };

    class GameOverState final : public EndGameState {
    public:
        explicit GameOverState(MyWindow& window, OwningFont& font)
            : EndGameState(window, font, "Game Over! Press space.") {}
    };

    class WinState final : public EndGameState {
    public:
        explicit WinState(MyWindow& window, OwningFont& font)
            : EndGameState(window, font, "A winner is you! Press space to restart.") {}
    };

    class Application final {
    public:
        Application() {
            currentState = createState(StateType::Menu);
        }

        void run() noexcept {
            rng().seed(seed::from_time());
            while (window.isOpen()) {
                update();
                render();
            }
        }

    private:
        MyWindow window{"Breakout!", {1280, 720}, sf::Style::Titlebar | sf::Style::Close};
        OwningFont font{"assets/sunny-spells-font/SunnyspellsRegular-MV9ze.otf"};
        std::unique_ptr<State> currentState;

        std::unique_ptr<State> createState(StateType type) {
            using enum StateType;
            switch (type) {
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

        void update() noexcept {
            if (auto nextState = currentState->update()) {
                currentState = createState(*nextState);
                assert(currentState);
            }
        }

        void render() noexcept {
            currentState->render();
        }
    };
}