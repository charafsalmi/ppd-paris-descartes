#include "Animated.hpp"


Animated::Animated(const Animation* animation, sf::Sprite& sprite)
{
	animation_ = animation;
	timer_ = animation_->GetDelay();
	frame_ = 0;
	loop_ = true;
	stopped_ = false;
	sprite.SetSubRect(animation_->GetFrame(0));
}


void Animated::Change(const Animation* animation, sf::Sprite& sprite)
{
	animation_ = animation;
	timer_ = animation_->GetDelay();
	frame_ = 0;
	
	sprite.SetSubRect(animation_->GetFrame(0));
}


void Animated::Update(float frametime, sf::Sprite& sprite)
{
	if (stopped_)
	{
		return;
	}
	
	timer_ -= frametime;
	
	if (timer_ <= 0)
	{
		timer_ = animation_->GetDelay();
		frame_ = (frame_ + 1) % animation_->GetSize();
		if (!loop_ && frame_ == 0)
		{
			stopped_ |= 1;
		}
		sprite.SetSubRect(animation_->GetFrame(frame_));
	}
}

bool Animated::operator==(const Animation& other)
{
	return ((this->animation_) == other);
}
