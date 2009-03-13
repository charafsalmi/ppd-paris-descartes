#ifndef ZONECONTAINER_HPP
#define ZONECONTAINER_HPP

#include "Zone.hpp"
#include "../xml/tinyxml.h"

/**
 * Ensemble de zones
 */
class ZoneContainer: public sf::Drawable
{
public:
	enum MapName
	{
		// noms des conteneurs possibles
		WORLD, CAVES
	};

	ZoneContainer();

	~ZoneContainer();

	/**
	 * Charger les zones du conteneur
	 * @param[in] name: identifiant du conteneur à charger
	 */
	void Load(MapName name);

	/**
	 * Désallouer toutes les zones du conteneur
	 */
	void Unload();

	enum Direction
	{
		UP, DOWN, LEFT, RIGHT
	};

	/**
	 * Demander un changement de zone
	 * @param[in] dir: direction souhaitée
	 */
	void ChangeZone(Direction dir);

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

	/**
	 * Nom du conteneur
	 * @return nom-identifiant
	 */
	MapName GetName() const;

	/**
	 * Indique si le conteneur est en train de procéder à un scrolling
	 * @return true si scrolling, sinon false
	 */
	inline bool Scrolling() const
	{
		return scrolling_;
	}

	/**
	 * Mise à jour du conteneur
	 * @param[in] frametime: temps de la frame courante
	 */
	void Update(float frametime);

	/**
	 * Nombre de zones en largeur
	 */
	int GetWidth() const;

	/**
	 * Nombre de zones en hauteur
	 */
	int GetHeight() const;

	/**
	 * Coordonnées de la zone du joueur
	 */
	sf::Vector2i GetPlayerPosition() const;

	/**
	 * Récupérer une zone aux coordonnées
	 */
	const Zone* GetZoneAt(int x, int y) const;

private:
	/**
	 * Affichage
	 * @param[in, out] target: cible de rendu
	 */
	void Render(sf::RenderTarget& target) const;

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

	mutable Zone* active_zone_;
	sf::Vector2i cds_zone_; // coordonnées de la zone courante

	Zone* next_zone_;

	MapName name_;
	int width_; // largeur en nombre de zones
	int height_; // hauteur en nombre de zones
	Zone** zones_;

	TiXmlDocument xml_doc_;
};


#endif /* ZONECONTAINER_HPP */

