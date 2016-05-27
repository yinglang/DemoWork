#ifndef __BASEBALL_H__
#define __BASEBALL_H__
#include "b2Sprite.h"

// Type和后面的imagepath 一一顺序对应

enum BallType{ 
	b_baseball_mid,
	b_baseball_small,
	b_eggball_mid,
	b_eggball_small,
	b_electricball_mid,
	b_electricballball_small,
	b_ghostball_mid,
	b_ghostball_small,
	b_glassball_mid,
	b_glassball_small,
	b_ironball_mid,
	b_ironball_small,
	b_ungravityball_mid,
	b_ungravityball_small,
	b_virusball_mid,
	b_virusball_small,

	b_doubleball_mid,
	b_doubleball_small,
	b_count
};

enum HoleType{
	h_basehole_mid,
	h_basehole_small,
	h_egghole_mid,
	h_egghole_small,
	h_electrichole_mid,
	h_electrichole_small,
	h_ghosthole_mid,
	h_ghosthole_small,
	h_glasshole_mid,
	h_glasshole_small,
	h_ironhole_mid,
	h_ironhole_small,
	h_ungravityhole_mid,
	h_ungravityhole_small,
	h_virushole_mid,
	h_virushole_small,

	h_blackhole_mid,
	h_blackhole_small,
	h_count
};

enum BodyType{ bd_ground, bd_ball, bd_fence,bd_count };

struct MyUserData{
	BodyType bd_type;
	BallType b_type;
	void* user;

	MyUserData(){
		bd_type = bd_ground;
		b_type = b_count;
		user = nullptr;
	}
};

class BaseBall : public b2Sprite
{
public:
	static std::string* ballimagepath;
	static std::string* holeimagepath;

	BaseBall(b2World* world, const char* filename, BallType _type);
	BaseBall(b2World* world, Sprite* sprite, BallType _type);
	~BaseBall();
	void applyForce(Vec2 force,bool musthaveeffect = false);
	void applyKeepForce(bool* pressed);

	void inHole(Vec2, int& lock);

	void onContact();	//碰撞时的动作

	float getVelocityValue(Vec2 direction = Vec2::ZERO); // 不给参数，返回总速度值

	BallType type;
	float radius;

	// 电球属性
	float effectRadius; // 作用力半径
protected:
	enum DIrection{ UP, DOWN, LEFT, RIGHT };

};

#endif
