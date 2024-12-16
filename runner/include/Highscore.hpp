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
            return;
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