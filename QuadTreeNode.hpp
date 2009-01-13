#ifndef QUADTREENODE_HPP
#define QUADTREENODE_HPP

// QuadTreeNode.h: interface for the CQuadTreeNode class.
//
//////////////////////////////////////////////////////////////////////
#include <set>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Entity.hpp"


class QuadTreeNode  
{
public:

	enum Status
	{
		OKAY,
		FAILURE
	};
	
	QuadTreeNode(sf::Vector2i LowerLeft, sf::Vector2i Size);
	~QuadTreeNode();

	int Subdivide();
	
	int AddEntity(Entity* entity);
	int Purge();
	
	int GetSize(sf::Vector2i* ll, sf::Vector2i* size);
	
	int GetEntities(Entity* object, std::set<Entity*>& entities);
	
	int RemoveEntity(Entity* entity);
	int UpdateEntity(Entity* entity);

private:
	sf::Vector2i lower_left_;
	sf::Vector2i size_;

	QuadTreeNode** subnodes_;

	typedef std::set<Entity*> EntitySet;
	EntitySet entities_;
};

#endif /* guard QUADTREENODE_HPP */

