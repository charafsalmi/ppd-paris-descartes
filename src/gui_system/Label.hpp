#ifndef LABEL_HPP
#define LABEL_HPP

#include "Control.hpp"

namespace gui{
    class Label : public Control{
    public:
        Label(const ControlPos& pos, const std::string& text);
        ~Label();

        void Show(sf::RenderTarget& app);
    private:
        sf::String text_;
    };
}
#endif
