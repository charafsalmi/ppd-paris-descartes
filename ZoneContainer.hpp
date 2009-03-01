#ifndef ZONECONTAINER_HPP
#define ZONECONTAINER_HPP

#include "Zone.hpp"

class ZoneContainer
{
public:
	enum MapName
	{
		WORLD, CAVES
	};
	
	enum Direction
	{
		UP, DOWN, LEFT, RIGHT
	};
	
	ZoneContainer();
	
	~ZoneContainer();
	
	/**
	 * Charger les zones du conteneur depuis un fichier XML
	 */
	void Load(MapName name, sf::RenderWindow& app);
	
	/**
	 * Désallouer toutes les zones du conteneur
	 */
	void Unload();
	
	/**
	 * Obtenir la zone active
	 * @return adresse zone active
	 */
	inline Zone* GetActiveZone()
	{
		return active_zone_;
	}
	
	/**
	 * Définir directement une zone active (si possible)
	 * @param[in] x: position x de la zone souhaitée
	 * @param[in] y: position y de la zone souhaitée
	 * @param[in] wait: indique si on doit attendre la fin de la boucle de jeu
	 * pour procéder au changement de zone
	 * @return true si le changement a été accepté, sinon false
	 */
	bool SetActiveZone(int x, int y, bool wait=true);
	
	MapName GetName() const;
	
	bool Scrolling() const
	{
		return scrolling_;
	}
	
	void Update(float frametime);
	
	void Show(sf::RenderWindow& app);
	
	/**
	 * Demander un changement de zone
	 * @param[in] dir: direction souhaitée
	 */
	void ChangeZone(Direction dir);
	
private:	
	/**
	 * Effet de défilement lors d'un changement de zone
	 */
	struct ZoneScroller
	{
		sf::Sprite current;
		sf::Sprite next;
		Direction dir;
		float timer;
	};
	
	bool scrolling_;
	ZoneScroller scroll_;
	
	Zone* active_zone_;
	sf::Vector2i cds_zone_; // coordonnées de la zone courante
	
	Zone* next_zone_;
	
	MapName name_;
	
	int width_;
	int height_;
	Zone** zones_;
};

#endif /* ZONECONTAINER_HPP */
