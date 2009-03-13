#include "LogConsole.hpp"

LogConsole::LogConsole()
{
    Window::Load("data/window/console.xml");
}

LogConsole::~LogConsole()
{

}

void LogConsole::Write(MsgType t, const std::string& rMessage)
{
    SetControlText(ID_TXT, rMessage);
}
