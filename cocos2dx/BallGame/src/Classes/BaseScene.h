#ifndef __BASESCENE_H__
#define __BASESCENE_H__
#include "MyTool.h"


/*	���ļ��а��������е��¼�������������������	*/

/*  �޸����º궨������ض����������Ƿ�ʹ���¼���������������ȵ�*/
// ��������
#if 1
#define BaseScene GameScene
// ��ʹ��box2d����
//#define NOBOX2D1
// ��ʹ��box2d debug ��
//#define NOBOX2DDEBUG1

// ��ʹ�ü��̵���¼�
//#define NOKEYBOARDEVENT1
// ��ʹ��������¼�
#define NOMOUSEEVENT
// ��ʹ�õ��㴥���¼�
//#define NOTOUCHONEBYONEEVENT1
// ��ʹ�ö�㴥���¼�
#define NOTOUCHALLATONCEEVENT
// ��ʹ�ü�����
//#define NOACCELERATIONEVENT1
//��ʹ���Զ����¼�
#define NOCUSTOMEVENT

// ��ʹ�û�ͼ����
#define NODRAW

#include "cocos2d.h"
#include "Box2D\Box2D.h"

#ifndef NOBOX2DDEBUG
#include "GLES-Render.h"
#endif

#ifndef NODRAW
#include "VisibleRect.h"	// ���Ǳ���ģ���ֻ�Ƕ����˼���Point������demo����ʹ�õ��ˣ�����ɾ��
#endif

USING_NS_CC;

#ifndef NOBOX2D
#ifndef RTM_RATIO
#define RTM_RATIO 32
#endif
#endif

#endif

#define NODESIGNER

// Ϊ����Action��CallFunc��ʹ���ⲿ����������������Ϊȫ�ֵ�ʹ�ã��������һ���н�
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

	static int lock;		// ���������ڽ���ʱ������+1������������ʱ����-1, ֻ��Ϊ��ʱ������gameover����nextScene()
	void nextScene();
	void gameover();
	bool checkOver();	// ����Ƿ���Ϸ����

	// ��ײ�¼�
	void ironContactFence(void* userdata); // ����������ɺ,�ƻ���ɺ
	void virusContactBall(void* userdata); // ���������������򣬽��������ɲ�����
	void glassContact(void* dataA, void*dataB);	 // ��������ײ������դ������������������飬remove()
	void bigCollision(Ref* bA, Ref* bB);	 // ������������ٶȺܴ���ײ����100��
	void dealafterContact();

	// // ����������ͳ�������
	void electricBallAreaEffect();

protected:
	void macroDefineInInit();	// init()�е���ӳ��Ķ���

#ifndef NODESIGNER
	std::vector<Vec2>* points;
#endif

/******************************************************************************************/
/* һ����������µĴ������world�Ķ��壬��debug�ߵ�flags���޸ģ����´��벻���޸� */

	/* ��������޸�world�ĳ�ʼ�� */
#if 1
#ifndef NOBOX2D1
	// �����������ײ����,��̳�b2ContactListener�����Բ鿴���ǵ�ע��
public:
	virtual void BeginContact(b2Contact* contact);  // �ĸ������ȿ�ʱ���ã��ҴӸտ�ʼ�Ӵ�ʱ���ã�ֱ����νӴ��������ʱ��ֻ������һ�Σ���������ײ����
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);  // �ڶ������ã��ڽӴ����ڼ䣬ÿ֡������һ�Σ�This allows you to inspect a contact before it goes to the solver
	virtual void EndContact(b2Contact* contact);    // ���������ã��ڽӴ��ڼ�ÿ֡����һ�Σ�This lets you inspect a contact after the solver is finished
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse); // �����ã��ӽӴ���ʼ��ֱ������Ӵ�ʱ���ã�֮����һ�Σ���������ײЧ������ǰ
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
	// ���ܼ��̵ķ���������
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
#endif

#ifndef NOMOUSEEVENT
protected:
	void onMouseDown(Event* event);
	void onMouseMove(Event* event);	//���������ƶ�
	void onMouseScroll(Event* event);
	void onMouseUp(Event* event);
#endif

#ifndef NOTOUCHONEBYONEEVENT1
protected:
	bool onTouchBegan(Touch* touch, Event* event);		// ����false�¼�������󴫵�
	void onTouchMoved(Touch* touch, Event* event);      // ������ƶ�
	void onTouchCancelled(Touch* touch, Event* event);  // // ���Ѵ���������������ϷͻȻ���˵绰
	void onTouchEnded(Touch* touch, Event* event); 
#endif

#ifndef NOTOUCHALLATONCEEVENT
	void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);		
	void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);      // ������ƶ�
	void onTouchesCancelled(const std::vector<Touch*>& touches, Event* event);  // // ���Ѵ���������������ϷͻȻ���˵绰
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

