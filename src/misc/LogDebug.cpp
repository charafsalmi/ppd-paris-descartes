#include "LogDebug.hpp"
#include <iostream>

void LogDebug::Write(MsgType t, const std::string& rMessage)
{
    if(t == L_WARNING)
        std::cout << "Warning :";
    if(t == L_ERROR){
        std::cerr << rMessage << std::endl;
    }else{
        std::cout << rMessage << std::endl;
    }
}
