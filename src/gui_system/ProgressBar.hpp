#ifndef PROGRESSBAR_HPP
#define PROGRESSBAR_HPP

#include "Control.hpp"

namespace gui{

class ProgressBar : public Control{
public:
	ProgressBar(const ControlID id, const ControlPos& pos, int width, int height);

private:
	sf::Shape background_;
	sf::Sprite bar_;
};

}

#endif
