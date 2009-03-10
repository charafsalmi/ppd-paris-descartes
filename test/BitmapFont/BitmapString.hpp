#ifndef BITMAPSTRING_HPP
#define BITMAPSTRING_HPP

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "BitmapFont.hpp"


class BitmapString: public sf::Drawable
{
public:
	/**
	 * @param[in] font: police bitmap
	 */
	BitmapString(const BitmapFont& font);
	
	/**
	 * Définir le texte de la chaîne
	 */
	void SetText(const char* text);
	void SetText(const std::string& text);
	
	/**
	 * Récupérer le texte de la chaîne
	 */
	const char* GetText() const;
	
	/**
	 * Ajouter un caractère en fin de chaîne
	 * @param[in] character: caractère à insérer
	 */
	void AppendChar(char character);
	
	/**
	 * Insérer un caractère
	 * @param[in] character: caractère à insérer
	 * @param[in] position: indice du caractère
	 * >= 0 : gauche vers droite, < 0 : droite vers gauche (python like)
	 */
	void InsertChar(char character, int position);
	
	/**
	 * Supprimer un caractère de la chaîne
	 * @param[in] position: indice du caractère
	 * >= 0 : gauche vers droite, < 0 : droite vers gauche (python like)
	 */
	void RemoveChar(int position);
	
	/**
	 * Obtenir un caractère de la chaîne
	 * @param[in] position: indice du caractère
	 * >= 0 : gauche vers droite, < 0 : droite vers gauche (python like)
	 * @return caractère de la chaîne
	 */
	char GetCharAt(int position) const;
	
	/**
	 * Changer la police de caractère
	 */
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
	
	int char_width_;
	std::string chars_;
	const BitmapFont* font_;
};


#endif /* BITMAPSTRING_HPP */

