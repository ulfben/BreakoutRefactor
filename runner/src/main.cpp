#include "application.hpp"
#include <iostream>
#include <stdexcept>
int main(){
    using enum StateType;
    try{
        runner::Application{Menu}.run();
    } catch(const std::runtime_error& e){
        std::cerr << e.what() << std::endl;
    } catch(const std::exception& e){
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch(...){
        std::cerr << "Unknown exception" << std::endl;
    }
    return 0;
}
