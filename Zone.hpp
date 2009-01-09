#ifndef ZONE_HPP
#define ZONE_HPP

#include <list>

#include "Entity.hpp"
#include "Item.hpp"

#ifdef DUMB_MUSIC
#include "Music.hpp"
#endif

/**
 * Une zone de jeu
 */
class Zone
{
public:
	enum
	{
		// Dimensions de la zone en nombre de tiles
		WIDTH = 20, HEIGHT = 16
	};

	enum TileContent
	{
		EMPTY,
		STATIC_NO,
		DYNAMIC_NO
	};

	Zone();

	~Zone();
	
	/**
	 * Charger les éléments de la zone
	 * @param[in] filename: fichier de la carte
	 */
	void Load(const char* filename, sf::RenderWindow& app);

	/**
	 * Mettre à jour la zone
	 * @param[in] frametime: temps de la frame courante
	 */
	void Update(float frametime);

	/**
	 * Afficher la zone de jeu
	 * @param[in] app: fenêtre de rendu
	 */
	void Show(sf::RenderWindow& app) const;

	/**
	 * Détermine si un mouvement est possible
	 * @param[in] emitter: entité qui cherche à se déplacer
	 * @param[in] rect: rectangle de la position issue du mouvement à tester
	 * @return true si le mouvement est possible, sinon false
	 */
	TileContent CanMove(Entity* emitter, const sf::FloatRect& rect, Item*& other);

	/**
	 * Ajouter une entité dans la zone de jeu
	 * @param[in, out] entity: entité ajoutée
	 */
	void AddEntity(Entity* entity);

	/**
	 * Retirer une entité de la zone de jeu
	 * @param[in] entity: entité a enlever
	 */
	void RemoveEntity(Entity* entity);

	/**
	 * Ajouter un objet statique dans la zone
	 * @param[in] i: position i de la tile
	 * @param[in] j: position j de la tile
	 */
	void PlaceStaticItem(int i, int j);
	
	void PlaceItem(char u, int i, int j);
	
	inline short GetMusic() const
	{
		return zone_music_index_;
	}
	
private:
	/**
	 * Désallouer toutes les entités de la zone
	 */
	typedef std::list<Entity*> EntityList;
	typedef std::list<Item*> ItemList;

	void Purge();

	mutable EntityList entities_;
	mutable ItemList interactives_;
	
	bool walkable_[HEIGHT][WIDTH];
	sf::Image tiles_; // tiles de la zone
#ifdef DUMB_MUSIC
	short zone_music_index_;
#endif
};

#endif /* guard ZONE_HPP */

