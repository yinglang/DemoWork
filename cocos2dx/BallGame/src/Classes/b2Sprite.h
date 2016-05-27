#pragma once
#ifndef B2SPRITE_H
#define B2SPRITE_H
/* 建议继承该类，但是重写构造函数，复制粘贴修改即可 */
#include "cocos2d.h"
#include "Box2D\Box2D.h"
USING_NS_CC;

#ifndef RTM_RATIO
#define RTM_RATIO 32 
#endif

// MyUserData 的使用
/*
enum BodyType{ bd_b2Sprite, bd_count};
struct MyUserData(){
	BodyType bd_type;
	void* user;
	MyUserData(){
		bd_type = bd_count;
		user = nullptr;
	}
}
*/

class b2Sprite : public Ref
{
public:
	//static std::vector<b2Sprite*>* b2sprites;  
	// 存储所有的b2Sprite 用于同一updataWithBody() 和 delete(因为world有一个时间步内的destroy锁)
	//static void updateAllEnableb2SpritePosWithBody();	
private:
	//static void deleteEnableb2Sprites(int index);
	//static void removeb2Sprite(int index);
	//void trydelete();

	//int numinb2sprites; // 在numinb2sprites 中的编号
	//bool needDelete;	// 删除标志
public:
	void updatePosWithBody();		// 用物理引擎计算出的 body 结果更新 sprite 位置
	
	b2Sprite(b2World* world, const char* filename);
	b2Sprite(b2World* world, Sprite* sprite);
	b2Sprite(b2Body* body, Sprite* sprite);
	b2Sprite();

	//void deleteb2Sprite();

	// 由于b2world的时间步中有个锁，在一个时间步中不能删除body，所以不能在一个时间步执行中时使用world->destroyBody();
	// 比如在碰撞触发的函数里不能destroyBody(), 既不能调用该析构函数， 使b2Sprite的deleteb2Sprite()进行标记， 在
	// b2Sprite::deleteEnableb2Sprites()进行统一删除，这个函数会在b2Sprite::updateAllEnableb2SpritePosWithBody()中调用
	~b2Sprite();
public:

	Sprite* getSprite();
	b2Body* getBody();

	void destroyBody();  

	//该函数最好在body不空时使用
	void setBox2DEnable(bool);
	// void setSprite(Sprite* sprite);
	// void setBody(b2Body* body);
	virtual void setPosition(Vec2 pos);
	virtual void setPosition(float x, float y);

protected:
	b2Body* body;
	Sprite* sprite;
public:
	bool enableBox2d;
	void initB2Sprite(b2World* world, Sprite* sprite);
};
#endif

