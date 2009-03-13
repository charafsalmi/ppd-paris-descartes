#ifndef MINIMAP_HPP
#define MINIMAP_HPP

#include <SFML/Graphics.hpp>

#include "../core/ZoneContainer.hpp"


class MiniMap: public sf::Drawable
{
public:
	MiniMap(const ZoneContainer& zones);

	~MiniMap();

	int GetWidth() const;

	int GetHeight() const;

	void SetPlayerPosition(const sf::Vector2i& coords);

	void Update(float frametime);

private:
	// inherited
	void Render(sf::RenderTarget& target) const;

	sf::Shape** preview_;
	sf::Shape* player_pos_;
	int width_;
	int height_;
	int square_size_;
};


#endif /* MINIMAP_HPP */
