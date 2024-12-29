#pragma once
#include <optional>
#include <format>
#include "OwningFont.hpp"
#include "MyWindow.hpp"
#include "Highscore.hpp"
#include <CodeAnalysis/warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/Event.hpp"
#pragma warning(pop)

enum class StateType{
    Menu,
    Gameplay,
    GameLost,
    GameWon
};

class State{
protected:
    MyWindow* window;
    OwningFont* font;
    Highscore highscore{"assets/HighScore.txt"};
    sf::Text highscoreText;

    std::optional<StateType> commonInput() noexcept{
        sf::Event event;
        while(window->pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window->close();
                return std::nullopt;
            }
            if(auto state = stateInput(event)){
                return state;
            }
        }
        return std::nullopt;
    }

    //interface for child states to implement their specific input handling
    virtual std::optional<StateType> stateInput(const sf::Event& event) noexcept = 0;

    void commonRender() noexcept{
        window->clear(BG_COLOR);
        stateRender();
        window->draw(highscoreText);
        window->display();
    }

    //interface for child states to implement their specific rendering
    virtual void stateRender() noexcept = 0;

public:
    explicit State(MyWindow& window, OwningFont& font) noexcept
        : window(&window)
        , font(&font){
        highscoreText = createText(
            std::format("Highscore: {}", highscore.get_score()),
            font, 50u, sf::Text::Bold, 0, 5
        );
    }

    virtual ~State() = default;

    virtual std::optional<StateType> update() noexcept{
        return commonInput();
    }

    void render() noexcept{
        commonRender();
    }
};