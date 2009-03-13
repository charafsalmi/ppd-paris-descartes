#ifndef LOG_HPP
#define LOG_HPP

#include <ostream>
#include <string>
#include <sstream>


/*@NOTE_4621
Log est une classe abstraite pour permettre
l'affichage de messages sous divers supports.

Exemple d'utilisation :
Log::SetLogger(new LogDebug());
Log::Log(WARNING) << "message;"
*/

#define Output Log::W()
#define OutputW Log::W(L_WARNING)
#define OutputE Log::W(L_ERROR)

enum MsgType{
    L_INFO=0,
    L_WARNING,
    L_ERROR
};

class Log
{
private:
    static MsgType t_;
public:
    virtual ~Log();
    static Log& W(MsgType t=L_INFO);
    template <class T> Log& operator <<(const T& ToLog);
    static void SetLogger(Log* pLogInterface);
private:
    static Log* m_Instance;
    virtual void Write(MsgType t, const std::string& rMessage) = 0;
    static void Destroy();
};

template <class T> Log& Log::operator <<(const T& ToLog){
    std::ostringstream Stream;
    Stream << ToLog;
    Write(Log::t_, Stream.str());

    return W(Log::t_);
}

#endif
