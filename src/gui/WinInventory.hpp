#ifndef WININVENTORY_HPP
#define WININVENTORY_HPP

#include "../gui_system/Window.hpp"
#include "../core/InputController.hpp"

class Item;

class WinInventory: public gui::Window
{
public:
	WinInventory();
	~WinInventory();

	/** Retours de l'inventaire */
	enum
	{
		_CLOSE=101
	};

	/**
	 * Ajouter un objet dans l'inventaire
	 * @param item: objet à ajouter
	 * @return true si ok, false si inventaire plein ou doublon
	 */
	bool AddItem(Item* item);

	/**
	 * Savoir si un item est deja présent dans l'inventaire
	 * @return true s'il y est
	 */
	bool HasItem(int id);

	/**
	 * Gère les évenements claviers relatifs au curseur et à l'assignation des touches
	 * @param[in]event :l'évenement clavier
	 */
	void OnEvent(input::Action action);

	/**
	 * Vider l'inventaire
	 */
	void Clear();

	void SetItem1ID(int id);

	/**
	 * Permet de connaitre l'ID du premier objet assignable
	 * @return l'ID ou 0 si non assigné
	 */
	int GetItem1ID() const;

	void SetItem2ID(int id);

	 /**
	 * Permet de connaitre l'ID du deuxième objet assignable
	 * @return l'ID ou 0 si non assigné
	 */
	int GetItem2ID() const;

	void SetItem3ID(int id);

	/**
	 * Permet de connaitre l'ID du troisième objet assignable
	 * @return l'ID ou 0 si non assigné
	 */
	int GetItem3ID() const;

	/**
	 * Remplir le stock à partir d'une chaîne d'id
	 */
	void StockFromString(const std::string& str);

	/**
	 * Obtenir le contenu du stock sous forme d'une chaîne d'id
	 */
	std::string StockToString() const;

private:
	// inherited
	void Render(sf::RenderTarget&) const;

	void PlaceItem(Item* item, int x, int y);

	int WindowCallback(gui::Control::ControlID id);

	static void ClearItem(Item*& item);

	static void SetItem(Item*& item, int id);

	/** Identifiant du slot principale */
	enum
	{
		COUNT_W = 5, COUNT_H = 3
	};

	struct Cursor
	{
		sf::Sprite sprite;
		sf::Vector2i coords;
	};
	Cursor cursor_;

	Item* items_[COUNT_H][COUNT_W];

	Item* item1_;
	Item* item2_;
	Item* item3_;
};

#endif // WININVENTORY_HPP
