#pragma once
#include "State.hpp"
class EndGameState : public State{
protected:
    sf::Text endGameText;

    std::optional<StateType> stateInput(const sf::Event& event) noexcept override{
        if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space){
            return StateType::Menu;
        }
        return std::nullopt;
    }

    void stateRender() noexcept override{
        window->draw(endGameText);
    }

public:
    EndGameState(MyWindow& window, OwningFont& font, const std::string& message)
        : State(window, font){
        endGameText = createText(message, font, 50u, sf::Text::Bold, 550, 300);
    }
};

class GameOverState final : public EndGameState{
public:
    explicit GameOverState(MyWindow& window, OwningFont& font)
        : EndGameState(window, font, "Game Over! Press space."){}
};

class WinState final : public EndGameState{
public:
    explicit WinState(MyWindow& window, OwningFont& font)
        : EndGameState(window, font, "A winner is you! Press space to restart."){}
};