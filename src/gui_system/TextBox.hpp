#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include "Control.hpp"
#include "../misc/BitmapString.hpp"

namespace gui
{

class TextBox: public Control
{
public:
	TextBox(ControlID id, const ControlPos& pos);

private:
	// inherited
	void Render(sf::RenderTarget& app) const;

	sf::Shape background_;
	sf::Shape cursor_;
	BitmapString text_;
};

}


#endif /* TEXTBOX_HPP */
