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
	static std::vector<BaseBall*>* removeBalls;  // 在上个时间步中要删除的物体
	static std::vector<BaseBall*>* tobeVirusBalls;// 在上个时间步中要创的物体
	static std::vector<Fence*>* tobebrokenFences;			//// 在上个时间步中要删的栅栏

	static void createScene(int scenenum, GameScene* layer, bool isreplace=false);
	static void removeBallAndHole(std::vector<int> removeballindex, std::vector<int> removeholeindex);
	static bool removeBall(BaseBall* ball);
	static bool removeFence(Fence* fence, bool isbrokenByIronBall);

	static int sceneCount;

	static bool otherOverCondition();

private:
	static void createb2Sprite(std::map<std::string, Sprite*>::iterator iterator, b2World*);

	static void ballInHole(BaseBall* ball, Hole* hole);

	static void clearScene(GameScene* layer, bool isreplace);		// 本关清空数据,如果是替换的layer,不需removeChilren
	static void initSceneData(GameScene* layer);	// 初始化本关数据
	

	static void createFirstScene(GameScene* layer);
	
};

#endif