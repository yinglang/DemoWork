#include "BaseScene.h"
#include "Box2DDesignerScene.h"
#include "SpriteBinder.h"
#include "GameBegin.h"
#include "GameChoose.h"


GameBegin::GameBegin()
{
}


GameBegin::~GameBegin()
{
}

bool GameBegin::init(){
	if (!Layer::init()) return false;
	visibleSize = Director::getInstance()->getVisibleSize();

	/* add a start label */
	LabelTTF* start = LabelTTF::create("click anywhere to start","",30);
	start->setOpacity(100);
	start->setPosition(visibleSize / 2);
	addChild(start);

	FadeOut* ft = FadeOut::create(1);
	FadeIn* fi = FadeIn::create(1);
	Sequence* sq = Sequence::create(ft, fi, nullptr);
	RepeatForever* ac = RepeatForever::create(sq);
	start->runAction(ac);

	return true;
}

void GameBegin::onEnter(){
	Layer::onEnter();
	auto listenerTouchOneByOne = EventListenerTouchOneByOne::create();
	listenerTouchOneByOne->onTouchBegan = CC_CALLBACK_2(GameBegin::toChooseScene, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerTouchOneByOne, this);

	auto listenerKeyBoard = EventListenerKeyboard::create();
	listenerKeyBoard->onKeyReleased = CC_CALLBACK_2(GameBegin::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerKeyBoard, this);

	setKeypadEnabled(true);
}

Scene* GameBegin::createScene(){
	Scene* scene = Scene::create();
	Layer* layer = GameBegin::create();
	scene->addChild(layer);
	return scene;
}

void GameBegin::update(float dt){

}

bool GameBegin::toChooseScene(Touch* t, Event* e){
	Sprite* ball1 = Sprite::create("ball/ironball_mid.png");
	Sprite* ball2 = Sprite::create("ball/glassball_mid.png");
	float radius = (ball1->getContentSize().height + ball1->getContentSize().width) / 4;
	ball1->setPosition(0, visibleSize.height /2);
	ball2->setPosition(visibleSize.width, visibleSize.height / 2);
	addChild(ball1);
	addChild(ball2);

	DelayTime* dl = DelayTime::create(0.5);

	MoveTo* mt1 = MoveTo::create(1, Vec2(visibleSize.width / 2 - radius, visibleSize.height/2 ));
	Sequence* sq1 = Sequence::create(mt1, dl,nullptr);
	ball1->runAction(sq1);

	MoveTo* mt2 = MoveTo::create(1, Vec2(visibleSize.width / 2 + radius, visibleSize.height / 2));
	Sequence* sq2 = Sequence::create(mt2, dl->clone(),
		CallFunc::create([](){
		Scene* scene = GameChoose::createScene();
		auto tasi = TransitionFlipX::create(0.5, scene);
		Director::getInstance()->replaceScene(tasi);
	}), nullptr);
	ball2->runAction(sq2);
	
	return false;
}

// 手机回退键，只有released有效，pressed无效
void GameBegin::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE || // backspace键
		keyCode == EventKeyboard::KeyCode::KEY_ESCAPE){		// 手机回退键{
		Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
#endif
	}
}