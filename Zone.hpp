#ifndef ZONE_HPP
#define ZONE_HPP

#include <list>

#include "Entity.hpp"

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
	bool CanMove(Entity* emitter, const sf::FloatRect& rect) const;

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
	
	inline short GetMusic() const
	{
		return zone_music_index_;
	}
	
private:
	/**
	 * Désallouer toutes les entités de la zone
	 */
	void Purge();

	typedef std::list<Entity*> EntityList;
	mutable EntityList entities_;
	
	bool walkable_[HEIGHT][WIDTH];
	sf::Image tiles_; // tiles de la zone
#ifdef DUMB_MUSIC
	short zone_music_index_;
#endif
};

#endif /* guard ZONE_HPP */

