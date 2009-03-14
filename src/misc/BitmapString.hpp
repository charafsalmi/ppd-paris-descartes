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
	 * @param[in] character: caractère à ajouter
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
	 * Changer la police bitmap
	 */
	void SetFont(const BitmapFont& font);

	/**
	 * Récupérer la police bitmap en cours d'utilisation
	 */
	const BitmapFont& GetFont() const;

	/**
	 * Vider la chaîne
	 */
	void Clear();

	/**
	 * Obtenir le nombre de caractères
	 */
	int Length() const;

	/**
	 * Définir la couleur des bitmaps
	 */
	void SetColor(const sf::Color& color);

private:
	/**
	 * Affichage
	 */
	void Render(sf::RenderTarget& target) const;

	/**
	 * Transforme les positions négatives en positions positives exploitables
	 */
	int GetRealPosition(int position) const;

	/**
	 * Recalculer la position des bitmaps
	 * @param[in] from: première position à recalculer
	 */
	void ComputePosition(int from);

	typedef std::vector<sf::Sprite> SpriteVec;
	SpriteVec bitmaps_;

	int char_width_;
	std::string chars_;
	const BitmapFont* font_;
};


#endif /* BITMAPSTRING_HPP */

