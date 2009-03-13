#include "Log.hpp"
#include <sstream>

// Static members
Log* Log::m_Instance;
MsgType Log::t_;

Log::~Log(){}

Log& Log::W(MsgType t)
{
    Log::t_ = t;
    return *m_Instance;
}

void Log::SetLogger(Log* pLogInterface)
{
    m_Instance = pLogInterface;
}
