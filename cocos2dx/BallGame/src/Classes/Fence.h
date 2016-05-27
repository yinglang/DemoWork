#ifndef __FENCE_H__
#define __FENCE_H__

#include "b2Sprite.h"
#include "BaseBall.h"

class Fence : public b2Sprite
{
public:
	Fence(b2World* world, const char * filename, Vec2 pos);
	Fence(b2World* world, Sprite* sprite);
	void broke();
	int life;		// 被铁球装多上次才能破坏
};

#endif 