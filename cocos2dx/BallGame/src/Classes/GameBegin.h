#ifndef __GAMEBEGIN_H__
#define __GAMEBEGIN_H__

#include "cocos2d.h"

USING_NS_CC;

class GameBegin : public Layer
{
public:
	GameBegin();
	~GameBegin();

	virtual bool init();
	virtual void onEnter();
	static Scene* createScene();
	CREATE_FUNC(GameBegin);
	Size visibleSize;

	virtual void update(float dt);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	bool toChooseScene(Touch* t, Event* e);
};

#endif