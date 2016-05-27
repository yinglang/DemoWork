#pragma once
#ifndef B2SPRITE_H
#define B2SPRITE_H
/* ����̳и��࣬������д���캯��������ճ���޸ļ��� */
#include "cocos2d.h"
#include "Box2D\Box2D.h"
USING_NS_CC;

#ifndef RTM_RATIO
#define RTM_RATIO 32 
#endif

// MyUserData ��ʹ��
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
	// �洢���е�b2Sprite ����ͬһupdataWithBody() �� delete(��Ϊworld��һ��ʱ�䲽�ڵ�destroy��)
	//static void updateAllEnableb2SpritePosWithBody();	
private:
	//static void deleteEnableb2Sprites(int index);
	//static void removeb2Sprite(int index);
	//void trydelete();

	//int numinb2sprites; // ��numinb2sprites �еı��
	//bool needDelete;	// ɾ����־
public:
	void updatePosWithBody();		// ���������������� body ������� sprite λ��
	
	b2Sprite(b2World* world, const char* filename);
	b2Sprite(b2World* world, Sprite* sprite);
	b2Sprite(b2Body* body, Sprite* sprite);
	b2Sprite();

	//void deleteb2Sprite();

	// ����b2world��ʱ�䲽���и�������һ��ʱ�䲽�в���ɾ��body�����Բ�����һ��ʱ�䲽ִ����ʱʹ��world->destroyBody();
	// ��������ײ�����ĺ����ﲻ��destroyBody(), �Ȳ��ܵ��ø����������� ʹb2Sprite��deleteb2Sprite()���б�ǣ� ��
	// b2Sprite::deleteEnableb2Sprites()����ͳһɾ���������������b2Sprite::updateAllEnableb2SpritePosWithBody()�е���
	~b2Sprite();
public:

	Sprite* getSprite();
	b2Body* getBody();

	void destroyBody();  

	//�ú��������body����ʱʹ��
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

