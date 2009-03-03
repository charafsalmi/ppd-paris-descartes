#include "Animation.hpp"


Animation::Animation()
{
	delay_ = 1.0f;
}


void Animation::AddFrame(const sf::IntRect& subrect)
{
	subrects_.push_back(subrect);
}


void Animation::AddFrame(int left, int top, int width, int height)
{
	AddFrame(sf::IntRect(left, top, left + width, top + height));
}


void Animation::SetDelay(float delay)
{
	delay_ = delay;
}

bool operator==(const Animation& my, const Animation& other)
{
	unsigned short i, j;
	for (i = 0, j = 0;
		i < my.subrects_.size() && j != other.subrects_.size();
		++ i, ++ j)
	{
		if (my.subrects_[i].Left != other.subrects_[i].Left)
			return false;
		if (my.subrects_[i].Right != other.subrects_[i].Right)
			return false;
		if (my.subrects_[i].Top != other.subrects_[i].Top)
			return false;
		if (my.subrects_[i].Bottom != other.subrects_[i].Bottom)
			return false;
	}
	if (i == j)
	{
		return true;
	}
	return false;
}


bool operator!=(const Animation& my, const Animation& other)
{
	return (!(my == other));
}


bool operator==(const Animation* my, const Animation& other)
{
	return ((*my) == other);
}


bool operator!=(Animation* my, const Animation& other)
{
	return (!((*my) == other));
}

