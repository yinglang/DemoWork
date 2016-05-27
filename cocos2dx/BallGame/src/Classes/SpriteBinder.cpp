#include "SpriteBinder.h"
#include "Box2DDesignerScene.h"

/******** warnning : 添加精灵路径时，不要忘了修改string数组的大小 ***********/

// 以下是能被移动，旋转，删除，拷贝的精灵
#define SpriteImageCount 19
std::string* SpriteBinder::spriteImagepath = new std::string[SpriteImageCount]{
		"fence.png",
		"ball/baseball_small.png",
		"ball/doubleball_small.png",
		"ball/eggball_small.png",
		"ball/electricballball_small.png",
		"ball/ghostball_small.png",
		"ball/glassball_small.png",
		"ball/ironball_small.png",
		"ball/ungravityball_small.png",
		"ball/virusball_small.png",

		"hole/basehole_small.png",
		"hole/egghole_small.png",
		"hole/electrichole_small.png",
		"hole/ghosthole_small.png",
		"hole/glasshole_small.png",
		"hole/ironhole_small.png",
		"hole/ungravityhole_small.png",
		"hole/virushole_small.png",
		"hole/blackhole_small.png"
};

// 不被移动，旋转，删除，拷贝的精灵，请添加到此
void SpriteBinder::loadBackground(){
	/*Sprite* bc = Sprite::create("ball/ball3.png");
	bc->setPosition(Vec2::ZERO);
	addChild(bc);*/
}

void SpriteBinder::setting(){
	uiLayerHeight = 50;
	uiRightWidth = 200;
	angleDelta = 20;

	minScale = 0.1;
	maxScale = 10;
	scaleDelta = 0.9;
}

/*********************************************************************************************/

Scene* SpriteBinder::createScene(){
	Scene* scene = Scene::create();
	auto layer = SpriteBinder::create();
	scene->addChild(layer->topListView);
	scene->addChild(layer->rightListView);
	scene->addChild(layer);
	return scene;
}

bool SpriteBinder::init(){
	if (!Layer::init()) return false;

	initPhysics();
	//Box2DDesignerScene::readDataFromXML(this,world);
	//Box2DDesignerScene::readDataToXMLInAnyModel(this, world);
	//auto body =Box2DDesignerScene::getBodyByName("body3",world);

	visibleSize = Director::getInstance()->getVisibleSize();
	currentSelectedSprite = nullptr;
	spriteCount = 0;
	nameCount = 0;
	hadOutightListView = false;
	hadmoved = false;
	setting();

	initTopUI();
	initRightUI();

	loadBackground();

	loadSprite();

	this->scheduleUpdate();

	return true;
}

void SpriteBinder::initTopUI(){
	topListView = ui::ListView::create();
	topListView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
	topListView->setBounceEnabled(true);
	topListView->setBackGroundImage("Box2DDesigner/board.jpg");
	topListView->setBackGroundImageScale9Enabled(true);
	topListView->setBackGroundImageOpacity(180);
	topListView->setContentSize(Size(visibleSize.width, uiLayerHeight));
	topListView->setPosition(Vec2(0, visibleSize.height- uiLayerHeight));
	topListView->setItemsMargin(10);

	ui::Button* saveButton = ui::Button::create("Box2DDesigner/button.png");
	saveButton->setTag(1);
	saveButton->setTitleText("save");
	saveButton->setTitleFontSize(25);
	saveButton->addTouchEventListener(CC_CALLBACK_2(SpriteBinder::saveEvent, this));
	topListView->addChild(saveButton);

	auto gobackButton = ui::Button::create("Box2DDesigner/button.png");
	gobackButton->setTag(2);
	gobackButton->setTitleText("go back");
	gobackButton->setTitleFontSize(25);
	gobackButton->addTouchEventListener(CC_CALLBACK_2(SpriteBinder::gobackEvent, this));
	//gobackButton->setEnabled(false);
	topListView->addChild(gobackButton);

	int i = -1;
	ui::Button* copyButton = ui::Button::create("Box2DDesigner/button.png");
	copyButton->setTag(i--);
	copyButton->setTitleText("copy");
	copyButton->setTitleFontSize(25);
	copyButton->addTouchEventListener(CC_CALLBACK_2(SpriteBinder::copyEvent, this));
	copyButton->setEnabled(false);
	topListView->addChild(copyButton);

	ui::Button* deleteButton = ui::Button::create("Box2DDesigner/button.png");
	deleteButton->setTag(i--);
	deleteButton->setTitleText("delete");
	deleteButton->setTitleFontSize(25);
	deleteButton->addTouchEventListener(CC_CALLBACK_2(SpriteBinder::deleteEvent, this));
	deleteButton->setEnabled(false);
	topListView->addChild(deleteButton);

	ui::Button* rotateButton = ui::Button::create("Box2DDesigner/button.png");
	rotateButton->setTag(i--);
	rotateButton->setTitleText("rotate");
	rotateButton->setTitleFontSize(25);
	rotateButton->addTouchEventListener(CC_CALLBACK_2(SpriteBinder::rotateEvent, this));
	rotateButton->setEnabled(false);
	topListView->addChild(rotateButton);

	ui::Button* scaleButton = ui::Button::create("Box2DDesigner/button.png");
	scaleButton->setTag(i--);
	scaleButton->setTitleText("bigger");
	scaleButton->setTitleFontSize(25);
	scaleButton->addTouchEventListener(CC_CALLBACK_2(SpriteBinder::scaleEvent, this));
	scaleButton->setEnabled(false);
	topListView->addChild(scaleButton);

	scaleButton = ui::Button::create("Box2DDesigner/button.png");
	scaleButton->setTag(i--);
	scaleButton->setTitleText("smaller");
	scaleButton->setTitleFontSize(25);
	scaleButton->addTouchEventListener(CC_CALLBACK_2(SpriteBinder::scaleEvent, this));
	scaleButton->setEnabled(false);
	topListView->addChild(scaleButton);

	auto flipButton = ui::Button::create("Box2DDesigner/button.png");
	flipButton->setTag(i--);
	flipButton->setTitleText("filp");
	flipButton->setTitleFontSize(25);
	flipButton->addTouchEventListener(CC_CALLBACK_2(SpriteBinder::flipEvent, this));
	flipButton->setEnabled(false);
	topListView->addChild(flipButton);

	scaleButton = ui::Button::create("Box2DDesigner/button.png");
	scaleButton->setTag(i--);
	scaleButton->setTitleText("biggerX");
	scaleButton->setTitleFontSize(23);
	scaleButton->addTouchEventListener(CC_CALLBACK_2(SpriteBinder::scaleEvent, this));
	scaleButton->setEnabled(false);
	topListView->addChild(scaleButton);

	scaleButton = ui::Button::create("Box2DDesigner/button.png");
	scaleButton->setTag(i--);
	scaleButton->setTitleText("biggerY");
	scaleButton->setTitleFontSize(23);
	scaleButton->addTouchEventListener(CC_CALLBACK_2(SpriteBinder::scaleEvent, this));
	scaleButton->setEnabled(false);
	topListView->addChild(scaleButton);

	scaleButton = ui::Button::create("Box2DDesigner/button.png");
	scaleButton->setTag(i--);
	scaleButton->setTitleText("smallerX");
	scaleButton->setTitleFontSize(23);
	scaleButton->addTouchEventListener(CC_CALLBACK_2(SpriteBinder::scaleEvent, this));
	scaleButton->setEnabled(false);
	topListView->addChild(scaleButton);

	scaleButton = ui::Button::create("Box2DDesigner/button.png");
	scaleButton->setTag(i--);
	scaleButton->setTitleText("smallerY");
	scaleButton->setTitleFontSize(23);
	scaleButton->addTouchEventListener(CC_CALLBACK_2(SpriteBinder::scaleEvent, this));
	scaleButton->setEnabled(false);
	topListView->addChild(scaleButton);
}

void SpriteBinder::initRightUI(){
	rightListView = ui::ListView::create();
	rightListView->setDirection(ui::ScrollView::Direction::VERTICAL);
	rightListView->setBounceEnabled(true);
	rightListView->setBackGroundImage("Box2DDesigner/green_edit.png");
	rightListView->setBackGroundImageScale9Enabled(true);
	rightListView->setBackGroundImageOpacity(180);
	rightListView->setContentSize(Size(uiRightWidth, visibleSize.height - uiLayerHeight));
	rightListView->setPosition(Vec2(visibleSize.width, 0));
	rightListView->setItemsMargin(10);

	_editName = ui::EditBox::create(Size(uiRightWidth - 10, 40),
		ui::Scale9Sprite::create("Box2DDesigner/green_edit.png"));
	_editName->setFontSize(20);
	_editName->setFontColor(Color3B::RED);
	_editName->setPlaceHolder("name:");
	_editName->setPlaceholderFontColor(Color3B::WHITE);
	_editName->setMaxLength(8);
	_editName->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	_editName->setDelegate(this);
	_editName->setName("name");
	rightListView->addChild(_editName);

}

void SpriteBinder::loadSprite(){
	// 可以一个tag/name 对应多个精灵，getSpriteByTag()是返回第一个找到的,
	// tag不设置默认是-1

	for (int i = 0; i < SpriteImageCount; i++){ 
		auto sprite = Sprite::create(spriteImagepath[i]);
		if (sprite){
			sprite->setPosition(visibleSize / 2);
			sprite->setTag(i);
			sprite->setName("");
			//sprite->setName("sprite_"+num2str(nameCount));
			addChild(sprite, 1);
			//spriteCount++;
			//nameCount++;
		}
		else{
			MessageBox("sprite image may not exit","warning");
		}
	}
}

void SpriteBinder::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox){
	//cocos2d::log("hhhh");
	if (editBox->getName() == "name"){
		editBox->setText(currentSelectedSprite->getName().c_str());
	}
}

void SpriteBinder::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox){
	//cocos2d::log("dddd");
	if (editBox->getName() == "name"){
		if (editBox->getText() != ""){
			currentSelectedSprite->setName(editBox->getText());
		}
	}
}

void SpriteBinder::editBoxReturn(ui::EditBox* editBox){
	//cocos2d::log("dddd2");
}

void SpriteBinder::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text){
	//cocos2d::log("dddd1");
}

void SpriteBinder::saveEvent(Ref* psender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		saveSpriteDataToXML();
	}
}

void SpriteBinder::copyEvent(Ref* psender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		if (currentSelectedSprite){
			if (currentSelectedSprite->getTag() < SpriteImageCount &&
				currentSelectedSprite->getTag() >= 0){
				auto imagepath = spriteImagepath[currentSelectedSprite->getTag()];
				Sprite* sprite = Sprite::create(imagepath);
				if (sprite){
					sprite->setTag(currentSelectedSprite->getTag()); // 设置图片路径 的index标志
					sprite->setName("sprite_"+num2str(nameCount));
					addChild(sprite,1);

					sprite->setPosition(currentSelectedSprite->getPosition());
					sprite->setScaleX(currentSelectedSprite->getScaleX());
					sprite->setScaleY(currentSelectedSprite->getScaleY());
					sprite->setRotation(currentSelectedSprite->getRotation());
					sprite->setFlippedX(currentSelectedSprite->isFlippedX());
					sprite->setFlippedY(currentSelectedSprite->isFlippedY());

					currentSelectedSprite = sprite;
					spriteCount++;
					nameCount++;
				}
				else{
					MessageBox("sprite image may not exit", "warning");
				}
			}
		}
	}
}

void SpriteBinder::deleteEvent(Ref* psender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		if (currentSelectedSprite){
			removeChild(currentSelectedSprite);
			currentSelectedSprite = nullptr;
			spriteCount--;
		}
	}
}

void SpriteBinder::rotateEvent(Ref* psender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		if (currentSelectedSprite){
			currentSelectedSprite->setRotation(currentSelectedSprite->getRotation() + angleDelta);
		}
	}
}

void SpriteBinder::scaleEvent(Ref* psender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		if (currentSelectedSprite){
			auto button = (ui::Button*)psender;
			if (button){
				if (button->getTitleText() == "bigger"){
					currentSelectedSprite->setScale(currentSelectedSprite->getScale() / scaleDelta);
				}
				else if (button->getTitleText() == "smaller"){
					currentSelectedSprite->setScale(currentSelectedSprite->getScale() * scaleDelta);
				}
				else if (button->getTitleText() == "biggerX"){
					currentSelectedSprite->setScaleX(currentSelectedSprite->getScaleX() / scaleDelta);
				}
				else if (button->getTitleText() == "biggerY"){
					currentSelectedSprite->setScaleY(currentSelectedSprite->getScaleY() / scaleDelta);
				}
				else if (button->getTitleText() == "smallerX"){
					currentSelectedSprite->setScaleX(currentSelectedSprite->getScaleX() * scaleDelta);
				}
				else if (button->getTitleText() == "smallerY"){
					currentSelectedSprite->setScaleY(currentSelectedSprite->getScaleY() * scaleDelta);
				}
			}
		}
	}
}

// FlipX,null -> null,FlipY ->FlipX FlipY -> null,null
void SpriteBinder::flipEvent(Ref* psender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		if (currentSelectedSprite){
			if (!currentSelectedSprite->isFlippedX() && !currentSelectedSprite->isFlippedY())
				currentSelectedSprite->setFlippedX(true);

			else if (currentSelectedSprite->isFlippedX() && !currentSelectedSprite->isFlippedY()) {
				currentSelectedSprite->setFlippedX(false);
				currentSelectedSprite->setFlippedY(true);
			}
			else if (!currentSelectedSprite->isFlippedX() && currentSelectedSprite->isFlippedY()){
				currentSelectedSprite->setFlippedX(true);
			}
			else{
				currentSelectedSprite->setFlippedX(false);
				currentSelectedSprite->setFlippedY(false);
			}
		}
	}
}

void SpriteBinder::gobackEvent(Ref* psender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		if (currentSelectedSprite){
			currentSelectedSprite->setScale(1);
			currentSelectedSprite->setRotation(0);
			currentSelectedSprite->setFlippedX(false);
			currentSelectedSprite->setFlippedY(false);
		}
		else{
			setPosition(Vec2::ZERO);
			setScale(1);
			setRotation(0);
		}
	}
}

void SpriteBinder::onEnter(){
	Layer::onEnter();

	auto listenerTouchOneByOne = EventListenerTouchOneByOne::create();
	listenerTouchOneByOne->onTouchBegan = CC_CALLBACK_2(SpriteBinder::onTouchBegan, this);
	listenerTouchOneByOne->onTouchMoved = CC_CALLBACK_2(SpriteBinder::onTouchMoved, this);
	listenerTouchOneByOne->onTouchEnded = CC_CALLBACK_2(SpriteBinder::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerTouchOneByOne, this);
}

void SpriteBinder::update(float dt){

	world->Step(dt, 8, 3);
}

void SpriteBinder::outRightListView(){
	if (_editName && currentSelectedSprite) _editName->setText(currentSelectedSprite->getName().c_str());
	if (!hadOutightListView && !hadmoved){
		MoveBy* mb = MoveBy::create(0.3, Vec2(-uiRightWidth, 0));
		rightListView->runAction(mb);
		hadOutightListView = true;
	}
}

void SpriteBinder::backRightListView(Vec2 currentselectedpoint){
	if (hadOutightListView && !hadmoved){
		if (currentselectedpoint.x < visibleSize.width - uiRightWidth){
			MoveBy* mb = MoveBy::create(0.3, Vec2(uiRightWidth, 0));
			rightListView->runAction(mb);
			hadOutightListView = false;
		}
	}
}

bool SpriteBinder::onTouchBegan(Touch* touch, Event* event){		// 返回false事件不会向后传递
	Vec2 currenselectedpoint = touch->getLocation();
	if (currenselectedpoint.y > visibleSize.height - uiLayerHeight) return false;
	if (hadOutightListView && currenselectedpoint.x > visibleSize.width - uiRightWidth) return  false;
	
	currenselectedpoint -= getPosition();
	currentSelectedSprite = nullptr;
	hadmoved = false;
	auto sprite = getSelectedSprite(currenselectedpoint);
	if (sprite){
		currentSelectedSprite = sprite;
		setEnableAllButton(true);
	}
	else{
		setEnableAllButton(false);
	}
	return true;
}

void SpriteBinder::onTouchMoved(Touch* touch, Event* event){      // 点击着移动
	hadmoved = true;
	if (currentSelectedSprite){
		currentSelectedSprite->setPosition(currentSelectedSprite->getPosition() + touch->getDelta());
	}
	else{
		setPosition(getPosition() + touch->getDelta());
	}
}

void SpriteBinder::onTouchEnded(Touch* touch, Event* event){
	Vec2 currenselectedpoint = touch->getLocation()-getPosition();
	if (currentSelectedSprite) outRightListView();
	else backRightListView(currenselectedpoint);
}

void SpriteBinder::setEnableAllButton(bool enable){
	auto buttons = topListView->getChildren();
	for (int i = 0; i < buttons.size(); i++){
		auto button = (ui::Button*)buttons.at(i);
		if (button){
			if (button->getTag() < 0){ // 不是save， goback
				button->setEnabled(enable);
			}
		}
	}
}

Sprite* SpriteBinder::getSelectedSprite(Vec2 currentselectedpoint){
	auto children = getChildren();
	for (int i = int(children.size()) - 1; i >= 0; i--){ // 倒着检查，越在上面越优先
		auto sprite = (Sprite*)children.at(i);
		if (sprite && sprite->getTag() >= 0){
			Vec2 center =sprite->getPosition();
			Size size = sprite->getContentSize();
			size.width *= getScaleX();
			size.height *= getScaleY();
			float angle = -CC_DEGREES_TO_RADIANS(sprite->getRotation());
			Vec2 startpoint = size.width / 2 * Vec2(cos(angle), sin(angle));
			Vec2 endpoint = -startpoint;
			startpoint = startpoint + center;
			endpoint = endpoint + center;
			if (isSeletedSprite(startpoint, endpoint, currentselectedpoint, size.height/2)){
				return sprite;
			}
		}
	}
	return nullptr;
}

bool SpriteBinder::isSeletedSprite(Vec2 startpoint, Vec2 endpoint, Vec2 targetpoint, float rangelength){
	Vec2 middlepoint = (startpoint + endpoint) / 2;
	if (startpoint.x == endpoint.x){  // L : x = x0
		float height = abs(startpoint.y - endpoint.y);
		float disx = abs(targetpoint.x - middlepoint.x);
		float disy = abs(targetpoint.y - middlepoint.y);
		if (disx <= rangelength && disy < height / 2){
			return true;
		}
		else return false;
	}
	if (startpoint.y == endpoint.y){  // L : y = y0

		float width = abs(startpoint.x - endpoint.x);
		float disx = abs(targetpoint.x - middlepoint.x);
		float disy = abs(targetpoint.y - middlepoint.y);

		if (disy <= rangelength && disx < width / 2){
			return true;
		}
		else return false;
	}

	else{  // L : y = a*x + b
		float a1 = (endpoint.y - startpoint.y) / (endpoint.x - startpoint.x);
		float b1 = middlepoint.y - a1 * middlepoint.x;
		// 是距离 < pointRange 的变式
		bool in1 = (abs(targetpoint.y - a1 * targetpoint.x - b1) - rangelength  * sqrt(1 + a1 * a1)) <= 0;

		float a2 = -1 / a1;
		float b2 = middlepoint.y - a2 * middlepoint.x;
		float dis = startpoint.distance(endpoint) / 2;
		bool in2 = (abs(targetpoint.y - a2 * targetpoint.x - b2) - dis* sqrt(1 + a2 * a2)) <= 0;

		if (in1 && in2) return true;
		return false;
	}
}

/* 物理世界和绘制调试线，以及绘图的成员 */

void SpriteBinder::initPhysics()
{
	b2Vec2 gravity;
	gravity.Set(0.0f, 0.0f);
	world = new b2World(gravity);
	world->SetAllowSleeping(true);			// 允许物体进入休眠状态
	world->SetContinuousPhysics(true);      // 使用连续物理碰撞检测

	debugdraw = new GLESDebugDraw(RTM_RATIO);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	//flags += b2Draw::e_jointBit;
	//flags += b2Draw::e_aabbBit;
	//flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
	debugdraw->SetFlags(flags);
	world->SetDebugDraw(debugdraw);
}

void SpriteBinder::draw(Renderer *renderer, const Mat4& transform, uint32_t flags){
	Layer::draw(renderer, transform, flags);

	_customCmd.init(1, transform, flags);				// 这个_customeCmd和layer 中的物体不再一个Z坐标系，只能设成要么全在上面，要么全在下面
	_customCmd.func = CC_CALLBACK_0(SpriteBinder::myDraw, this, transform, flags);
	renderer->addCommand(&_customCmd);
}

void SpriteBinder::myDraw(const Mat4 &transform, uint32_t flags){
	kmGLPushMatrix();
	kmGLLoadMatrix(&transform);

	world->DrawDebugData();
	// 圈出选中物体
	if (currentSelectedSprite){
		Vec2 v[4];
		Size size = currentSelectedSprite->getContentSize();
		size.width *= currentSelectedSprite->getScaleX();
		size.height *= currentSelectedSprite->getScaleY();
		size = size + Size(5, 5);

		Vec2 center = currentSelectedSprite->getPosition();
		float angle = -CC_DEGREES_TO_RADIANS(currentSelectedSprite->getRotation());
		v[0] = center + pointRotate(Vec2(size / 2), angle);
		v[1] = center + pointRotate(Vec2(size.width / 2, -size.height / 2), angle);
		v[2] = center + pointRotate(Vec2(-1 * size / 2), angle);
		v[3] = center + pointRotate(Vec2(-size.width / 2, size.height / 2), angle);
		DrawPrimitives::setDrawColor4B(255,0,0,255);
		DrawPrimitives::drawPoly(v,4,true);
	}
	// 圈出边界
	{
		Vec2 v[4];
		Size size = visibleSize;

		Vec2 center = Vec2(visibleSize / 2);
		float angle = 0;
		v[0] = center + pointRotate(Vec2(size / 2), angle);
		v[1] = center + pointRotate(Vec2(size.width / 2, -size.height / 2), angle);
		v[2] = center + pointRotate(Vec2(-1 * size / 2), angle);
		v[3] = center + pointRotate(Vec2(-size.width / 2, size.height / 2), angle);
		DrawPrimitives::setDrawColor4B(255, 255, 255, 255);
		DrawPrimitives::drawPoly(v, 4, true);
	}
	

	//end draw
	kmGLPopMatrix();
}

// 使用极坐标思想
Vec2 SpriteBinder::pointRotate(Vec2 point, float angle){
	float costheta = cos(angle);
	float sintheta = sin(angle);
	return Vec2(point.x * costheta - point.y * sintheta,
		point.y * costheta + point.x * sintheta);
}

/* sprite数据存储 */
std::vector<SpriteData>* SpriteBinder::readSpriteDataFromXML(std::string filePath){
	std::vector<SpriteData>* spritedatas = nullptr;
	try{
		if (filePath == "")
			filePath = FileUtils::getInstance()->getWritablePath() + "SpriteData.xml";

		tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();

		tinyxml2::XMLError errorId;
		filePath = FileUtils::getInstance()->fullPathForFilename(filePath).c_str();
		if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
			ssize_t bufferSize = 0;
			auto pBuffer = FileUtils::getInstance()->getFileData(filePath.c_str(), "rb", &bufferSize);
			if (pBuffer == nullptr) { MessageBox("pBuffefer is nullptr in SpriteBinder::readSpriteFromXML", "error"); return nullptr; }
			pBuffer[bufferSize - 1] = '\0';
			errorId = pDoc->Parse((const char*)pBuffer, bufferSize);
			if (errorId != 0){
				MessageBox(("xml read errorId is " + num2str(errorId) + " in SpriteBinder::readSpriteFromXML").c_str(), "error");
				return nullptr;
			}
		}
		else
			errorId = pDoc->LoadFile(filePath.c_str());
		if (errorId != 0) {
			//xml格式错误
			cocos2d::log("xml read error");
			MessageBox((filePath + "xml read error in SpriteBinder::readSpriteDataFromXML").c_str(), "error");
			return nullptr;
		}

		tinyxml2::XMLElement *spritedatalistE = pDoc->RootElement();
		spritedatas = new std::vector<SpriteData>();
		const int count = spritedatalistE->FirstAttribute()->IntValue();

		for (auto spritedataE = spritedatalistE->FirstChildElement(); spritedataE;
			spritedataE = spritedataE->NextSiblingElement()){
			SpriteData spritedata;
			auto spritedataA = spritedataE->FirstAttribute();
			spritedata.name = spritedataA->Value();
			spritedataA = spritedataA->Next();
			spritedata.imagepath = spritedataA->Value();
			spritedataA = spritedataA->Next();
			spritedata.scaleX = spritedataA->FloatValue();
			spritedataA = spritedataA->Next();
			spritedata.scaleY = spritedataA->FloatValue();
			spritedataA = spritedataA->Next();
			spritedata.position.x = spritedataA->FloatValue();
			spritedataA = spritedataA->Next();
			spritedata.position.y = spritedataA->FloatValue();
			spritedataA = spritedataA->Next();
			spritedata.rotation = spritedataA->FloatValue();
			spritedataA = spritedataA->Next();
			spritedata.isFlipX = spritedataA->BoolValue();
			spritedataA = spritedataA->Next();
			spritedata.isFlipY = spritedataA->BoolValue();
			spritedatas->push_back(spritedata);
		}

		delete pDoc;
	}
	catch (std::exception e){
		MessageBox((filePath + "xml read error in SpriteBinder::readSpriteDataFromXML").c_str(), "error");
		return nullptr;
	}

	return spritedatas;
}

std::map<std::string, Sprite*>* SpriteBinder::readSpriteFromXML(std::string filePath, Layer* layer, int zOrder){
	if(filePath == "") filePath = FileUtils::getInstance()->getWritablePath() + "SpriteData.xml";

	std::map<std::string, Sprite*>* sprites = nullptr;
	tinyxml2::XMLError errorId;
	try{
		tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
		filePath = FileUtils::getInstance()->fullPathForFilename(filePath).c_str();
		if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS){// android 是压缩包不能直接存储
			ssize_t bufferSize = 0;
			auto pBuffer = FileUtils::getInstance()->getFileData(filePath.c_str(), "rb", &bufferSize);
			if (pBuffer == nullptr) { MessageBox("pBuffefer is nullptr in SpriteBinder::readSpriteFromXML", "error"); return nullptr; }
			pBuffer[bufferSize - 1] = '\0';
			errorId = pDoc->Parse((const char*)pBuffer, bufferSize);
			if (errorId != 0){
				MessageBox(("xml read errorId is "+ num2str(errorId)+" in SpriteBinder::readSpriteFromXML").c_str(), "error"); 
				return nullptr;
			}
		}
		else
			errorId = pDoc->LoadFile(filePath.c_str());

		if (errorId != 0) {
			//xml格式错误
			cocos2d::log("xml read error");
			MessageBox((filePath + "xml read error in SpriteBinder::readSpriteFromXML").c_str(), "error");
			return nullptr;
		}
		tinyxml2::XMLElement *spritedatalistE = pDoc->RootElement();
		sprites = new std::map<std::string, Sprite*>();
		const int count = spritedatalistE->FirstAttribute()->IntValue();

		for (auto spritedataE = spritedatalistE->FirstChildElement(); spritedataE;
			spritedataE = spritedataE->NextSiblingElement()){
			auto spritedataA = spritedataE->FirstAttribute();
			std::string name = spritedataA->Value();
			spritedataA = spritedataA->Next();
			Sprite* sprite = Sprite::create(spritedataA->Value());
			if (sprite){
				spritedataA = spritedataA->Next();
				sprite->setScaleX(spritedataA->FloatValue());
				spritedataA = spritedataA->Next();
				sprite->setScaleY(spritedataA->FloatValue());
				spritedataA = spritedataA->Next();
				Vec2 position;
				position.x = spritedataA->FloatValue();
				spritedataA = spritedataA->Next();
				position.y = spritedataA->FloatValue();
				sprite->setPosition(position);
				spritedataA = spritedataA->Next();
				sprite->setRotation(spritedataA->FloatValue());
				spritedataA = spritedataA->Next();
				sprite->setFlippedX(spritedataA->BoolValue());
				spritedataA = spritedataA->Next();
				sprite->setFlipY(spritedataA->BoolValue());
				if (layer) layer->addChild(sprite, zOrder);
				sprites->insert(std::pair<std::string, Sprite*>(name, sprite));
			}
		}

		delete pDoc;
	}
	catch (std::exception e){
		MessageBox((filePath + "xml read error in SpriteBinder::readSpriteFromXML").c_str(), "error");
		return nullptr;
	}
	return sprites;
}

void SpriteBinder::saveSpriteDataToXML(){
	std::string filePath = FileUtils::getInstance()->getWritablePath() + "SpriteData.xml";
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	
	//xml 声明（参数可选）
	tinyxml2::XMLDeclaration *pDel = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
	pDoc->LinkEndChild(pDel);

	//添加plist节点
	auto sprites = getChildren();
	tinyxml2::XMLElement *plistElement = pDoc->NewElement("spritedatalist");
	plistElement->SetAttribute("count", spriteCount);
	pDoc->LinkEndChild(plistElement);

	for (int i = 0; i < sprites.size(); i++){
		auto sprite = (Sprite*)sprites.at(i);
		if (sprite && sprite->getTag() >= 0 && sprite->getTag() < SpriteImageCount && sprite->getName() != ""){
			auto spritedataElement = pDoc->NewElement("spritedata");
			plistElement->LinkEndChild(spritedataElement);
			spritedataElement->SetAttribute("name", sprite->getName().c_str());
			spritedataElement->SetAttribute("imagepath", spriteImagepath[sprite->getTag()].c_str());
			spritedataElement->SetAttribute("scalex", sprite->getScaleX());
			spritedataElement->SetAttribute("scaley", sprite->getScaleY());
			spritedataElement->SetAttribute("postionx", sprite->getPosition().x);
			spritedataElement->SetAttribute("postiony", sprite->getPosition().y);
			spritedataElement->SetAttribute("rotation", sprite->getRotation());
			spritedataElement->SetAttribute("isflipx", sprite->isFlippedX());
			spritedataElement->SetAttribute("isflipy", sprite->isFlippedY());
		}
	}
	pDoc->SaveFile(filePath.c_str());
	cocos2d::log("%s",filePath.c_str());
	delete pDoc;
}
