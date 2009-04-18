#ifndef OPTION_HPP
#define OPTION_HPP

#include "../gui_system/Window.hpp"

class Option: public gui::Window
{
public:
    Option();
    ~Option();

     /** Retours de l'inventaire */
    enum
    {
        _EXIT=101
    };

private:
    /// Listing des fenêtres
    enum Windows
    {
        ID_MUTE=7000, ID_PLUS, ID_MOINS
    };

    float volume_;

    int WindowCallback(const gui::Control::ControlID id, const int p1, void *p2);
};

#endif
