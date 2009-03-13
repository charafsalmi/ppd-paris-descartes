#include "Log.hpp"
#include <sstream>

// Static members
Log* Log::m_Instance;
MsgType Log::t_;
int Log::verbosity_level_;

Log::~Log(){}

Log& Log::W(MsgType t)
{
    Log::t_ = t;
    return *m_Instance;
}

void Log::SetVerboseLevel(int v)
{
    Log::verbosity_level_ = v;
}

void Log::SetLogger(Log* pLogInterface)
{
    m_Instance = pLogInterface;
}
