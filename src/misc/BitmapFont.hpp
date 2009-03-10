#ifndef BITMAPFONT_HPP
#define BITMAPFONT_HPP

#include <SFML/Graphics.hpp>


class BitmapFont
{
public:
	/**
	 * @param[in] image: image de la police bitmap
	 * @param[in] width: nombre de caractères en largeur
	 * @param[in] height: nombre de caractère en hauteur
	 */
	BitmapFont(const sf::Image& image, int width, int height);

	/**
	 * Changer l'image de la police
	 */
	void SetImage(const sf::Image& image);

	/**
	 * Récupérer l'image en cours d'utilisation
	 */
	const sf::Image& GetImage() const;

	/**
	 * Obtenir le subrect d'un caractère
	 * @param[in] character: caractère demandé
	 * @return: subrect dans l'image de la police
	 */
	sf::IntRect GetCharRect(char character) const;

	/**
	 * Largeur d'un caractère
	 */
	int GetCharWidth() const;

	/**
	 * Hauteur d'un caractère
	 */
	int GetCharHeight() const;

private:
	const sf::Image* image_;
	int width_;
	int height_;
	int char_width_;
	int char_height_;
};


#endif /* BITMAPFONT_HPP */

