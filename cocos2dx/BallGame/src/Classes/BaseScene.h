#ifndef __BASESCENE_H__
#define __BASESCENE_H__
#include "MyTool.h"


/*	此文件中包含了所有的事件处理和物理引擎的引入	*/

/*  修改以下宏定义可以重定义类名，是否使用事件触发，物理引擎等等*/
// 定义类名
#if 1
#define BaseScene GameScene
// 不使用box2d引擎
//#define NOBOX2D1
// 不使用box2d debug 线
//#define NOBOX2DDEBUG1

// 不使用键盘点击事件
//#define NOKEYBOARDEVENT1
// 不使用鼠标点击事件
#define NOMOUSEEVENT
// 不使用单点触屏事件
//#define NOTOUCHONEBYONEEVENT1
// 不使用多点触屏事件
#define NOTOUCHALLATONCEEVENT
// 不使用加速器
//#define NOACCELERATIONEVENT1
//不使用自定义事件
#define NOCUSTOMEVENT

// 不使用绘图功能
#define NODRAW

#include "cocos2d.h"
#include "Box2D\Box2D.h"

#ifndef NOBOX2DDEBUG
#include "GLES-Render.h"
#endif

#ifndef NODRAW
#include "VisibleRect.h"	// 不是必需的，它只是定义了几个Point，但是demo测试使用到了，懒的删了
#endif

USING_NS_CC;

#ifndef NOBOX2D
#ifndef RTM_RATIO
#define RTM_RATIO 32
#endif
#endif

#endif

#define NODESIGNER

// 为了在Action的CallFunc中使用外部变量，将数据声明为全局的使用，该类就是一个中介
struct ActionData{ 
	int intdata;
	void* pointdata;
	void* pointdata2;
	ActionData(){
		intdata = 0;
		pointdata = nullptr;
		pointdata2 = nullptr;
	}
};

class BaseScene : public Layer, b2ContactListener
{
	friend  class SceneDesigner;
public:
	BaseScene();
	~BaseScene();

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	static Scene* createScene();
	CREATE_FUNC(BaseScene);

	virtual void update(float dt);

	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);//override;

	void particleTest();
	ParticleSystemQuad* createParticle(int type);
	ParticleSystemQuad* createComplexParticle(int type);
	int particletype;
	cocos2d::ParticleSystemQuad*  _emitter;

	static int scenenum;
protected:
	Size visibleSize;
	enum DIrection{UP,DOWN,LEFT, RIGHT};
	bool pressed[4];

	float score;
	LabelTTF* scoreLabel;
	Vec2 scoreLabelPos;

	ActionData actiondata;

	void addEdge(Vec2 startpoint, Vec2 endpoint);
	void ballInRightHole();
	void addScore(int score, Vec2 pos);

	static int lock;		// 当动画正在进行时，上锁+1；当动画结束时解锁-1, 只有为零时，才能gameover或者nextScene()
	void nextScene();
	void gameover();
	bool checkOver();	// 检查是否游戏结束

	// 碰撞事件
	void ironContactFence(void* userdata); // 铁球碰到阑珊,破坏阑珊
	void virusContactBall(void* userdata); // 病毒球碰到其他球，将其他球变成病毒球
	void glassContact(void* dataA, void*dataB);	 // 玻璃球碰撞到除了栅栏外的其他东西，破碎，remove()
	void bigCollision(Ref* bA, Ref* bB);	 // 两个球以相对速度很大碰撞，加100分
	void dealafterContact();

	// // 电球的引力和斥力作用
	void electricBallAreaEffect();

protected:
	void macroDefineInInit();	// init()中的相映宏的定义

#ifndef NODESIGNER
	std::vector<Vec2>* points;
#endif

/******************************************************************************************/
/* 一般情况下以下的代码除了world的定义，和debug线的flags的修改，以下代码不需修改 */

	/* 这里可以修改world的初始化 */
#if 1
#ifndef NOBOX2D1
	// 物理引擎的碰撞监听,需继承b2ContactListener，可以查看他们的注释
public:
	virtual void BeginContact(b2Contact* contact);  // 四个里最先开时调用；且从刚开始接触时调用，直到这次接触脱离结束时，只调用那一次；发生在碰撞检测后
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);  // 第二个调用，在接触的期间，每帧都调用一次，This allows you to inspect a contact before it goes to the solver
	virtual void EndContact(b2Contact* contact);    // 第三个调用，在接触期间每帧调用一次，This lets you inspect a contact after the solver is finished
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse); // 最后调用；从接触开始，直到脱离接触时调用，之调用一次；发生在碰撞效果生成前
protected:
	b2World* world;
	void initPhysics();
#ifndef NOBOX2DDEBUG1
	GLESDebugDraw* debugdraw;
public:
#endif // !NOBOX2DDEBUG

#endif 

#ifndef NOKEYBOARDEVENT1
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

#ifndef NOTOUCHONEBYONEEVENT1
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

#ifndef NOACCELERATIONEVENT1
	void onAcceleration(Acceleration* acc, Event* event);
#endif

#ifndef NOCUSTOMEVENT
	void onEventCustom(EventCustom*event);
#endif

#ifndef NODRAW
protected:
#if define NOBOX2D
#if define NOBOX2DDEBUG
	GLESDebugDraw* debugdraw;
#endif
#endif
	CustomCommand _customCmd;
public:
	void myDraw(const Mat4 &transform, uint32_t flags);
#endif

#endif

};

#endif

