#ifndef LOGCONSOLE_HPP
#define LOGCONSOLE_HPP

#include "../gui_system/Window.hpp"
#include "Log.hpp"

class LogConsole : public Log, public gui::Window
{
public:
    LogConsole();
    ~LogConsole();
private:
    enum{
        ID_TXT=7000
    };
    void Write(MsgType t, const std::string& rMessage);
};

#endif
