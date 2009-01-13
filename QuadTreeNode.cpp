#include "QuadTreeNode.hpp"



QuadTreeNode::QuadTreeNode(sf::Vector2i LowerLeft, sf::Vector2i Size) :
	lower_left_(LowerLeft), 
	size_(Size),
	subnodes_(NULL)
{
}
	
QuadTreeNode::~QuadTreeNode()
{
	if (!subnodes_)
	{
		EntitySet::iterator it = entities_.begin();
		while (it != entities_.end())
		{
			//delete (*it);		
			it++;
		}
		entities_.clear();
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			delete subnodes_[i];
		}
		delete subnodes_;
		if (!entities_.empty())
		{
			abort();
		}
	}
}

int QuadTreeNode::Subdivide()
{
	int result = QuadTreeNode::OKAY;
	if (!subnodes_)
	{
		// new size
		int w = size_.x / 2;
		int h = size_.y / 2;

		// lower x, y
		int llx = lower_left_.x;
		int lly = lower_left_.y;

		// node 0: upper left quadrant
		// node 1: upper right quadrant
		// node 2: lower right quadrant
		// node 3: lower left quaderant
		subnodes_ = new QuadTreeNode*[4];
		subnodes_[0] = new QuadTreeNode(sf::Vector2i(llx, lly + h), sf::Vector2i(w, h));
		subnodes_[1] = new QuadTreeNode(sf::Vector2i(llx + w, lly + h), sf::Vector2i(w, h));
		subnodes_[2] = new QuadTreeNode(sf::Vector2i(llx + w, lly), sf::Vector2i(w, h));
		subnodes_[3] = new QuadTreeNode(sf::Vector2i(llx, lly), sf::Vector2i(w, h));
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			result = subnodes_[i]->Subdivide();
		}
	}
	return result;
}

int QuadTreeNode::AddEntity(Entity* entity)
{
	int result = QuadTreeNode::OKAY;
	// If this node has subnodes, figure out which subnode this object beints
	// in and add it to him, otherwise, just add it to our list
	if (!subnodes_)
	{
		// no subnodes, this is a leaf node, so add it to the list
		std::pair<EntitySet::iterator, bool> newInsertion;
		newInsertion = entities_.insert(entity);
		if (newInsertion.second)
		{
			// only add ref if this is object is not already in our set,
			// otherwise we will over ref count it.
			// entity->AddRef();
		}
		else
		{
			// they should never add the same object to us twice
			abort();
		}
	}
	else
	{
		// the object may need to go in more than one sub node if it
		// spans multiple quadrants
		int collisionArray[4] = { -1, -1, -1, -1 };
		int collisionIndex = 0;

		sf::FloatRect floor_rect;
		entity->GetFloorRect(floor_rect);


		int lowerLeftX = 0;
		int lowerLeftY = 0;
		int width = 0;
		int height = 0;

		width = floor_rect.GetWidth();
		height = floor_rect.GetHeight();
		
		sf::Vector2f pos;
		pos = entity->GetPosition();
		sf::Vector2i r_pos;
		r_pos.x = ::abs(pos.x);
		r_pos.y = ::abs(pos.y);

		lowerLeftX = r_pos.x - width;
		lowerLeftY = r_pos.y - height;
;
		sf::Vector2i ll, size;
		for (int i = 0; i < 4; i++)
		{
			result = subnodes_[i]->GetSize(&ll, &size);
			int left1, left2;
			int right1, right2;
			int top1, top2;
			int bottom1, bottom2;

			left1 = lowerLeftX;
			left2 = ll.x;
			right1 = lowerLeftX + width;
			right2 = ll.x + size.x;
			bottom1 = lowerLeftY;
			bottom2 = ll.y;
			top1 = lowerLeftY + height;
			top2 = ll.y + size.y;
			if (bottom1 > top2 || top1 < bottom2 || right1 < left2 || left1 > right2)
			{
				continue;
			}
			else
			{
				collisionArray[collisionIndex++] = i;
			}
		}
		for (int j = 0; j < collisionIndex; j++)
		{
			subnodes_[collisionArray[j]]->AddEntity(entity);
		}
	}
	return result;
}

int QuadTreeNode::Purge()
{
	int result = QuadTreeNode::OKAY;
	if (!subnodes_)
	{
		EntitySet::iterator it = entities_.begin();
		while (it != entities_.end())
		{
			//delete (*it);		
			it++;
		}
		entities_.clear();
	}
	else
	{
		 for (int i = 0; i < 4; i++)
		 {
			result = subnodes_[i]->Purge();
		}
		if (!entities_.empty())
		{
			abort();
		}
	}
	return result;
}

int QuadTreeNode::GetSize(sf::Vector2i* ll, sf::Vector2i* size)
{
	*ll = lower_left_;
	*size = size_;
	return QuadTreeNode::OKAY;
}

// if moving is true, caller should have moved the object forward a time step before
// calling this function

int QuadTreeNode::GetEntities(Entity* entity, std::set<Entity*>& collidables)
{
		int result = QuadTreeNode::OKAY;
		// If this node has subnodes, figure out which subnode this object beints
		// in and add it to him, otherwise, just add it to our list
		if (!subnodes_)
		{
			// no subnodes, this is a leaf node, so return entire list
			EntitySet::iterator it = entities_.begin();
			while (it != entities_.end())
			{
				// exclude the actual object they are passing in, since they want objects
				// other than this object in the area.
				if ( (entity != NULL &&*it != entity) || entity == NULL)
				{
					collidables.insert(*it);
				}
				it++;
			}
		}
		else
		{
			// the object may need to go in more than one sub node if it
			// spans multiple quadrants

			int lowerLeftX, lowerLeftY, collisionIndex;
			int collisionArray[4] = { -1, -1, -1, -1 };
			int width, height;

			// create bounding box around circular object
			std::set<int> collisionSave;
			
			if (entity != NULL)
			{
				sf::FloatRect floor_rect;
				entity->GetFloorRect(floor_rect);
		
				width = floor_rect.GetWidth();
				height = floor_rect.GetHeight();
		


				sf::Vector2f pos;
				pos = entity->GetPosition();
				sf::Vector2i r_pos;
				r_pos.x = ::abs(pos.x);
				r_pos.y = ::abs(pos.y);

				lowerLeftX = r_pos.x - width;
				lowerLeftY = r_pos.y - height;
				collisionIndex = 0;
			}
			
			sf::Vector2i ll, size;
			for (int i = 0; i < 4; i++)
			{
				result = subnodes_[i]->GetSize(&ll, &size);
				
				if (entity != NULL)
				{
					int left1, left2;
					int right1, right2;
					int top1, top2;
					int bottom1, bottom2;

					left1 = lowerLeftX;
					left2 = ll.x;
					right1 = lowerLeftX + width;
					right2 = ll.x + size.x;
					bottom1 = lowerLeftY;
					bottom2 = ll.y;
					top1 = lowerLeftY + height;
					top2 = ll.y + size.y;
					if (bottom1 > top2 || top1 < bottom2 || right1 < left2 || left1 > right2)
					{
						continue;
					}
					else
					{
						collisionArray[collisionIndex++] = i;
					}
					for (int j = 0; j < collisionIndex; j++)
					{
						collisionSave.insert(collisionArray[j]);
					}	

				}
				else
				{
					collisionArray[collisionIndex++] = i;
					for (int j = 0; j < collisionIndex; j++)
					{
						collisionSave.insert(collisionArray[j]);
					}						
				}

					for (int j = 0; j < collisionIndex; j++)
					{

					/*if (k == 1)
					{
						if (collisionSave.find(collisionArray[j]) != collisionSave.end())
						{
							continue;
						}
					}*/
					EntitySet subNodeEntities;
					subnodes_[collisionArray[j]]->GetEntities(entity, subNodeEntities);
					EntitySet::iterator it = subNodeEntities.begin();
					while (it != subNodeEntities.end())
					{
						collidables.insert(*it);
						it++;
					}
				}
			}
			//cobj1->Release();
	}
	return result;
}


int QuadTreeNode::RemoveEntity(Entity* entity)
{
	int result = QuadTreeNode::OKAY;
	// If this node has subnodes, figure out which subnode this object beints
	// in and remove it from him, otherwise, just remove it to our list
	if (!subnodes_)
	{
		// no subnodes, this is a leaf node, so remove it
		EntitySet::iterator it = entities_.find(entity);
		if (it != entities_.end())
		{
			//delete (*it);
			entities_.erase(it);
		}
	}
	else
	{
		// the object may need to be removed from more than one sub node if it
		// spans multiple quadrants
		int collisionArray[4] = { -1, -1, -1, -1 };
		int collisionIndex = 0;
#ifdef _DEBUG
		int noncollisionArray[4] = { -1, -1, -1, -1 };
		int noncollisionIndex = 0;
#endif


		sf::FloatRect floor_rect;
		entity->GetFloorRect(floor_rect);

		int width, height;
		
		width = floor_rect.GetWidth();
		height = floor_rect.GetHeight();
		
		sf::Vector2f pos;
		pos = entity->GetPosition();
		sf::Vector2i r_pos;
		r_pos.x = ::abs(pos.x);
		r_pos.y = ::abs(pos.y);

		int lowerLeftX = r_pos.x - width;
		int lowerLeftY = r_pos.y - height;

		sf::Vector2i ll, size;
		for (int i = 0; i < 4; i++)
		{
			result = subnodes_[i]->GetSize(&ll, &size);
			int left1, left2;
			int right1, right2;
			int top1, top2;
			int bottom1, bottom2;

			left1 = lowerLeftX;
			left2 = ll.x;
			right1 = lowerLeftX + width;
			right2 = ll.x + size.x;
			bottom1 = lowerLeftY;
			bottom2 = ll.y;
			top1 = lowerLeftY + height;
			top2 = ll.y + size.y;
			if (bottom1 > top2 || top1 < bottom2 || right1 < left2 || left1 > right2)
			{
#ifdef _DEBUG
				noncollisionArray[noncollisionIndex++] = i;
#endif
				continue;
			}
			else
			{
				collisionArray[collisionIndex++] = i;
			}
		}
		for (int j = 0; j < collisionIndex; j++)
		{
			result = subnodes_[collisionArray[j]]->RemoveEntity(entity);
		}
#ifdef _DEBUG
		// validate that we do NOT find this object in subnodes where it should not be, since
		// it is outside of us (the parents) bounds, it should not be in our subnodes.
		for (j = 0; j < noncollisionIndex; j++)
		{
			result = subnodes_[noncollisionArray[j]]->RemoveEntity(entity);
		}
#endif
	}
	return result;
}

// Call if an object has moved and needs to be updated in the quad tree
int QuadTreeNode::UpdateEntity(Entity* entity)
{
	int result = QuadTreeNode::OKAY;
	if (!subnodes_)
	{
		// this function should only be called on a quad tree node with sub nodes
		abort();	}
	else
	{
		// first remove the object from subnodes based on its previous position, then add it
		// back with current position.
		for (int i = 0; i < 4; i++)
		{
			result = subnodes_[i]->RemoveEntity(entity);
		}
		for (int i = 0; i < 4; i++)
		{
			result = subnodes_[i]->AddEntity(entity);
		}
	}
	return result;
}
