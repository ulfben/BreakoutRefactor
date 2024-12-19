#pragma once
#include "State.hpp"
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