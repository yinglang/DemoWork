#ifndef __GameChoose_H__
#define __GameChoose_H__
#include "b2Sprite.h"

/*	此文件中包含了所有的事件处理和物理引擎的引入	*/

/*  修改以下宏定义可以重定义类名，是否使用事件触发，物理引擎等等*/
// 定义类名
#if 1
// 不使用box2d引擎
//#define NOBOX2D
// 不使用box2d debug 线
//#define NOBOX2DDEBUG

// 不使用键盘点击事件
//#define NOKEYBOARDEVENT2
// 不使用鼠标点击事件
#define NOMOUSEEVENT
// 不使用单点触屏事件
//#define NOTOUCHONEBYONEEVENT
// 不使用多点触屏事件
#define NOTOUCHALLATONCEEVENT
// 不使用加速器
#define NOACCELERATIONEVENT
// 不使用自定义事件
#define NOCUSTOMEVENT

// 不使用绘图功能
//#define NODRAW

#include "cocos2d.h"
#include "Box2D\Box2D.h"

#ifndef NOBOX2DDEBUG
#include "GLES-Render.h"
#endif

USING_NS_CC;

#ifndef NOBOX2D
#ifndef RTM_RATIO
#define RTM_RATIO 32
#endif
#endif

#endif

class GameChoose : public Layer, b2ContactListener
{
public:
	GameChoose();
	~GameChoose();

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	static Scene* createScene();
	CREATE_FUNC(GameChoose);

	virtual void update(float dt);

	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);//override;

	Size visibleSize;
protected:
	int getSelectedScene(Vec2 pos);

	void initRoadLine();
	b2Body* addLine(Vec2 pos, float length, float angle = 0);
	b2Body* addCable(Vec2 pos, int num);
	std::vector<Sprite*>* cablesprites;

	std::vector<b2Sprite*>* cables;
	b2Body* baseline;

protected:
	void macroDefineInInit();	// init()中的相映宏的定义

/******************************************************************************************/
/* 一般情况下以下的代码除了world的定义，和debug线的flags的修改，以下代码不需修改 */

	/* 这里可以修改world的初始化 */
#if 1
#ifndef NOBOX2D
	// 物理引擎的碰撞监听,需继承b2ContactListener，可以查看他们的注释
public:
	virtual void BeginContact(b2Contact* contact);  // 四个里最先开时调用；且从刚开始接触时调用，直到这次接触脱离结束时，只调用那一次；发生在碰撞检测后
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);  // 第二个调用，在接触的期间，每帧都调用一次，This allows you to inspect a contact before it goes to the solver
	virtual void EndContact(b2Contact* contact);    // 第三个调用，在接触期间每帧调用一次，This lets you inspect a contact after the solver is finished
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse); // 最后调用；从接触开始，直到脱离接触时调用，之调用一次；发生在碰撞效果生成前
protected:
	b2World* world;
	void initPhysics();
#ifndef NOBOX2DDEBUG
	GLESDebugDraw* debugdraw;
public:
#endif // !NOBOX2DDEBUG

#endif 

#ifndef NOKEYBOARDEVENT2
protected:
	// 接受键盘的方向性输入
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
#endif

#ifndef NOMOUSEEVENT
protected:
	void onMouseDown(Event* event);
	void onMouseMove(Event* event);	//不需点击的移动
	void onMouseScroll(Event* event);
	void onMouseUp(Event* event);
#endif

#ifndef NOTOUCHONEBYONEEVENT
protected:
	bool onTouchBegan(Touch* touch, Event* event);		// 返回false事件不会向后传递
	void onTouchMoved(Touch* touch, Event* event);      // 点击着移动
	void onTouchCancelled(Touch* touch, Event* event);  // // 很难触发，比如玩着游戏突然来了电话
	void onTouchEnded(Touch* touch, Event* event); 
#endif

#ifndef NOTOUCHALLATONCEEVENT
	void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);		
	void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);      // 点击着移动
	void onTouchesCancelled(const std::vector<Touch*>& touches, Event* event);  // // 很难触发，比如玩着游戏突然来了电话
	void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);
#endif

#ifndef NOACCELERATIONEVENT
	void onAcceleration(Acceleration* acc, Event* event);
#endif

#ifndef NOCUSTOMEVENT
	void onEventCustom(EventCustom*event);
#endif

#ifndef NODRAW
protected:
	CustomCommand _customCmd;
public:
	void myDraw(const Mat4 &transform, uint32_t flags);

#endif
#endif

};

#endif

