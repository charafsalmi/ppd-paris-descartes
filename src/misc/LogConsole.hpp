#ifndef LOGCONSOLE_HPP
#define LOGCONSOLE_HPP

#include "Log.hpp"

class LogConsole : public Log
{
private:
    virtual void Write(MsgType t, const std::string& rMessage);
};

#endif
