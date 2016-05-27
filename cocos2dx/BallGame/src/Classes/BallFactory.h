#ifndef __BALLFACTORY_H__
#define __BALLFACTORY_H__

#include "BaseBall.h"

class BallFactory
{
public:
	static BaseBall* createBall(b2World* world, BallType type);
	static BaseBall* createBall(b2World* world, Sprite* sprite, BallType type);
	static BaseBall* createBallWithOtherBallState(b2World* world, BallType, BaseBall*);
	BallFactory();
	~BallFactory();
};

#endif