#pragma once
#include "Configs.hpp"
#include <fstream>
#include <filesystem>

class Highscore final{
    unsigned score = 0;
    std::filesystem::path path;
public:
    explicit Highscore(std::filesystem::path p) : path(std::move(p)){
        std::ifstream readFile(path);
        if(!readFile.is_open()){
            return; //I won't throw an exception here, because the game should still be able to run without a highscore file
        }
        while(!readFile.eof()){
            readFile >> score;
        }
    }
    [[nodiscard]] unsigned get_score() const noexcept{
        return score;
    }
    void save(unsigned newScore){
        if(newScore <= score){
            return;
        }
        score = newScore;
        std::ofstream writeFile(path);
        if(writeFile.is_open()){
            writeFile << score;
        }
    }
};