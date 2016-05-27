#ifndef __GameChoose_H__
#define __GameChoose_H__
#include "b2Sprite.h"

/*	���ļ��а��������е��¼�������������������	*/

/*  �޸����º궨������ض����������Ƿ�ʹ���¼���������������ȵ�*/
// ��������
#if 1
// ��ʹ��box2d����
//#define NOBOX2D
// ��ʹ��box2d debug ��
//#define NOBOX2DDEBUG

// ��ʹ�ü��̵���¼�
//#define NOKEYBOARDEVENT2
// ��ʹ��������¼�
#define NOMOUSEEVENT
// ��ʹ�õ��㴥���¼�
//#define NOTOUCHONEBYONEEVENT
// ��ʹ�ö�㴥���¼�
#define NOTOUCHALLATONCEEVENT
// ��ʹ�ü�����
#define NOACCELERATIONEVENT
// ��ʹ���Զ����¼�
#define NOCUSTOMEVENT

// ��ʹ�û�ͼ����
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
	void macroDefineInInit();	// init()�е���ӳ��Ķ���

/******************************************************************************************/
/* һ����������µĴ������world�Ķ��壬��debug�ߵ�flags���޸ģ����´��벻���޸� */

	/* ��������޸�world�ĳ�ʼ�� */
#if 1
#ifndef NOBOX2D
	// �����������ײ����,��̳�b2ContactListener�����Բ鿴���ǵ�ע��
public:
	virtual void BeginContact(b2Contact* contact);  // �ĸ������ȿ�ʱ���ã��ҴӸտ�ʼ�Ӵ�ʱ���ã�ֱ����νӴ��������ʱ��ֻ������һ�Σ���������ײ����
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);  // �ڶ������ã��ڽӴ����ڼ䣬ÿ֡������һ�Σ�This allows you to inspect a contact before it goes to the solver
	virtual void EndContact(b2Contact* contact);    // ���������ã��ڽӴ��ڼ�ÿ֡����һ�Σ�This lets you inspect a contact after the solver is finished
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse); // �����ã��ӽӴ���ʼ��ֱ������Ӵ�ʱ���ã�֮����һ�Σ���������ײЧ������ǰ
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

#ifndef NOTOUCHONEBYONEEVENT
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

