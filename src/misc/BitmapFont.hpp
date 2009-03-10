#ifndef BITMAPFONT_HPP
#define BITMAPFONT_HPP

#include <SFML/Graphics.hpp>

class BitmapFont
{
public:
	/**
	 * @param[in] width: largeur d'un caractère
	 * @param[in] height: hauteur d'un caractère
	 * @param[in] width_count: nombre de caractère en largeur
	 * @param[in] height_count: nombre de caractère en hauteur
	 */
	BitmapFont(int width, int height, int width_count, int height_count);
	
	void SetImage(const sf::Image& image);
	
	const sf::Image& GetImage() const;
	
	/**
	 * Obtenir le subrect d'un caractère
	 * @param[in] character: caractère demandé
	 * @param[out] subrect: subrect de l'image de la police
	 */
	void GetCharRect(char character, sf::IntRect& subrect) const;
	
	int GetCharWidth() const;
	
private:
	const sf::Image* image_;
	int width_;
	int height_;
	int width_count_;
	int height_count_;
};


#endif /* BITMAPFONT_HPP */

