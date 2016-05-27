#include "BallFactory.h"

BaseBall* BallFactory::createBall(b2World* world, BallType type){
	if (type < b_count){
		BaseBall* ball = new BaseBall(world, BaseBall::ballimagepath[type].c_str(), type);
		if (type == b_ghostball_mid || type == b_ghostball_small){
			b2Filter filter;
			filter.categoryBits = 0x0004;
			filter.maskBits = 0xfff9;
			ball->getBody()->GetFixtureList()->SetFilterData(filter);
			ball->getSprite()->setOpacity(125);
		}

		else if (type == b_ungravityball_mid || type == b_ungravityball_small){
			ball->getBody()->SetGravityScale(0);
		}
		return ball;
	}
	return nullptr;
}

BaseBall* BallFactory::createBall(b2World* world, Sprite* sprite, BallType type){
	if (type < b_count){
		BaseBall* ball = new BaseBall(world, sprite, type);
		if (type == b_ghostball_mid || type == b_ghostball_small){
			b2Filter filter;
			filter.categoryBits = 0x0004;
			filter.maskBits = 0xfff9;
			ball->getBody()->GetFixtureList()->SetFilterData(filter);
			ball->getSprite()->setOpacity(125);
		}

		else if (type == b_ungravityball_mid || type == b_ungravityball_small){
			ball->getBody()->SetGravityScale(0);
		}
		return ball;
	}
	return nullptr;
}

BaseBall* BallFactory::createBallWithOtherBallState(b2World* world, BallType type, BaseBall* ball){
	auto newball = createBall(world, type);
	if (ball){
		newball->getBody()->SetTransform(ball->getBody()->GetPosition(),ball->getBody()->GetAngle());
		newball->setPosition(ball->getSprite()->getPosition());
		newball->getBody()->SetLinearVelocity(ball->getBody()->GetLinearVelocity());
		newball->getBody()->SetAngularVelocity(ball->getBody()->GetAngularVelocity());
	}
	return newball;
}

BallFactory::BallFactory()
{
}


BallFactory::~BallFactory()
{
}
