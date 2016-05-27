#ifndef __HOLE_H__
#define __HOLE_H__

#include "cocos2d.h"
#include "BaseBall.h"
USING_NS_CC;

class Hole
{
public:
	Hole();
	Hole(HoleType);
	Hole(Sprite* sprite, HoleType);
	~Hole();

	void ballIn(int& lock);
	Sprite* sprite;
	HoleType type;
	float holesize;
};

#endif