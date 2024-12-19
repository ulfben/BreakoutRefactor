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
#include "./states/State.hpp"
#include "./states/GamePlayState.hpp"
#include "./states/MenuState.hpp"
#include "./states/EndGameState.hpp"

namespace runner {    
    class Application final {
    public:
        Application() {
            currentState = createState(StateType::Menu);
        }

        void run() {
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

        void update() {
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