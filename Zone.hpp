#ifndef ZONE_HPP
#define ZONE_HPP

#include <list>

#include "Entity.hpp"
#include "Item.hpp"
#include "Tileset.hpp"

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
	 * Ajouter une entité dans la zone de jeu
	 * @param[in, out] entity: entité ajoutée
	 */
	void AddEntity(Entity* entity);
	
	/**
	 * Retirer une entité de la zone de jeu
	 * @param[in] entity: entité à enlever
	 */
	void RemoveEntity(Entity* entity);
	
	/**
	 * Détermine si un mouvement est possible
	 * @param[in] emitter: entité qui cherche à se déplacer
	 * @param[in] rect: rectangle de la position issue du mouvement à tester
	 * @return true si le mouvement est possible, sinon false
	 */
	bool CanMove(Entity* emitter, const sf::FloatRect& rect);
	
	/**
	 * Ajouter un objet
	 */
	void AddItem(char id, int x, int y);

#ifdef DUMB_MUSIC
	inline int GetMusic() const
	{
		return zone_music_index_;
	}
#endif

	inline int GetTileAt(int x, int y)
	{
		return tiles_[y][x];
	}
	
	int GetEffectArg(int tile_id)
	{
		Tileset::EffectIter it = special_args_.find(tile_id);
		if (it != special_args_.end())
		{
			return special_args_[tile_id];
		}
		return -1;
	}
	
	/**
	 * Obtenir l'image des tiles du décor
	 */
	const sf::Image* GetBackground() const;
	
private:
	/**
	 * Ajouter une entité dans la zone de jeu
	 * @param[in] name: identifiant
	 * @param[in] x: tile x
	 * @param[in] y: tile y
	 */
	void AddEntity(const char* name, int x, int y);
	
	/**
	 * Désallouer toutes les entités et tous les items de la zone
	 */
	void Purge();
	
	typedef std::list<Entity*> EntityList;
	typedef std::list<Item*> ItemList;
	
	mutable EntityList entities_;
	ItemList items_;
	
	Tileset::EffectArgs special_args_;	// arguments des tiles speciales de la zone
	
	int tiles_[HEIGHT][WIDTH];
	bool walkable_[HEIGHT][WIDTH];
	sf::Image tiles_img_; // image des tiles de la zone
	sf::Sprite tiles_sprite_; // sprite associé aux tiles
#ifdef DUMB_MUSIC
	int zone_music_index_;
#endif
};

#endif /* ZONE_HPP */

