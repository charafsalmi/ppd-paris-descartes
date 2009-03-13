#include "LogDebug.hpp"
#include <iostream>

void LogDebug::Write(MsgType t, const std::string& rMessage)
{
    if(t == L_ERROR){
        std::cerr << rMessage;
    }else{
        std::cout << rMessage;
    }
}
