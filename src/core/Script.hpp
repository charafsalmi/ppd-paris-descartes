#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <string>

class Script{
public:
    Script();
    ~Script();

    /// Charge un script en mémoire
    /// @param[in] str Script à charger
    void Load(const std::string & str);
};

#endif

