#ifndef PROGRESSBAR_HPP
#define PROGRESSBAR_HPP

#include "Control.hpp"

namespace gui{
    class ProgressBar : public Control{
    public:
        ProgressBar(const ControlID id, const ControlPos& pos, const ControlPos& size);
        void Update();
    private:

        void Render(sf::RenderTarget& app) const;

        float width_, height_;

        sf::Shape background_;
        sf::Sprite bar_;
    };
}

#endif
