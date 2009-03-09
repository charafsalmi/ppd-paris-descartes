#ifndef BITMAPSTRING_HPP
#define BITMAPSTRING_HPP

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "BitmapFont.hpp"


class BitmapString: public sf::Drawable
{
public:
	BitmapString();
	
	/**
	 * Définir le texte de la chaîne
	 */
	void SetText(const char* text);
	void SetText(const std::string& text);
	
	/**
	 * Ajouter un caractère en fin de chaîne
	 */
	void AppendChar(char character);
	
	void InsertChar(char character, int position);
	
	void RemoveChar(int position);
	
	void SetFont(const BitmapFont& font);
	
	/**
	 * Vider la chaîne
	 */
	void Clear();
	
	/**
	 * Obtenir le nombre de caractères
	 */
	int Length() const;
	
private:
	void Render(sf::RenderTarget& target) const;
	
	typedef std::vector<sf::Sprite> SpriteVec;
	SpriteVec text_;
	
	const BitmapFont* font_;
};


#endif /* BITMAPSTRING_HPP */

