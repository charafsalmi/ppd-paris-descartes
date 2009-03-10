#ifndef LABEL_HPP
#define LABEL_HPP

#include "Control.hpp"
#include "../misc/BitmapString.hpp"

namespace gui{
    class Label : public Control{
    public:
        Label(const ControlID id, const ControlPos& pos, const std::string& text);
        ~Label();


    private:
		void Render(sf::RenderTarget& app) const;
        BitmapString text_;
    };
}
#endif
