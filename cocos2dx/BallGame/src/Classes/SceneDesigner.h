#ifndef __SCENEDESIGNER_H__
#define __SCENEDESIGNER_H__

#include "Box2DDesignerScene.h"
#include "BallFactory.h"
#include "Hole.h"
#include "BaseScene.h"
#include "Fence.h"

class SceneDesigner
{
public:

	static std::vector<BaseBall*>* balls;
	static std::vector<Hole*>* holes;
	static std::vector<Fence*>* fences;
	static std::vector<BaseBall*>* removeBalls;  // ���ϸ�ʱ�䲽��Ҫɾ��������
	static std::vector<BaseBall*>* tobeVirusBalls;// ���ϸ�ʱ�䲽��Ҫ��������
	static std::vector<Fence*>* tobebrokenFences;			//// ���ϸ�ʱ�䲽��Ҫɾ��դ��

	static void createScene(int scenenum, GameScene* layer, bool isreplace=false);
	static void removeBallAndHole(std::vector<int> removeballindex, std::vector<int> removeholeindex);
	static bool removeBall(BaseBall* ball);
	static bool removeFence(Fence* fence, bool isbrokenByIronBall);

	static int sceneCount;

	static bool otherOverCondition();

private:
	static void createb2Sprite(std::map<std::string, Sprite*>::iterator iterator, b2World*);

	static void ballInHole(BaseBall* ball, Hole* hole);

	static void clearScene(GameScene* layer, bool isreplace);		// �����������,������滻��layer,����removeChilren
	static void initSceneData(GameScene* layer);	// ��ʼ����������
	

	static void createFirstScene(GameScene* layer);
	
};

#endif