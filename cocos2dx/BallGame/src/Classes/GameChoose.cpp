#include "GameChoose.h"
#include "MyTool.h"
#include "BaseScene.h"
#include "GameBegin.h"

/****************************************************************************************************/

bool GameChoose::init(){
	if (!Layer::init()) return false; 
	macroDefineInInit();
	visibleSize = Director::getInstance()->getVisibleSize();

	/*you should edit here*/
	cables = new std::vector<b2Sprite*>();

	auto bc = Sprite::create("choosebackground.png");
	bc->setPosition(visibleSize / 2);
	Size size = bc->getContentSize();
	bc->setScaleX(visibleSize.width / size.width);
	bc->setScaleY(visibleSize.height / size.height);
	addChild(bc);

	cablesprites = new std::vector<Sprite*>();

	for (int i = 0; i < 5; i++){
		Sprite* sprite = Sprite::create("cable.png");
		size = sprite->getContentSize();
		sprite->setPosition(Vec2(visibleSize.width / 5 * i + size.width / 2,
			visibleSize.height / 5 * 4 - size.height / 2));
		addChild(sprite);
		LabelTTF* label = LabelTTF::create(MyTool::num2str(i + 1), "", 80);
		label->setColor(Color3B(255, 0, 0));
		label->setPosition(sprite->getContentSize() / 2);
		sprite->addChild(label);
		cablesprites->push_back(sprite);
	}
	for (int i = 0; i < 5; i++){
		Sprite* sprite = Sprite::create("cable.png");
		size = sprite->getContentSize();
		sprite->setPosition(Vec2(visibleSize.width / 5 * i + size.width / 2,
			visibleSize.height / 5 * 2 - size.height / 2));
		addChild(sprite);
		LabelTTF* label = LabelTTF::create(MyTool::num2str(i + 6), "", 80);
		label->setColor(Color3B(255, 0, 0));
		label->setPosition(sprite->getContentSize() / 2);
		sprite->addChild(label);
		cablesprites->push_back(sprite);
	}
	//initRoadLine();

	/*auto cablebody = addCable(Vec2(100, visibleSize.height - 164), 1);
	auto linkline = addLine(Vec2(100, visibleSize.height - 75), 80, 90);
	b2RevoluteJointDef jointdef;
	jointdef.Initialize(baseline, linkline, b2Vec2(100 / RTM_RATIO, 50 / RTM_RATIO));
	world->CreateJoint(&jointdef);
	jointdef.Initialize(linkline, cablebody, b2Vec2(100 / RTM_RATIO, 100 / RTM_RATIO));
	world->CreateJoint(&jointdef);*/

	this->scheduleUpdate();
	return true;
}

void GameChoose::update(float dt){
	if (dt <= 0) dt = Director::getInstance()->getAnimationInterval();

#ifndef NOBOX2D
	world->Step(dt, 8, 3);
#endif
	for (auto body = world->GetBodyList(); body; body = body->GetNext()){
		Sprite* sprite = (Sprite*)body->GetUserData();
		if (sprite){
			sprite->setPosition(body->GetPosition().x * RTM_RATIO, body->GetPosition().y * RTM_RATIO);
			sprite->setRotation(-CC_RADIANS_TO_DEGREES(body->GetAngle()));
		}
	}
}

Scene* GameChoose::createScene(){
	Scene* scene = Scene::create();
	GameChoose* layer = GameChoose::create();
	scene->addChild(layer);
	return scene;
}

int GameChoose::getSelectedScene(Vec2 pos){
	for (int i = 0; i < int(cablesprites->size()); i++){
		auto sprite = cablesprites->at(i);
		Vec2 spos = sprite->getPosition();
		Size size = sprite->getContentSize();
		cocos2d::log("%f, %f", size.width, size.height);
		if (pos.x < spos.x + size.width/2 && pos.x > spos.x - size.width/2 &&
			pos.y < spos.y + size.height/2 && pos.y > spos.y - size.height/2){
			auto children = sprite->getChildren();
			if (children.size()){
				auto label = (LabelTTF*)children.at(0);
				if (label){
					std::string s = label->getString();
					return MyTool::str2num<int>(s);
				}
			}
		}
	}
	return -1;
}

/*
void GameChoose::initRoadLine(){
	b2BodyDef bodydef1;
	Sprite* sprite = Sprite::create("line.png");
	sprite->setScaleX(3);
	sprite->setPosition(visibleSize.width/2, visibleSize.height - 50);
	addChild(sprite);
	Vec2 pos = sprite->getPosition();
	Size size = Size(sprite->getContentSize().width * sprite->getScaleX(),
					sprite->getContentSize().height * sprite->getScaleY());
	bodydef1.position.Set(pos.x/ RTM_RATIO, pos.y / RTM_RATIO);
	bodydef1.type = b2_dynamicBody;
	bodydef1.linearDamping = 0.8;
	baseline = world->CreateBody(&bodydef1);
	b2PolygonShape polygonshape;
	polygonshape.SetAsBox(size.width /2/ RTM_RATIO, size.height /4/ RTM_RATIO);
	baseline->CreateFixture(&polygonshape, 0.5);
	baseline->SetUserData(sprite);
	
	b2BodyDef bodydef;
	b2EdgeShape edge;
	edge.Set(b2Vec2(-50 / RTM_RATIO, 0), b2Vec2(50 / RTM_RATIO, 0));
	
	bodydef.position.Set(0 / RTM_RATIO, (visibleSize.height - 58) / RTM_RATIO);
	auto leftline = world->CreateBody(&bodydef);
	leftline->CreateFixture(&edge, 0);
	leftline->SetUserData(nullptr);
	
	bodydef.position.Set(0 / RTM_RATIO, (visibleSize.height - 42) / RTM_RATIO);
	leftline = world->CreateBody(&bodydef);
	leftline->CreateFixture(&edge, 0);
	leftline->SetUserData(nullptr);

	bodydef.position.Set(visibleSize.width / RTM_RATIO, (visibleSize.height - 58) / RTM_RATIO);
	auto rightline = world->CreateBody(&bodydef);
	rightline->CreateFixture(&edge, 0);
	rightline->SetUserData(nullptr);

	bodydef.position.Set(visibleSize.width / RTM_RATIO, (visibleSize.height - 42) / RTM_RATIO);
	rightline = world->CreateBody(&bodydef);
	rightline->CreateFixture(&edge, 0);
	rightline->SetUserData(nullptr);
}

b2Body* GameChoose::addLine(Vec2 pos, float length, float angle){
	Sprite* sprite = nullptr;
	if (length < 1024)
		sprite = Sprite::create("line.png",Rect(0,0,length, 18));
	else{
		sprite = Sprite::create("line.png");
		float width = sprite->getContentSize().width;
		sprite->setScaleX(length / width);
	}
	addChild(sprite);
	sprite->setPosition(pos);
	
	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.position.Set(pos.x / RTM_RATIO, pos.y / RTM_RATIO);
	b2Body* body = world->CreateBody(&bodydef);
	b2EdgeShape edgeshape;
	edgeshape.Set(b2Vec2(-length/RTM_RATIO / 2, 0), b2Vec2(length/RTM_RATIO/2, 0));
	body->CreateFixture(&edgeshape, 0.5);
	body->SetUserData(sprite);

	if (angle) {
		sprite->setRotation(angle);
		body->SetTransform(body->GetPosition(), -CC_DEGREES_TO_RADIANS(angle));
	}

	return body;
}

b2Body* GameChoose::addCable(Vec2 pos, int num){
	auto sprite = Sprite::create("cable.png");
	addChild(sprite);
	sprite->setPosition(pos);

	auto label = LabelTTF::create(MyTool::num2str(num), "", 80);
	label->setPosition(sprite->getContentSize()/ 2);
	label->setColor(Color3B(255,0,0));
	sprite->addChild(label);

	auto cable = new b2Sprite(world, sprite);
	cables->push_back(cable);
	cable->getBody()->SetUserData(sprite);

	return cable->getBody();
}
*/

GameChoose::GameChoose()
{
}

GameChoose::~GameChoose()
{
}


/****************************************************************************************************/
/* 以下为事件或物理引擎可能需要用到的处理函数 */

#ifndef NOBOX2D

void GameChoose::BeginContact(b2Contact* contact){
}

void GameChoose::PreSolve(b2Contact* contact, const b2Manifold* oldManifold){
}

void GameChoose::EndContact(b2Contact* contact){}

void GameChoose::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse){}

/* 这里可以修改world的初始化 */
void GameChoose::initPhysics(){
	b2Vec2 gravity;
	gravity.Set(0.0f, -9.0f);
	world = new b2World(gravity);
	world->SetAllowSleeping(true);			// 允许物体进入休眠状态
	world->SetContinuousPhysics(false);      // 使用连续物理碰撞检测
	world->SetContactListener(this);			// 设置碰撞监听器

#ifndef NOBOX2DDEBUG
	// 设置debug 线的初始化信息
	debugdraw = new GLESDebugDraw(RTM_RATIO);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit; 
	//flags += b2Draw::e_aabbBit; 
	//flags += b2Draw::e_pairBit; 
	flags += b2Draw::e_centerOfMassBit;
	debugdraw->SetFlags(flags);
	world->SetDebugDraw(debugdraw);
#endif
}

#endif

#ifndef NOKEYBOARDEVENT2
void GameChoose::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
	//cocos2d::log("pressed");
	
}
// 手机回退键，只有released有效，pressed无效
void GameChoose::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){//cocos2d::log("released");
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE || // backspace键
		keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)		// 手机回退键
		Director::getInstance()->replaceScene(GameBegin::createScene());
}
#endif

#ifndef NOMOUSEEVENT
void GameChoose::onMouseDown(Event* event){
	cocos2d::log("down");	
	EventMouse * e = (EventMouse*)event;
	if(e->getMouseButton() == 0) cocos2d::log("left mouse");
	else if(e->getMouseButton() == 1) cocos2d::log("right mouse");
	else if(e->getMouseButton() == 2) cocos2d::log("middle mouse");

	/* 都是获取当前点坐标，但e->getDelta()左上角是原点，y轴向下；
	/* e->getCursorX(), e->getCursorY()左上角是原点，y轴向上  */
}

void GameChoose::onMouseMove(Event* event){cocos2d::log("move");}
void GameChoose::onMouseScroll(Event* event){cocos2d::log("scroll");}
void GameChoose::onMouseUp(Event* event){cocos2d::log("up");}
#endif

#ifndef NOTOUCHONEBYONEEVENT
bool GameChoose::onTouchBegan(Touch* touch, Event* event){
	cocos2d::log("began , %d", BaseScene::scenenum);
	if (BaseScene::scenenum <= 0){
		BaseScene::scenenum = getSelectedScene(touch->getLocation());
		if (BaseScene::scenenum > 0){
			Scene* scene = BaseScene::createScene();
			auto tras = TransitionProgress::create(0.5, scene);
			Director::getInstance()->replaceScene(scene);
		}
	}
	//cocos2d::log("%d", num);
	return false;
}//返回false，事件不会向下传 

void GameChoose::onTouchMoved(Touch* touch, Event* event){
	cocos2d::log("moved");
	/*if (((baseline->GetPosition().x < visibleSize.width / 3 * 4 / RTM_RATIO) && (touch->getDelta().x > 0)) ||
		((baseline->GetPosition().x > -visibleSize.width / 3 / RTM_RATIO) && (touch->getDelta().x) < 0))
		baseline->SetLinearVelocity(b2Vec2(touch->getDelta().x / RTM_RATIO * 50, 0));*/

}
void GameChoose::onTouchCancelled(Touch* touch, Event* event){ cocos2d::log("cancelled"); }	// 很难触发，比如玩着游戏突然来了电话
void GameChoose::onTouchEnded(Touch* touch, Event* event){
	/*baseline->SetLinearVelocity(b2Vec2(0, baseline->GetLinearVelocity().y));
	if (baseline->GetPosition().y * RTM_RATIO < visibleSize.height - 58 ||
		baseline->GetPosition().y * RTM_RATIO > visibleSize.height - 40)
		baseline->SetTransform(b2Vec2(baseline->GetPosition().x, visibleSize.height - 50), baseline->GetAngle());*/
	cocos2d::log("ended");
}
#endif

#ifndef NOTOUCHALLATONCEEVENT
void GameChoose::onTouchesBegan(const std::vector<Touch*>& touches, Event* event){cocos2d::log("began");}	
void GameChoose::onTouchesMoved(const std::vector<Touch*>& touches, Event* event){cocos2d::log("moved");}      // 点击着移动
void GameChoose::onTouchesCancelled(const std::vector<Touch*>& touches, Event* event){cocos2d::log("cancelled");}  // // 很难触发，比如玩着游戏突然来了电话
void GameChoose::onTouchesEnded(const std::vector<Touch*>& touches, Event* event){cocos2d::log("ended");}
#endif

#ifndef NOACCELERATIONEVENT
void GameChoose::onAcceleration(Acceleration* acc, Event* event){
	cocos2d::log("acceleration");
}
#endif

#ifndef NOCUSTOMEVENT
void GameChoose::onEventCustom(EventCustom*event){
	cocos2d::log("custom");
}
#endif

#ifndef NODRAW
void GameChoose::myDraw(const Mat4 &transform, uint32_t flags){
	kmGLPushMatrix();
	kmGLLoadMatrix(&transform);


#ifndef NOBOX2D
#ifndef NOBOX2DDEBUG
	world->DrawDebugData();
	/* 如果要设置绘制图的Zorder，请放入myDraw中，并设置下面_customCmd.init(_globalZOrder, transform, flags);
	的第一个参数，默认为0 设的越大越靠上*/
#endif
#endif

	//end draw
	kmGLPopMatrix();
}
#endif



/****************************************************************************************************/
/* 以下四个函数一般无需改动 */

void GameChoose::draw(Renderer *renderer, const Mat4& transform, uint32_t flags){
	Layer::draw(renderer, transform, flags);

#ifndef NODRAW
	_customCmd.init(1, transform, flags); /* 第一个参数可以设置绘图的Zorder，默认为0 设的越大越靠上 */
	_customCmd.func = CC_CALLBACK_0(GameChoose::myDraw, this, transform, flags);
	renderer->addCommand(&_customCmd);
	// follow code can instand of this
	// myDraw(transform, flags);
#endif
}

void GameChoose::onEnter(){
	Layer::onEnter();

#ifndef NOKEYBOARDEVENT2
	auto listenerKeyboard = EventListenerKeyboard::create();
	listenerKeyboard->onKeyPressed = CC_CALLBACK_2(GameChoose::onKeyPressed, this);
	listenerKeyboard->onKeyReleased = CC_CALLBACK_2(GameChoose::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerKeyboard, this);
#endif

#ifndef NOMOUSEEVENT
	auto listenerMouse = EventListenerMouse::create();
	listenerMouse->onMouseDown = CC_CALLBACK_1(GameChoose::onMouseDown, this);
	listenerMouse->onMouseMove = CC_CALLBACK_1(GameChoose::onMouseMove, this);
	listenerMouse->onMouseScroll = CC_CALLBACK_1(GameChoose::onMouseScroll, this);
	listenerMouse->onMouseUp = CC_CALLBACK_1(GameChoose::onMouseUp, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerMouse, this);
#endif

#ifndef NOTOUCHONEBYONEEVENT
	auto listenerTouchOneByOne = EventListenerTouchOneByOne::create();
	listenerTouchOneByOne->onTouchBegan = CC_CALLBACK_2(GameChoose::onTouchBegan, this);
	listenerTouchOneByOne->onTouchMoved = CC_CALLBACK_2(GameChoose::onTouchMoved, this);
	listenerTouchOneByOne->onTouchCancelled = CC_CALLBACK_2(GameChoose::onTouchCancelled, this);
	listenerTouchOneByOne->onTouchEnded = CC_CALLBACK_2(GameChoose::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerTouchOneByOne, this);
#endif

#ifndef NOTOUCHALLATONCEEVENT
	auto listenerTouchAllAtOnce = EventListenerTouchAllAtOnce::create();
	listenerTouchAllAtOnce->onTouchesBegan = CC_CALLBACK_2(GameChoose::onTouchesBegan, this);
	listenerTouchAllAtOnce->onTouchesMoved = CC_CALLBACK_2(GameChoose::onTouchesMoved, this);
	listenerTouchAllAtOnce->onTouchesCancelled = CC_CALLBACK_2(GameChoose::onTouchesCancelled, this);
	listenerTouchAllAtOnce->onTouchesEnded = CC_CALLBACK_2(GameChoose::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerTouchAllAtOnce, this);
#endif

#ifndef NOACCELERATIONEVENT
	Device::setAccelerometerEnabled(true);
	auto listenerAcceleration = EventListenerAcceleration::create(CC_CALLBACK_2(GameChoose::onAcceleration, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerAcceleration, this);
#endif

#ifndef NOACCELERATIONEVENT
	auto _listener = EventListenerCustom::create("custom_event", CC_CALLBACK_1(GameChoose::onEventCustom, this));
	_eventDispatcher->addEventListenerWithFixedPriority(_listener, 1);
	EventCustom event("custom_event");
	//event.setUserData(void*);  // 可以在GameChoose::onEventCustom()使用event.getUserData()再强转
	_eventDispatcher->dispatchEvent(&event);  // 这句话将会调用GameChoose::onEventCustom()
#endif
	setKeypadEnabled(true);
}

void GameChoose::onExit(){
	Layer::onExit();
	cocos2d::log("exit");
	_eventDispatcher->removeAllEventListeners();
}

void GameChoose::macroDefineInInit(){
#ifndef NOBOX2D
	initPhysics();

	// create a polygon body
	/*b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.position = b2Vec2(200 / RTM_RATIO, 400 / RTM_RATIO);
	b2Body* b2 = world->CreateBody(&bodydef);
	b2PolygonShape polygon;
	b2Vec2 v[4];
	v[0].Set(-1.5, 1.5);
	v[1].Set(1.5, 1.5);
	v[2].Set(1.5, -1.5);
	v[3].Set(-1.5, -1.5);
	polygon.Set(v, 4);
	b2->CreateFixture(&polygon, 0.5);*/
#endif

#ifndef NOTOUCHONEBYONEEVENT
	// 这段代码好像不写也可以
	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	
#endif

#ifndef NOTOUCHALLATONCEEVENT
	// 这段代码好像不写也可以
	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ALL_AT_ONCE);
#endif

}