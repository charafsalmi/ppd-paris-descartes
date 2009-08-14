#ifndef LABEL_HPP
#define LABEL_HPP

#include "Control.hpp"
#include "../misc/BitmapString.hpp"

namespace gui
{

class Label : public Control
{
public:
	Label(ControlID id, const ControlPos& pos, const std::string& text, const std::string &font);

	void SetText(const std::string& s);
private:
	void Update();
	void Render(sf::RenderTarget& app) const;
	BitmapString BStext_;
};
}
#endif
