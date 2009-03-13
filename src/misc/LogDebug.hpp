#ifndef LOGDEBUG_HPP
#define LOGDEBUG_HPP

#include "Log.hpp"

class LogDebug : public Log
{
private:
    virtual void Write(MsgType t, const std::string& rMessage);
};

#endif
