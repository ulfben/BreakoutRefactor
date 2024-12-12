#include "application.hpp"
#include <iostream>
#include <stdexcept>
int main(){
    try{
        runner::Application{}.run();
    } catch(const std::runtime_error& e){
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
