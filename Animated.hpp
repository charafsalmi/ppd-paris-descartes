#ifndef ANIMATED_HPP
#define ANIMATED_HPP

#include <SFML/Graphics.hpp>

#include "Animation.hpp"

/**
 * Interface de sprite animé
 */
class Animated
{
public:
	Animated(const Animation* animation, sf::Sprite& sprite);

	/**
	 * Mettre à jour le subrect d'un sprite animé
	 * @param[in] framtime: temps de la frame courante
	 * @param[out] sprite: sprite à mettre à jour
	 */
	 
	void Change(const Animation* animation, sf::Sprite& sprite);
	void Update(float frametime, sf::Sprite& sprite);

	
	inline void Start()
	{
		stopped_ = false;
	}

	inline void Halt()
	{
		stopped_ = true;
	}

	inline bool Halted()
	{
		return stopped_;
	}
	
	inline void SetLoop(bool mode)
	{
		loop_ = mode;
	}
	
	inline bool GetLoop()
	{
		return loop_;
	}
	
	
private:
	Animation* animation_;
	int frame_;
	float timer_;
	bool stopped_, loop_;
};

#endif /* ANIMATED_HPP */

