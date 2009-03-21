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
#define OutputD Log::W(L_DEVEL)
#define OutputT Log::W(L_TITLE)

#define PLAYER_S "[Player]\t"
#define GAME_S "[Game]\t"
#define TILE_S "[Tileset]\t"
#define ZONE_S "[Zone]\t"
#define WINDOW_S "[Window]\t"
#define UF_S "[UnitFactory]\t"
#define ZC_S "[ZoneContainer]\t"

/// Caractères de fin de ligne
#define lEnd "ejo37895Yhfrj"

enum MsgType{
    L_DEVEL,
    L_DEBUG,
    L_TITLE,
    L_WARNING,
    L_ERROR
};

class Log
{
private:
    static MsgType t_;
    static int verbosity_level_;
public:
    virtual ~Log();
    static Log& W(MsgType t=L_DEBUG);
    template <class T> Log& operator <<(const T& ToLog);
    static void SetLogger(Log* pLogInterface);
    static void SetVerboseLevel(int v);
    static std::ostringstream stream;
private:
    static Log* m_Instance;
    virtual void Write(MsgType t, const std::string& rMessage) = 0;
    static void Destroy();
};

template <class T> Log& Log::operator <<(const T& ToLog){
    if(Log::t_ < Log::verbosity_level_)
        return W(Log::t_);

    std::ostringstream tmp;
    tmp << ToLog;
    if(tmp.str() == lEnd){
        Write(Log::t_, Log::stream.str());
        stream.str("");
    }else{
        stream << ToLog;
    }
    return W(Log::t_);
}

#endif
