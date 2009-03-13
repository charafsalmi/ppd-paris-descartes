#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include "Control.hpp"
#include "../misc/BitmapString.hpp"

namespace gui
{

class TextBox: public Control
{
public:
	TextBox(ControlID id, const ControlPos& pos, int width);

	/**
	 * Indiquer le texte de la textbox
	 */
	void SetText(const std::string & s);

	/**
	 * Récupérer le contenu de la textbox
	 */
	std::string GetText() const;

	// inherited
	void OnTextEntered(sf::Uint32 unicode);

	// inherited
	void OnKeyPressed(sf::Key::Code key);

private:
	// inherited
	void Render(sf::RenderTarget& app) const;

	/**
	 * Positionne le curseur
	 * @param[in] position: position du curseur dans le texte
	 */
	void SetCursor(int position);

	sf::Shape background_;
	sf::Shape cursor_;
	int cursor_pos_;
	BitmapString BStext_;
};

}

#endif /* TEXTBOX_HPP */
