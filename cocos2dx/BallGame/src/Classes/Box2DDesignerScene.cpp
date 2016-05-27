#include "Box2DDesignerScene.h"

/************************************************************************************************************************/
/* 如果你并不想更改源码，但想给designer加载背景和设置一些基本属性，你应该只修改以下两个函数 */
/* void Box2DDesignerScene::loadBackGroundIamge() */

void Box2DDesignerScene::loadBackGroundIamge(){
	//Sprite* bc1 = Sprite::create("");
	//addChild(bc1);
}

std::string Box2DDesignerScene::defaultReadFilePath = "Box2DDesigner/bos2ddesigner.xml";

void Box2DDesignerScene::setting(){
	saveOfFrames = 3600;			// 每隔saveOfFrames保存一次
	usingSimpleSavingModel = false;
	defaultSaveFilepath = "Box2DDesigner/bos2ddesigner.xml";
}

/************************************************************************************************************************/

/* UILayer 类 designer 的 UI界面设计 */

bool UILayer::init(){// 此时成员变量designer还是nullptr,初始化在Box2DDesignerScene::init
	if (!Layer::init()) return false;

	visiableSize = Director::getInstance()->getVisibleSize();

	uiLayerHeight = 50;
	designer = nullptr;
	uiRightWidth = 220;
	outrange = 30;
	currentRightShowBody = nullptr;
	currentSelectedTextField = nullptr;
	savefilename = "";

	topListViewOut = false;
	rightListViewOut = false;

	inputs = new ui::Widget*[a_count];
	for (int i = 0; i < a_count; i++) inputs[i] = nullptr;

	shapetype = EDGE;

	/* 初始化上方的ListView */
	initTopListView();

	/* 初始化右侧的ListView */
	initRightListView();

	return true;
}

void UILayer::initTopListView(){
	topListView = ui::ListView::create();
	topListView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
	topListView->setBounceEnabled(true);
	topListView->setBackGroundImage("Box2DDesigner/board.jpg");
	topListView->setBackGroundImageScale9Enabled(true);
	topListView->setBackGroundImageOpacity(180);
	topListView->setContentSize(Size(visiableSize.width, uiLayerHeight));
	topListView->setPosition(Vec2(0, visiableSize.height));
	topListView->setItemsMargin(10);
	addChild(topListView);

	ui::Button* shapeButton = ui::Button::create("Box2DDesigner/button.png");
	shapeButton->setTitleText("edge");
	shapeButton->setTitleFontSize(25);
	//shapeButton->setPosition(Vec2(shapeButton->getContentSize().width/2, uiLayerHeight / 2));
	shapeButton->addTouchEventListener(CC_CALLBACK_2(UILayer::shapeEvent, this));
	topListView->addChild(shapeButton);

	ui::Button* savemodelButton = ui::Button::create("Box2DDesigner/button.png");
	savemodelButton->setTitleText("simple");
	savemodelButton->setTitleFontSize(23);
	savemodelButton->addTouchEventListener(CC_CALLBACK_2(UILayer::saveEvent, this));
	topListView->addChild(savemodelButton);

	ui::Button* gravityButton = ui::Button::create("Box2DDesigner/button.png");
	gravityButton->setTitleText("gravity");
	gravityButton->setTitleFontSize(23);
	gravityButton->addTouchEventListener(CC_CALLBACK_2(UILayer::gravityEvent, this));
	topListView->addChild(gravityButton);

	ui::Button* saveButton = ui::Button::create("Box2DDesigner/button.png");
	saveButton->setTitleText("save");
	saveButton->setTitleFontSize(25);
	saveButton->addTouchEventListener(CC_CALLBACK_2(UILayer::saveEvent, this));
	topListView->addChild(saveButton);

	/* 添加filepath编辑器 */
	auto _editName = ui::EditBox::create(Size(uiRightWidth - 10, 40),
		ui::Scale9Sprite::create("Box2DDesigner/green_edit.png"));
	_editName->setFontSize(8);
	_editName->setFontColor(Color3B::RED);
	_editName->setPlaceHolder("save file name");
	_editName->setPlaceholderFontColor(Color3B::WHITE);
	_editName->setPlaceholderFontSize(8);
	_editName->setMaxLength(28);
	_editName->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	_editName->setDelegate(this);
	_editName->setName("savefilename");
	topListView->addChild(_editName);
}

void UILayer::initRightListView(){
	/* 右下角listView */
	rightListView = ui::ListView::create();
	rightListView->setDirection(ui::ScrollView::Direction::VERTICAL);
	rightListView->setBounceEnabled(true);
	rightListView->setBackGroundImage("Box2DDesigner/green_edit.png");
	rightListView->setBackGroundImageScale9Enabled(true);
	rightListView->setBackGroundImageOpacity(180);
	rightListView->setContentSize(Size(uiRightWidth, visiableSize.height - uiLayerHeight));
	rightListView->setPosition(Vec2(visiableSize.width, 0));
	rightListView->setItemsMargin(10);
	addChild(rightListView);

	addLayoutToRightListView("name", a_name);
	// bodyDef
	addLayoutToRightListView("positionX", a_positionX);
	addLayoutToRightListView("positionY", a_positionY);
	addLayoutToRightListView("type", a_type, 2);
	addLayoutToRightListView("angle", a_angle);
	addLayoutToRightListView("angularDumping", a_angularDumping);
	addLayoutToRightListView("angularVelocity", a_angularVelocity);
	addLayoutToRightListView("linearDumping", a_linearDumping);
	addLayoutToRightListView("linearVelocityX", a_linearVelocityX);
	addLayoutToRightListView("linearVelocityY", a_linearVelocityY);
	addLayoutToRightListView("gravityScale", a_gravityScale);
	addLayoutToRightListView("massCenterX", a_massCenterX);
	addLayoutToRightListView("massCenterY", a_massCenterY);
	addLayoutToRightListView("inertia", a_inertia);
	addLayoutToRightListView("mass", a_mass);
	addLayoutToRightListView("allowSleep", a_isAllowSleep, 1);
	addLayoutToRightListView("active", a_isActive, 1);
	addLayoutToRightListView("awake", a_isAwake, 1);
	addLayoutToRightListView("bullet", a_isBullet, 1);
	addLayoutToRightListView("fixedRotation", a_isFixedRotation, 1);
	// fixtureDef
	addLayoutToRightListView("categoryBits", a_filter_categoryBits, 2);
	addLayoutToRightListView("groundIndex", a_filter_groundIndex);
	addLayoutToRightListView("maskBits", a_filter_maskBits);
	addLayoutToRightListView("density", a_density);
	addLayoutToRightListView("fiction", a_friction);
	addLayoutToRightListView("resititution", a_restitution);
	addLayoutToRightListView("isSenSor", a_isSensor, 1);
}

ui::Widget* UILayer::addLayoutToRightListView(const std::string& name, Attribute attribute, int type){
	ui::Layout* layout = ui::Layout::create();

	ui::Text* text = ui::Text::create(name, "", 15);
	//text->setColor(Color3B(0,0,0));
	text->setPosition(Vec2(text->getContentSize().width / 2 + 10, text->getPosition().y));
	layout->addChild(text);
	layout->setContentSize(Size(text->getContentSize().width * 2, text->getContentSize().height));

	if (type == 0){ // textField
		ui::TextField* textField = ui::TextField::create("1.234567890", "", 14); // 保证初始的ContentSize能容纳10位的小数
		textField->setPosition(Vec2(text->getContentSize().width + textField->getContentSize().width / 2 + 20, text->getPosition().y));
		textField->setTextHorizontalAlignment(TextHAlignment::LEFT);
		textField->setColor(Color3B(0,0,0));
		textField->setPlaceHolder(""); 
		//textField->setText("");
		textField->setMaxLengthEnabled(true);
		textField->setMaxLength(10);
		Attribute* _attri = new Attribute(attribute);
		textField->setUserData(_attri);
		inputs[attribute] = textField;
		textField->setTouchEnabled(true);
		
		textField->addEventListener(CC_CALLBACK_2(UILayer::textFieldEvent, this));
		layout->addChild(textField);
		rightListView->addChild(layout);
		return textField;
	}
	else if (type == 1){	// checkBox
		ui::CheckBox* checkBox = ui::CheckBox::create("Box2DDesigner/check_box_normal.png",
			"Box2DDesigner/check_box_normal_press.png",
			"Box2DDesigner/check_box_active.png",
			"Box2DDesigner/check_box_normal_disable.png",
			"Box2DDesigner/check_box_active_disable.png");
		checkBox->setPosition(Vec2(text->getContentSize().width + checkBox->getContentSize().width / 2 + 20, text->getPosition().y));
		Attribute* _attri = new Attribute(attribute);
		checkBox->setUserData(_attri);
		inputs[attribute] = checkBox;
		checkBox->addEventListener(CC_CALLBACK_2(UILayer::checkBoxEvent,this));
		layout->addChild(checkBox);
		rightListView->addChild(layout);
		return checkBox;
	}
	else if (type == 2){ // button
		ui::Button* button = ui::Button::create();
		button->setTitleText("b2_kinematicBody"); // 设置对大长度的字段，保证contentSize
		button->setTitleFontSize(15);
		button->setTitleColor(Color3B(0,0,0));
		button->setPosition(Vec2(text->getContentSize().width + button->getContentSize().width / 2 + 20, text->getPosition().y));
		button->addTouchEventListener(CC_CALLBACK_2(UILayer::buttonEvent, this));
		Attribute* _attri = new Attribute(attribute);
		button->setUserData(_attri);
		inputs[attribute] = button;
		layout->addChild(button);
		rightListView->addChild(layout);
		return button;
	}
	return nullptr;
}

// 上方命令栏事件

void UILayer::shapeEvent(Ref* psender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		auto shapeButton = (ui::Button*)(psender);
		shapetype = ShapeType((shapetype + 1) % ShapeType::tyep_count);
		switch (shapetype){
		case EDGE: shapeButton->setTitleText("edge"); break;
		case CIRCLE: shapeButton->setTitleText("circle"); break;
		case CHAIN: shapeButton->setTitleText("chain"); break;
		case POLYGON: shapeButton->setTitleText("polygon"); break;
		default: break;
		}
	}
}

void UILayer::saveEvent(Ref* psender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		Box2DDesignerScene* _desinger = (Box2DDesignerScene*)(designer);
		ui::Button* button = (ui::Button*)psender;
		if (_desinger && button){
			if (button->getTitleText() == "save"){
				if (_desinger->usingSimpleSavingModel)
					_desinger->saveDataToXML(_desinger->defaultSaveFilepath);
				else
					_desinger->saveCompleteDataToXML(_desinger->defaultSaveFilepath);
			}
			else if (button->getTitleText() == "simple"){
				_desinger->usingSimpleSavingModel = false;
				button->setTitleText("complete");
			}
			else if (button->getTitleText() == "complete"){
				_desinger->usingSimpleSavingModel = true;
				button->setTitleText("simple");
			}
		}
	}
}

void UILayer::gravityEvent(Ref* psender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		Box2DDesignerScene* _desinger = (Box2DDesignerScene*)(designer);
		if (_desinger){
			b2Vec2 gravity = _desinger->world->GetGravity();
			if (gravity.y > -5){
				_desinger->world->SetGravity(b2Vec2(0, -9.8f));
			}
			else 
				_desinger->world->SetGravity(b2Vec2(0, 0));
		}
	}
}

// editbox 事件
void UILayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox){
	//cocos2d::log("hhhh");
	if (editBox->getName() == "savefilename"){
		if (savefilename == ""){
			editBox->setText("bos2ddesigner.xml");
		}
	}
}

void UILayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox){
	//cocos2d::log("dddd");
	if (editBox->getName() == "savefilename"){
		if (editBox->getText() != ""){
			Box2DDesignerScene* _desinger = (Box2DDesignerScene*)(designer);
			if (_desinger){
				savefilename = editBox->getText();
				_desinger->defaultSaveFilepath = "../Resources/Box2DDesigner/";
				_desinger->defaultSaveFilepath += savefilename;
			}
		}
	}
}

void UILayer::editBoxReturn(ui::EditBox* editBox){
	//cocos2d::log("dddd2");
}

void UILayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text){
	//cocos2d::log("dddd1");
}

// 右侧属性栏事件

void UILayer::textFieldEvent(Ref *pSender, ui::TextField::EventType type){
	switch (type)
	{
	case ui::TextField::EventType::ATTACH_WITH_IME:		/** 获得焦点 */
	{
		cocos2d::log("ATTACH_WITH_IME");
		currentSelectedTextField = (ui::TextField*)(pSender);
		currentOrignText = currentSelectedTextField->getString();
		currentSelectedTextField->setString("");
	}
		break;

	case ui::TextField::EventType::DETACH_WITH_IME:		/** 失去焦点 */
	{
		cocos2d::log("DETACH_WITH_IM");
		auto textField = (ui::TextField*)(pSender);
		/* 确认非空 */
		if (textField == nullptr) {
			currentSelectedTextField->setString(currentOrignText); 
			currentSelectedTextField = nullptr; 
			return;
		}
		auto attribute = (Attribute*)textField->getUserData();
		if (attribute == nullptr) {
			currentSelectedTextField->setString(currentOrignText);
			currentSelectedTextField = nullptr;
			return;
		}
		
		/* 属性设置*/

		auto inputstr = currentSelectedTextField->getString();
		/* 如果是a_name */
		if ((*attribute) == Attribute::a_name){
			if (currentRightShowBody){
				BodyData* bodydata = (BodyData*)currentRightShowBody->GetUserData();
				bodydata->name = a_name;
			}
		}
		/* 其他属性 */
		else if (isDigit(inputstr)){  // 是数值
			//cocos2d::log("%f", str2num<float>(inputstr));
			float num = str2num<float>(inputstr);
			setBodyData(*attribute, num);

			if ((*attribute) == Attribute::a_filter_groundIndex ){
				int16 intnum = (int(num))& (0xffff);
				currentSelectedTextField->setString(int16Tostr0x(intnum));
			}
			else if ((*attribute) == Attribute::a_filter_maskBits){
				uint16 intnum = (int(num))& (0xffff);
				currentSelectedTextField->setString(int16Tostr0x(intnum));
			}
		}
		else{
			currentSelectedTextField->setString(currentOrignText);
		}
		currentSelectedTextField = nullptr;
	}
		break;
	}
}

void UILayer::buttonEvent(Ref* psender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		/* 确认非空 */
		auto button = (ui::Button*)(psender);
		if (!button) return;
		Attribute* attribute =  (Attribute*)button->getUserData();
		if (!attribute) return;
		Box2DDesignerScene* _designer = (Box2DDesignerScene*)(designer);
		if (!_designer || ! _designer->leftSelectedBody)	return;

		/* 设置属性 */
		switch (*attribute){
		case a_type:
		{
			auto bodytype = _designer->leftSelectedBody->GetType();
			bodytype = (b2BodyType)((bodytype + 1) % 3);
			switch (bodytype){
			case b2BodyType::b2_staticBody:	((ui::Button*)inputs[a_type])->setTitleText("b2_staticBody"); break;
			case b2BodyType::b2_kinematicBody:	((ui::Button*)inputs[a_type])->setTitleText("b2_kinematicBody"); break;
			case b2BodyType::b2_dynamicBody:	((ui::Button*)inputs[a_type])->setTitleText("b2_dynamicBody"); break;
			}
			_designer->leftSelectedBody->SetType(bodytype);
		}
			break;
		case a_filter_categoryBits:
		{
			auto categoryBits = _designer->leftSelectedBody->GetFixtureList()->GetFilterData().categoryBits;
			if ((categoryBits & (0x8000)) == 0x8000) {
				categoryBits = 1; 
			}
			else categoryBits = (categoryBits << 1);  // 左移一位，相当于乘以2
			((ui::Button*)inputs[a_filter_categoryBits])->setTitleText(int16Tostr0x(categoryBits));
			b2Filter filter = _designer->leftSelectedBody->GetFixtureList()->GetFilterData();
			filter.categoryBits = categoryBits;
			_designer->leftSelectedBody->GetFixtureList()->SetFilterData(filter);
		}
			break;
		default : break;
		}
		
	}
}

void UILayer::setBodyData(Attribute attribute, float num){
	if (currentRightShowBody == nullptr) return;
	auto b = currentRightShowBody;
	b2MassData* m = nullptr;
	b2Fixture* f = nullptr;
	b2Filter filter;
	switch (attribute){
		/*bodydef*/
	case	a_positionX:	b->SetTransform(b2Vec2(num, b->GetPosition().y),b->GetAngle()); break;
	case	a_positionY:	b->SetTransform(b2Vec2(b->GetPosition().x, num), b->GetAngle()); break;
	case	a_angle:		b->SetTransform(b->GetPosition(), num); break;
	case	a_angularDumping:	b->SetAngularDamping(num); break;
	case	a_angularVelocity:	b->SetAngularVelocity(num); break;
	case	a_linearDumping:	b->SetLinearDamping(num); break;
	case	a_linearVelocityX:	b->SetLinearVelocity(b2Vec2(num,b->GetLinearVelocity().y)); break;
	case	a_linearVelocityY:	b->SetLinearVelocity(b2Vec2(b->GetLinearVelocity().x, num)); break;
	case	a_gravityScale:		b->SetGravityScale(num); break;
	case	a_massCenterX:		m = new b2MassData(); b->GetMassData(m); m->center.x = num; b->SetMassData(m); delete m; break;
	case	a_massCenterY:		m = new b2MassData(); b->GetMassData(m); m->center.y = num; b->SetMassData(m); delete m; break;
	case	a_inertia:			m = new b2MassData(); b->GetMassData(m); m->I = num; b->SetMassData(m); delete m; break;
	case	a_mass:				m = new b2MassData(); b->GetMassData(m); m->mass = num; b->SetMassData(m); delete m; break;
		/*fixturedef*/
	case	a_density:			f = b->GetFixtureList(); if (f == nullptr) break; f->SetDensity(num); break;
	case	a_friction:			f = b->GetFixtureList(); if (f == nullptr) break; f->SetFriction(num); break;
	case	a_restitution:		f = b->GetFixtureList(); if (f == nullptr) break; f->SetRestitution(num); break;

	case	a_filter_maskBits:	{
		f = b->GetFixtureList(); if (f == nullptr) break; 
		filter = f->GetFilterData();
		filter.maskBits = (int(num)) & (0xffff);	// 获取后16位
		f->SetFilterData(filter);
	}
		break;
	case	a_filter_groundIndex:{
		f = b->GetFixtureList(); if (f == nullptr) break; 
		filter = f->GetFilterData();
		filter.groupIndex = (int(num)) & (0xffff);	// 获取后16位
		f->SetFilterData(filter);
	}
		break;
	}
}

void UILayer::checkBoxEvent(Ref *pSender, ui::CheckBox::EventType type){
	/* 判断非空 */
	auto checkBox = (ui::CheckBox*) pSender;
	if (!checkBox) return;
	auto attribute = (Attribute*)checkBox->getUserData();
	if (!attribute) return;
	if (!currentRightShowBody) return;
	auto b = currentRightShowBody;

	/* 设置属性 */
	b2Fixture* f = nullptr;
	switch (type){
	case ui::CheckBox::EventType::SELECTED:
	{
		switch (*attribute){
		case a_isAllowSleep:	b->SetSleepingAllowed(true); break;
		case a_isActive:		b->SetActive(true); break;
		case a_isAwake:			b->SetAwake(true); break;
		case a_isBullet:		b->SetBullet(true); break;
		case a_isFixedRotation:	b->SetFixedRotation(true); break;
		case a_isSensor:		f = b->GetFixtureList(); f->SetSensor(true); break;
		}
	}
		break;

	case ui::CheckBox::EventType::UNSELECTED:
	{
		switch (*attribute){
		case a_isAllowSleep:	b->SetSleepingAllowed(false); break;
		case a_isActive:		b->SetActive(false); break;
		case a_isAwake:			b->SetAwake(false); break;
		case a_isBullet:		b->SetBullet(false); break;
		case a_isFixedRotation:	b->SetFixedRotation(false); break;
		case a_isSensor:		f = b->GetFixtureList(); f->SetSensor(false); break;
		}
	}
		break;

	}
}

/* action */
void UILayer::OutTopListView(){		// 使得topListView下拉
	topListViewOut = true;
	MoveBy* out = MoveBy::create(0.1, Vec2(0, -uiLayerHeight));
	topListView->runAction(out);
}

void UILayer::OutRightListView(){	// 选中物体时使得rightListView下拉
	rightListViewOut = true;
	initRightListViewWithSelectedBody();
	MoveBy* out = MoveBy::create(0.1, Vec2(-uiRightWidth, 0));
	rightListView->runAction(out);
}

void UILayer::BackTopListView(){		// 使得topListView回拉
	topListViewOut = false;
	MoveBy* back = MoveBy::create(0.1, Vec2(0, uiLayerHeight));
	topListView->runAction(back);
}

void UILayer::BackRightListView(){	// 使得rightListView右拉
	rightListViewOut = false;
	MoveBy* back = MoveBy::create(0.1, Vec2(uiRightWidth, 0));
	rightListView->runAction(back);
}

/*  */
void UILayer::initRightListViewWithSelectedBody(){
	Box2DDesignerScene* _designer = (Box2DDesignerScene*)(designer);
	if (_designer && _designer->leftSelectedBody && (_designer->leftSelectedBody != currentRightShowBody)){
		currentRightShowBody = _designer->leftSelectedBody;
		b2Body* b = _designer->leftSelectedBody;
		
		BodyData* bd = (BodyData*) (b->GetUserData());
		if (inputs[a_name]) ((ui::TextField*)inputs[a_name])->setText(bd->name);

		/* bodydef*/
		if (inputs[a_positionX])  ((ui::TextField*)inputs[a_positionX])->setText(num2str(b->GetPosition().x));
		if (inputs[a_positionY])  ((ui::TextField*)inputs[a_positionY])->setText(num2str(b->GetPosition().y));
		if (inputs[a_type]) {
			auto _type = b->GetType();
			switch (_type){
			case b2BodyType::b2_staticBody :	((ui::Button*)inputs[a_type])->setTitleText("b2_staticBody"); break;
			case b2BodyType::b2_kinematicBody :	((ui::Button*)inputs[a_type])->setTitleText("b2_kinematicBody"); break;
			case b2BodyType::b2_dynamicBody:	((ui::Button*)inputs[a_type])->setTitleText("b2_dynamicBody"); break;
			}
		}
		if (inputs[a_angle])  ((ui::TextField*)inputs[a_angle])->setText(num2str(b->GetAngle()));
		if (inputs[a_angularDumping])  ((ui::TextField*)inputs[a_angularDumping])->setText(num2str(b->GetAngularDamping()));
		if (inputs[a_angularVelocity])  ((ui::TextField*)inputs[a_angularVelocity])->setText(num2str(b->GetAngularVelocity()));
		if (inputs[a_linearDumping])  ((ui::TextField*)inputs[a_linearDumping])->setText(num2str(b->GetLinearDamping()));
		if (inputs[a_linearVelocityX])  ((ui::TextField*)inputs[a_linearVelocityX])->setText(num2str(b->GetLinearVelocity().x));
		if (inputs[a_linearVelocityY])  ((ui::TextField*)inputs[a_linearVelocityY])->setText(num2str(b->GetLinearVelocity().y));
		if (inputs[a_gravityScale])  ((ui::TextField*)inputs[a_gravityScale])->setText(num2str(b->GetGravityScale()));
		b2MassData* massdata = new b2MassData();
		b->GetMassData(massdata);
		if (inputs[a_massCenterX])  ((ui::TextField*)inputs[a_massCenterX])->setText(num2str(massdata->center.x));
		if (inputs[a_massCenterY])  ((ui::TextField*)inputs[a_massCenterY])->setText(num2str(massdata->center.y));
		if (inputs[a_inertia])  ((ui::TextField*)inputs[a_inertia])->setText(num2str(massdata->I));
		if (inputs[a_mass])  ((ui::TextField*)inputs[a_mass])->setText(num2str(massdata->mass));
		delete massdata;
		if (inputs[a_isAllowSleep])  ((ui::CheckBox*)inputs[a_isAllowSleep])->setSelected(b->IsSleepingAllowed());
		if (inputs[a_isActive])  ((ui::CheckBox*)inputs[a_isActive])->setSelected((b->IsActive()));
		if (inputs[a_isAwake])  ((ui::CheckBox*)inputs[a_isAwake])->setSelected((b->IsAwake()));
		if (inputs[a_isBullet])  ((ui::CheckBox*)inputs[a_isBullet])->setSelected((b->IsBullet()));
		if (inputs[a_isFixedRotation])  ((ui::CheckBox*)inputs[a_isFixedRotation])->setSelected((b->IsFixedRotation()));

		/* fixturedef */
		auto f = b->GetFixtureList();		// 此处如果有多个fixture的话应绑定
		if (inputs[a_filter_categoryBits])  {
			((ui::Button*)inputs[a_filter_categoryBits])->setTitleText(int16Tostr0x(f->GetFilterData().categoryBits));
		}
		if (inputs[a_filter_groundIndex])  {
			((ui::TextField*)inputs[a_filter_groundIndex])->setText(int16Tostr0x(f->GetFilterData().groupIndex));
		}
		if (inputs[a_filter_maskBits])  {
			((ui::TextField*)inputs[a_filter_maskBits])->setText(int16Tostr0x(f->GetFilterData().maskBits));
		}
		if (inputs[a_density])  ((ui::TextField*)inputs[a_density])->setText(num2str(f->GetDensity()));
		if (inputs[a_friction])  ((ui::TextField*)inputs[a_friction])->setText(num2str(f->GetFriction()));
		if (inputs[a_restitution])  ((ui::TextField*)inputs[a_restitution])->setText(num2str(f->GetRestitution()));
		if (inputs[a_isSensor])  ((ui::CheckBox*)inputs[a_isSensor])->setSelected((f->IsSensor()));
	}
}

/***********************************************************************************************************************/

/* Box2DDesignerScene 类 */

Scene* Box2DDesignerScene::createScene(){
	Scene* scene = Scene::create();
	Box2DDesignerScene* rootLayer = Box2DDesignerScene::create();
	scene->addChild(rootLayer);
	scene->addChild(rootLayer->uiLayer);
	return scene;
}

Box2DDesignerScene::~Box2DDesignerScene()
{
	delete selectedPoints;
}

bool Box2DDesignerScene::init(){
	if (!Layer::init()) return false;
	
	/* 初始化物理引擎的信息 以及bodys向量的创建 */
	initPhysics();
	drawZorder = DrawZOrder;

	//setScale(0.5);

	/* 初始化选中点的容器 以及 鼠标点击的数据*/
	disable = false;
	selectedPoints = new std::vector <Vec2>();
	minMovedDistance = 5;
	pointRange = 10;
	scaleRate = 0.9;
	buttonOfDownMouse = NONE;
	currentMousePoint = visiableSize / 2;
	scalerange.min = 0.1;					// 如果使用 noScaleMOvedPosition当伸缩率低于0.15时，由于浮点数计算误差变得极大，而且由于效果叠加，导致完全
	scalerange.max = 10;
	noScaleMovedPosition = getPosition();
	lastScaleAnchorRate = getScale();
	leftSelectedBody = nullptr;

	/* designer 的 UI界面设计 */
	visiableSize = Director::getInstance()->getVisibleSize();
	uiLayer = UILayer::create();
	uiLayer->setPosition(Vec2::ZERO);  // 相对scene
	uiLayer->designer = this;

	/*Sprite* s = Sprite::create("car.jpg");
	// s->setColor(Color3B(255,0,0));
	s->setPosition(visiableSize / 2);
	s->setName("s");
	addChild(s);*/

	// create a polygon body
	/*b2Vec2 v[4];
	v[0].Set(-1.5, 1.5);
	v[1].Set(1.5, 1.5);
	v[2].Set(1.5, -1.5);
	v[3].Set(-1.5, -1.5);
	std::vector<Vec2>* ve = new std::vector<Vec2>();
	for(int i =0 ; i < 4; i++) ve->push_back(Vec2(v[i].x * RTM_RATIO, v[i].y * RTM_RATIO));

	addEdge(Vec2(20,20), Vec2(100,100));
	addCircle(Vec2(40,40), 10);
	addChainEdge(ve);
	addChainLoop(ve);
	addPolygon(ve);*/

	/* xml数据存储初始化 */
	pDoc = nullptr;
	saving = false;
	changed = false;
	saveOfFrames = 3600;
	framecount = 0;
	defaultSaveFilepath = "";
	usingSimpleSavingModel = true;

	setting();
	loadBackGroundIamge();
	//xmltest();
	//saveDataToXML(std::string("C:\\Users\\hui\\Desktop\\desktop\\test.xml"));

	

	this->scheduleUpdate();
	return true;
}

void Box2DDesignerScene::onEnter(){
	Layer::onEnter();

	auto listenerMouse = EventListenerMouse::create();
	listenerMouse->onMouseDown = CC_CALLBACK_1(Box2DDesignerScene::onMouseDown, this);
	listenerMouse->onMouseMove = CC_CALLBACK_1(Box2DDesignerScene::onMouseMove, this);
	listenerMouse->onMouseScroll = CC_CALLBACK_1(Box2DDesignerScene::onMouseScroll, this);
	listenerMouse->onMouseUp = CC_CALLBACK_1(Box2DDesignerScene::onMouseUp, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerMouse, this);
}

void Box2DDesignerScene::update(float dt){
	if (dt <= 0) dt = Director::getInstance()->getAnimationInterval();

	/* 改编后定是保存数据 */
	if (changed){
		framecount++;
		if (framecount >= saveOfFrames && !saving) {
			if (usingSimpleSavingModel)
				saveDataToXML(defaultSaveFilepath);  // framecount, saving和change在该函数里改变
			else
				saveCompleteDataToXML(defaultSaveFilepath);  // framecount, saving和change在该函数里改变
		}
	}

	world->Step(dt, 8, 3);
}

/* 物理世界和绘制调试线，以及绘图的成员 */

void Box2DDesignerScene::initPhysics()
{
	b2Vec2 gravity;
	gravity.Set(0.0f, 0.0f);
	world = new b2World(gravity);
	world->SetAllowSleeping(true);			// 允许物体进入休眠状态
	world->SetContinuousPhysics(true);      // 使用连续物理碰撞检测
	world->SetContactListener(this);			// 设置碰撞监听器

	debugdraw = new GLESDebugDraw(RTM_RATIO);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	//flags += b2Draw::e_jointBit;
	//flags += b2Draw::e_aabbBit;
	//flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
	debugdraw->SetFlags(flags);
	world->SetDebugDraw(debugdraw);

	///bodys = new std::vector<b2Body*>();
}

void Box2DDesignerScene::draw(Renderer *renderer, const Mat4& transform, uint32_t flags){
	Layer::draw(renderer, transform, flags);
	
	_customCmd.init(drawZorder, transform, flags);				// 这个_customeCmd和layer 中的物体不再一个Z坐标系，只能设成要么全在上面，要么全在下面
	_customCmd.func = CC_CALLBACK_0(Box2DDesignerScene::myDraw, this, transform, flags);
	renderer->addCommand(&_customCmd);
}

void Box2DDesignerScene::myDraw(const Mat4 &transform, uint32_t flags){
	kmGLPushMatrix();
	kmGLLoadMatrix(&transform);

	world->DrawDebugData();

	//draw
	CHECK_GL_ERROR_DEBUG();
	int pointCount = selectedPoints->size();  // size一定要转为int, 否则负数是最大数

	DrawPrimitives::setPointSize(4);
	DrawPrimitives::setDrawColor4B(255, 0, 0, 255);
	for (int i = 0; i < pointCount; i++){
		DrawPrimitives::drawPoint(selectedPoints->at(i));
	}
	
	glLineWidth(1.5f);
	DrawPrimitives::setDrawColor4B(125, 0, 0, 255);
	for (int i = 0; i < pointCount - 1; i++){  
		DrawPrimitives::drawLine(selectedPoints->at(i),selectedPoints->at(i+1));
	}
	bool insidetop = (currentMousePoint.y > visiableSize.height - uiLayer->uiLayerHeight) && (uiLayer->topListViewOut);
	bool insideright = (currentMousePoint.y < visiableSize.height - uiLayer->uiLayerHeight) && 
		(currentMousePoint.x > visiableSize.width - uiLayer->uiRightWidth) && (uiLayer->rightListViewOut);

	if (pointCount && !insidetop && !insideright)
		DrawPrimitives::drawLine(selectedPoints->at(pointCount - 1), currentMousePoint);

	CHECK_GL_ERROR_DEBUG();

	//end draw
	kmGLPopMatrix();
}

/* 鼠标点击事件 */

void Box2DDesignerScene::onMouseDown(Event* event){
	/* 当鼠标某键已按下时，禁用其他键 */
	if (disable || buttonOfDownMouse != NONE) return;
	EventMouse * e = (EventMouse*)event;
	
	/* e->getCursorX(), e->getCursorY()左上角是原点，y轴向上, 这个position屏幕，要画在layer里，所以要转化为layer的坐标 */
	currentSelectedPoint = getRalativePos(Vec2(e->getCursorX(), e->getCursorY() + visiableSize.height));
	LastPoint = cocos2d::Vec2(e->getCursorX(), e->getCursorY() + visiableSize.height);
	/* 期望在屏幕上的是Vec2(e->getCursorX(), e->getCursorY() + visiableSize.height)，
	真实画在在屏上的是Vec2(e->getCursorX(), e->getCursorY() + visiableSize.height) + getPosition(),
	所以要再减getPosition() */

	/* 当鼠标带你在UI界面层时无反应 */
	if((LastPoint.y >= visiableSize.height - uiLayer->uiLayerHeight) && uiLayer->topListViewOut) return;
	if ((LastPoint.x >= visiableSize.width - uiLayer->uiRightWidth) && uiLayer->rightListViewOut) return;

	hasMoved = false;
	buttonOfDownMouse = (ButtonMouse) e->getMouseButton();
	leftSelectedBody = nullptr;			// 单击任何键，先取消选择

	switch (buttonOfDownMouse){
	case LEFT : 
		if (selectedPoints->empty()) leftSelectedBody = getSelectedBody();
		break;
	case RIGHT: break;
	case MIDDLE: break;
	default: break;
	}

	if (selectedPoints->empty() && uiLayer->rightListViewOut && (LastPoint.x < (visiableSize.width - uiLayer->uiRightWidth))
		&& (!leftSelectedBody))	uiLayer->BackRightListView();
}

void Box2DDesignerScene::onMouseMove(Event* event){	//移动时左键的优先级较高
	EventMouse * e = (EventMouse*)event;
	// ui 层的listview的拉下与收回动作,懒得给ui层用点击事件了，直接借用Designer的
	Vec2 mousepoint = cocos2d::Vec2(e->getCursorX(), e->getCursorY() + visiableSize.height);
	if (buttonOfDownMouse == NONE && selectedPoints->empty()){
		if ((!uiLayer->topListViewOut) && mousepoint.y >= (visiableSize.height - uiLayer->outrange)) uiLayer->OutTopListView();
		else if (uiLayer->topListViewOut && mousepoint.y < (visiableSize.height - uiLayer->uiLayerHeight)) uiLayer->BackTopListView();

		/*if ((!uiLayer->rightListViewOut) && mousepoint.x >= (visiableSize.width - uiLayer->outrange)) uiLayer->OutRightListView();
		else if (uiLayer->rightListViewOut && (mousepoint.x < (visiableSize.width - uiLayer->uiRightWidth))
			&& (!leftSelectedBody))	uiLayer->BackRightListView();*/
	}


	currentMousePoint = getRalativePos(Vec2(e->getCursorX(), e->getCursorY() + visiableSize.height));

	/* 每次鼠标移动，伸缩anchor变更，进行更新 */
	lastScaleAnchorRate = getScale();
	noScaleMovedPosition = getPosition();

	// /* 当鼠标靠近起始点时，强制它到改点 */

	/* 没有按下键的移动不作处理  */
	if (disable || buttonOfDownMouse == NONE) return;
	
	/* 由于精度不是很要紧，所以这里的距离用 abs(x2+y2 - x1-y1) 代替简化计算 */
	if (currentMousePoint.distance(currentSelectedPoint) < minMovedDistance) return;

	hasMoved = true;

	Vec2 delta;	/* 不可使用currentMousePoint currentSelectedPoint的差求解delta, 因为前后两帧的getPosition()在移动中是不同的 */
	switch (e->getMouseButton()){
	case LEFT : 
		delta = Vec2(e->getCursorX() - LastPoint.x, e->getCursorY() + visiableSize.height - LastPoint.y);
		if (leftSelectedBody == nullptr){
			moveWorld(delta);
		}
		else{
			moveBody(delta);
		}
		break;
	case RIGHT:  rotateWorld(); break;
	case MIDDLE: middelMouseMoved(); break;
	default : break;
	}
	LastPoint = cocos2d::Vec2(e->getCursorX(), e->getCursorY() + visiableSize.height);
}

void Box2DDesignerScene::onMouseScroll(Event* event){
	if (disable) return;

	EventMouse * e = (EventMouse*)event;
	currentMousePoint = getRalativePos(Vec2(e->getCursorX(), e->getCursorY() + visiableSize.height));
	float orignScale = getScale();
	Vec2 currentMousePosFromWin = Vec2(e->getCursorX(), e->getCursorY() + visiableSize.height);
	Vec2 currentMousePosFromOrignLayer = getRalativePos(currentMousePosFromWin);  
	/* 缩放世界 */
	scaleWorld(event);			
	/* 移动世界，使得看起来好像缩放中心是鼠标位置 */
	
	Vec2 delta = (currentMousePosFromOrignLayer - visiableSize / 2) * (1 - getScale() / lastScaleAnchorRate)
		*  lastScaleAnchorRate; // 先用鼠标在上次伸缩的Layer坐标系下的坐标求出本次放缩后他会移动到的在上次坐标系下的位置
								// 以次求出距离，然后再将该距离换算会屏幕坐标系（Scene坐标系），setPosition();
	/*cocos2d::log("currentMousePosFromWin : %f, %f",currentMousePosFromWin.x, currentMousePosFromWin.y);
	cocos2d::log("currentMousePosFromOrignLayer : %f %f, scale: %f , lastscale: %f", currentMousePosFromOrignLayer.x, currentMousePosFromOrignLayer.y, getScale(), lastScaleAnchorRate);
	cocos2d::log("noScaleMovedPosition : %f %f, delta : %f, %f", noScaleMovedPosition.x, noScaleMovedPosition.y, delta.x, delta.y);
	cocos2d::log("Position : %f %f",getPosition().x, getPosition().y );*/
	setPosition(noScaleMovedPosition + delta);

	/* 使用以下方法时，因为每次滚动误差会积累 ，使得不能完全以某个点为中心缩放，但不会出现骤变（去掉以下代码就能看到骤变效果）*/
	/* 不能只使用getScale()，这是获得相对初始的scale,
	/* 但position已不是初始的position */
	lastScaleAnchorRate = getScale();
	noScaleMovedPosition = getPosition();  // 这句代码使得noScaleMovedPosition基本完全与getPosition()等价,相当于关闭了noScaleMovedPosition
	/*float nowScale = getScale();
	
	Vec2 delta = (currentMousePosFromWin - visiableSize / 2) * ((1 - nowScale) - (1 - orignScale));   // 移动总距离 - 上次移动总距离
	setPosition(getPosition() + delta);*/
}

void Box2DDesignerScene::onMouseUp(Event* event){
	if (disable) return;
	EventMouse * e = (EventMouse*)event;
	/* 已禁用键的松开不必处理 ,鼠标拖动过不必处理 */
	if (buttonOfDownMouse != e->getMouseButton() || hasMoved) {
		buttonOfDownMouse = NONE;
		return;
	}

	switch (buttonOfDownMouse){
	case LEFT:
		if (leftSelectedBody == nullptr)	addBodyOrPoint(e);
		else{
			selectedBody(e);
		}
		break;
	case RIGHT: 
		if (!selectedPoints->empty()) cancellLastPoint();
		else deleteSelectedBody();
		break;
	case MIDDLE: 
		sureToCreateChainEdge();
		break;
	default: break;
	}

	buttonOfDownMouse = NONE;
}

Vec2 Box2DDesignerScene::getRalativePos(Vec2 pos){
	pos = pos - getPosition();
	pos = (pos - Vec2(visiableSize / 2)) / getScale() + Vec2(visiableSize / 2);
	return pos;
}

/* 鼠标响应事件 */

void Box2DDesignerScene::moveWorld(Vec2 delta){
#ifndef NODEBUG
	//cocos2d::log("move world %f %f", delta.x, delta.y);
#endif
	setPosition(getPosition() + delta);		
	noScaleMovedPosition += delta;
	/* 平行世界观念，移动layer，由layer draw的Box2d调试图形也会动，产生可视化世界与物理世界分离，但没有影响 */
	changed = true;
}

void Box2DDesignerScene::moveBody(Vec2 delta){
	if (leftSelectedBody){
		delta = delta / RTM_RATIO;
		b2Vec2 pos = leftSelectedBody->GetPosition();
		leftSelectedBody->SetTransform(b2Vec2(pos.x + delta.x, pos.y+delta.y), 
			leftSelectedBody->GetAngle());
	}
}

void Box2DDesignerScene::rotateWorld(){
#ifndef NODEBUG
	cocos2d::log("rotate world");
#endif
}

void Box2DDesignerScene::addBodyOrPoint(EventMouse * e){
#ifndef NODEBUG
	cocos2d::log("add point");
#endif
	selectedPoints->push_back(currentSelectedPoint);

	Vec2 p;
	int count = selectedPoints->size();
	switch (uiLayer->shapetype){
	case EDGE:
		if(count == 2){
			addEdge (selectedPoints->at(0), selectedPoints->at(1)); 
			selectedPoints->clear();
			changed = true;
		}
		break;

	case CIRCLE: 
		if(count == 2) {
			p = selectedPoints->at(0) - selectedPoints->at(1);
			addCircle(selectedPoints->at(0), sqrt(p.x * p.x + p.y * p.y));
			selectedPoints->clear();
			changed = true;
		}
		break;

	case CHAIN: 
		if(count >= 3){ 
			p = selectedPoints->at(selectedPoints->size() - 1) - selectedPoints->at(0); // last point - first point
			if (sqrt(p.x * p.x + p.y * p.y) <= minMovedDistance)  {
				selectedPoints->pop_back();
				addChainLoop(selectedPoints);
				selectedPoints->clear();
				changed = true;
			}
		}
		break;

	case POLYGON : 
		p = selectedPoints->at(selectedPoints->size() - 1) - selectedPoints->at(0); // last point - first point
		if((count >= 3 && abs(p.x) + abs(p.y) <= minMovedDistance ) || count == 8){ 
			selectedPoints->pop_back();
			addPolygon(selectedPoints);
			selectedPoints->clear();
			changed = true;
		}
		break;

	default : break;
	}
}

void Box2DDesignerScene::selectedBody(EventMouse * e){
	if (leftSelectedBody){
		//cocos2d::log("selected");
		if (!uiLayer->rightListViewOut)	uiLayer->OutRightListView(); // 选中物体没出来属性栏
		else uiLayer->initRightListViewWithSelectedBody();			 // 选中物体属性栏出来了。在函数内会判定当前选中body和当前显示body是否相同
	}
}

b2Body* Box2DDesignerScene::getSelectedBody(){
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()){
		for (auto fixture = b->GetFixtureList(); fixture; fixture = fixture->GetNext()){  	/// Get the list of all fixtures attached to this body.(一个物体可能有多个物体）
			auto shapetype = fixture->GetType();
			if (shapetype == b2Shape::e_edge){
				b2EdgeShape* shape = (b2EdgeShape*)fixture->GetShape();
				b2Vec2 pos = b->GetPosition();
				b2Vec2 start = shape->m_vertex1;
				b2Vec2 end = shape->m_vertex2;
				Vec2 startpoint = Vec2(start.x + pos.x, start.y + pos.y) * RTM_RATIO;
				Vec2 endpoint = Vec2(end.x + pos.x, end.y + pos.y) * RTM_RATIO;
#ifndef NODEBUG
				cocos2d::log("%f %f, %f %f , %f %f", startpoint.x, startpoint.y, endpoint.x, endpoint.y, currentSelectedPoint.x, currentSelectedPoint.y);
#endif
				if (isNearLine(startpoint, endpoint, currentSelectedPoint)){
					return b;
				}
			}
			else if (shapetype == b2Shape::e_chain){
				b2ChainShape* shape = (b2ChainShape*)fixture->GetShape();
				bool isbreak = false;
				for (int i = 0; i < shape->m_count - 1; i++){
					b2Vec2 start = shape->m_vertices[i];
					b2Vec2 end = shape->m_vertices[i + 1];

					b2Vec2 pos = b->GetPosition();
					start = b2Vec2(start.x+pos.x, start.y + pos.y);
					end = b2Vec2(end.x + pos.x, end.y + pos.y);

					Vec2 startpoint = Vec2(start.x, start.y) * RTM_RATIO;
					Vec2 endpoint = Vec2(end.x, end.y) * RTM_RATIO;
					if (isNearLine(startpoint, endpoint, currentSelectedPoint)){
						return b;
					}
				}
				if (isbreak) break;
				// if is chainloop there is another edge
				BodyData* bodydata = (BodyData*)b->GetUserData();
				if (bodydata && bodydata->ischainloop){
					b2Vec2 start = shape->m_vertices[shape->m_count];
					Vec2 startpoint = Vec2(start.x, start.y) * RTM_RATIO;
					b2Vec2 end = shape->m_vertices[0];
					Vec2 endpoint = Vec2(end.x, end.y) * RTM_RATIO;
					if (isNearLine(startpoint, endpoint, currentSelectedPoint)){
						return b;
					}
				}
			}
			else if (shapetype == b2Shape::e_circle || shapetype == b2Shape::e_polygon){
				if (fixture->TestPoint(b2Vec2
					(currentSelectedPoint.x / RTM_RATIO, currentSelectedPoint.y / RTM_RATIO))){
					return b;		// 选到了就马上break，删了不能GetNext()
				}
			}

		}
	}
	return nullptr;
}

void Box2DDesignerScene::cancellLastPoint(){
#ifndef NODEBUG
	cocos2d::log("cancell");
#endif
	if (!selectedPoints->empty())
		selectedPoints->pop_back();
}

float Box2DDesignerScene::scaleWorld(Event* event){
	EventMouse * e = (EventMouse*)event;
	int scaletime = (int)(e->getScrollY());
	float scale = 1.0;
	if (scaletime > 0)
		for (int i = 0; (i < scaletime) && (getScale()*scale > scalerange.min); i++)
			scale *= scaleRate;
	else if (scaletime < 0)
		for (int i = 0; (i < -scaletime) && (getScale()*scale < scalerange.max); i++)
			scale /= scaleRate;
	setScale(getScale()* scale);
	//cocos2d::log("sacle : %f, scaletime: %d", getScale(), scaletime);
	changed = true;
	return scale;
}

void Box2DDesignerScene::deleteSelectedBody(){
#ifndef NODEBUG
	cocos2d::log("deleted");
#endif
	b2Body* body = getSelectedBody();
	auto bodydata = (BodyData*)(body->GetUserData());
	deleteBodysTag.push_back(bodydata->tag);
	if (body) world->DestroyBody(body);
	/* 以下代码被以上两句代替，主要内容放到了函数getSelectedBody()中 */
	/*
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()){
		bool breakout = false;
		for(auto fixture = b->GetFixtureList();fixture;fixture=fixture->GetNext()){  	/// Get the list of all fixtures attached to this body.(一个物体可能有多个物体）
			auto shapetype = fixture->GetType();
			if( shapetype == b2Shape::e_edge){
				b2EdgeShape* shape = (b2EdgeShape*) fixture->GetShape();
				b2Vec2 pos = b->GetPosition();
				b2Vec2 start = shape->m_vertex1;
				b2Vec2 end = shape->m_vertex2;
				Vec2 startpoint = Vec2(start.x + pos.x, start.y + pos.y) * RTM_RATIO;
				Vec2 endpoint = Vec2(end.x + pos.x, end.y + pos.y) * RTM_RATIO;
#ifndef NODEBUG
				cocos2d::log("%f %f, %f %f , %f %f", startpoint.x, startpoint.y, endpoint.x, endpoint.y, currentSelectedPoint.x, currentSelectedPoint.y);
#endif
				if (isNearLine(startpoint, endpoint, currentSelectedPoint)){
					world->DestroyBody(b);
					changed = true;
					breakout = true;
					break;
				}
			}
			else if(shapetype == b2Shape::e_chain){
				b2ChainShape* shape = (b2ChainShape*)fixture->GetShape();
				bool isbreak = false;
				for (int i = 0; i < shape->m_count - 1; i++){
					b2Vec2 start = shape->m_vertices[i];
					Vec2 startpoint = Vec2(start.x, start.y) * RTM_RATIO;
					b2Vec2 end = shape->m_vertices[i+1];
					Vec2 endpoint = Vec2(end.x, end.y) * RTM_RATIO;
					if (isNearLine(startpoint, endpoint, currentSelectedPoint)){
						world->DestroyBody(b);
						changed = true;
						isbreak = true;
						breakout = true;
						break;
					}
				}
				if (isbreak) break;
				// if is chainloop there is another edge
				BodyData* bodydata = (BodyData*) b->GetUserData();
				if (bodydata && bodydata->ischainloop){
					b2Vec2 start = shape->m_vertices[shape->m_count];
					
					b2Vec2 end = shape->m_vertices[0];

					b2Vec2 pos = b->GetPosition();
					start = b2Vec2(start.x+pos.x, start.y + pos.y);
					end = b2Vec2(end.x + pos.x, end.y + pos.y);

					Vec2 startpoint = Vec2(start.x, start.y) * RTM_RATIO;
					Vec2 endpoint = Vec2(end.x, end.y) * RTM_RATIO;
					if (isNearLine(startpoint, endpoint, currentSelectedPoint)){
						world->DestroyBody(b);
						changed = true;
						breakout = true;
						break;
					}
				}
			}
			else if ( shapetype == b2Shape::e_circle || shapetype == b2Shape::e_polygon){
				if(fixture->TestPoint(b2Vec2
				(currentSelectedPoint.x / RTM_RATIO, currentSelectedPoint.y / RTM_RATIO))){
					world->DestroyBody(b);
					changed = true;
					breakout = true;
					break;					// 选到了就马上break，删了不能GetNext()
				}
			}
		}
		if (breakout) break;
	}
	*/
}

void Box2DDesignerScene::middelMouseMoved(){			/* 中键点击+移动事件*/
	//cocos2d::log("middle mouse move");
}

void Box2DDesignerScene::sureToCreateChainEdge(){		/* 中键盘点击事件 */
	//cocos2d::log("middle mouse click");
	if (int(selectedPoints->size()) >= 2 && uiLayer->shapetype == CHAIN){
		addChainEdge(selectedPoints);
		selectedPoints->clear();
		changed = true;
	}
}

/*该函数 已使用 b2Shape的getType()代替*/
ShapeType Box2DDesignerScene::getShapeType(b2Shape* shape){
	b2EdgeShape* edge =  dynamic_cast<b2EdgeShape*>(shape);
	if(edge) return EDGE;
	b2ChainShape* chain =  dynamic_cast<b2ChainShape*>(shape);
	if(chain) return CHAIN;
	b2CircleShape* circle =  dynamic_cast<b2CircleShape*>(shape);
	if(circle) return CIRCLE;
	b2PolygonShape* polygon =  dynamic_cast<b2PolygonShape*>(shape);
	if(polygon) return POLYGON;
	return ShapeType::tyep_count;  // 如果不是ShpeType里的反回改值
}

bool Box2DDesignerScene::isNearLine(Vec2 startpoint, Vec2 endpoint, Vec2 targetpoint){
	Vec2 middlepoint = (startpoint + endpoint) / 2;
	if(startpoint.x == endpoint.x){  // L : x = x0
		float height = abs(startpoint.y - endpoint.y);
		float disx = abs(targetpoint.x - middlepoint.x);
		float disy = abs(targetpoint.y - middlepoint.y);
		if( disx <= pointRange && disy < height / 2 + pointRange){
			return true;
		}
		else return false;
	}
	if(startpoint.y == endpoint.y){  // L : y = y0
		
		float width = abs(startpoint.x - endpoint.x);
		float disx = abs(targetpoint.x - middlepoint.x);
		float disy = abs(targetpoint.y - middlepoint.y);

		if( disy <= pointRange && disx < width / 2 + pointRange){
			return true;
		}
		else return false;
	}

	else{  // L : y = a*x + b
		float a1 = (endpoint.y - startpoint.y) / (endpoint.x - startpoint.x);
		float b1 = middlepoint.y - a1 * middlepoint.x;
		// 是距离 < pointRange 的变式
		bool in1 = (abs(targetpoint.y - a1 * targetpoint.x -b1) - pointRange  * sqrt(1 + a1 * a1)) <= 0;

		float a2 = -1 / a1;
		float b2 = middlepoint.y - a2 * middlepoint.x;
		float dis = startpoint.distance(endpoint);
		bool in2 = (abs(targetpoint.y - a2 * targetpoint.x -b2) - (dis/2+pointRange) * sqrt(1 + a2 * a2)) <= 0;

		if(in1 && in2) return true;
		return false;
	}
}

/** 添加物理body */

void Box2DDesignerScene::addEdge(Vec2 startPoint, Vec2 endPoint){
	//cocos2d::log("add %f %f, %f %f, %f %f", startPoint.x, startPoint.y, endPoint.x, endPoint.y, getPosition().x, getPosition().y);

	startPoint = startPoint / RTM_RATIO;
	endPoint = endPoint / RTM_RATIO;
	Vec2 middlePoint = (startPoint + endPoint) / 2;

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(middlePoint.x, middlePoint.y);
	b2Body* ground = world->CreateBody(&groundBodyDef);
	b2EdgeShape groundShape;
	
	groundShape.Set(b2Vec2(startPoint.x - middlePoint.x, startPoint.y-middlePoint.y)
		, b2Vec2(endPoint.x-middlePoint.x, endPoint.y-middlePoint.y));
	ground->CreateFixture(&groundShape, 0);//静态物体密度没意义，设为0

	BodyData * bodydata = new BodyData();
	bodydata->tag = world->GetBodyCount() - 1;
	if (!deleteBodysTag.empty()) {
		auto end = deleteBodysTag.end();  // end是终止符号，没有对应元素
		end--;
		bodydata->tag = *(end);
		deleteBodysTag.pop_back();
	}
	bodydata->name = "body" + num2str(bodydata->tag);
	ground->SetUserData(bodydata);
}

void Box2DDesignerScene::addCircle(Vec2 center, float radius){
	b2BodyDef bodydef;
	bodydef.position = b2Vec2(center.x / RTM_RATIO, center.y / RTM_RATIO);
	b2Body* body = world->CreateBody(&bodydef);
	b2CircleShape circle;
	circle.m_radius = radius / RTM_RATIO;
	body->CreateFixture(&circle, 0.5);

	BodyData * bodydata = new BodyData();
	bodydata->tag = world->GetBodyCount() - 1;
	if (!deleteBodysTag.empty()) {
		auto end = deleteBodysTag.end();  // end是终止符号，没有对应元素
		end--;
		bodydata->tag = *(end);
		deleteBodysTag.pop_back();
	}
	bodydata->name = "body" + num2str(bodydata->tag);
	body->SetUserData(bodydata);
}

void Box2DDesignerScene::addChainLoop(std::vector<Vec2>* points){
	const int count = points->size();
	if (count < 2) return;

	b2BodyDef bodydef;
	b2Body* body = world->CreateBody(&bodydef);
	b2Vec2 *v = new b2Vec2[count];
	for (int i = 0; i < count; i++){ v[i].Set(points->at(i).x / RTM_RATIO, points->at(i).y/ RTM_RATIO); }
	b2ChainShape* chain = new b2ChainShape();	// 动态的,,即使设成bullet,也不会与b2EdgeShape 相碰撞，一般用来写静态边界, polygon代替他写动态物体
	//chain->CreateChain(v, count);
	chain->CreateLoop(v, count);
	b2FixtureDef fixturedef;
	fixturedef.shape = chain;
	body->CreateFixture(&fixturedef);


	BodyData * bodydata = new BodyData();
	bodydata->ischainloop = true;
	bodydata->tag = world->GetBodyCount() - 1;
	if (!deleteBodysTag.empty()) {
		auto end = deleteBodysTag.end();  // end是终止符号，没有对应元素
		end--;
		bodydata->tag = *(end);
		deleteBodysTag.pop_back();
	}
	bodydata->name = "body" + num2str(bodydata->tag);
	body->SetUserData(bodydata);
}

void Box2DDesignerScene::addChainEdge(std::vector<Vec2>* points){
	const int count = points->size();
	if (count < 2) return;

	b2BodyDef bodydef;
	b2Body* body = world->CreateBody(&bodydef);
	b2Vec2 *v = new b2Vec2[count];
	for (int i = 0; i < count; i++){ v[i].Set(points->at(i).x / RTM_RATIO, points->at(i).y/ RTM_RATIO); }
	b2ChainShape* chain = new b2ChainShape();	// 动态的,,即使设成bullet,也不会与b2EdgeShape 相碰撞，一般用来写静态边界, polygon代替他写动态物体
	chain->CreateChain(v, count);
	b2FixtureDef fixturedef;
	fixturedef.shape = chain;
	body->CreateFixture(&fixturedef);

	BodyData * bodydata = new BodyData();
	bodydata->tag = world->GetBodyCount() - 1;
	if (!deleteBodysTag.empty()) {
		auto end = deleteBodysTag.end();  // end是终止符号，没有对应元素
		end--;
		bodydata->tag = *(end);
		deleteBodysTag.pop_back();
	}
	bodydata->name = "body" + num2str(bodydata->tag);
	body->SetUserData(bodydata);
}

void Box2DDesignerScene::addPolygon(std::vector<Vec2>* points){
	const int count = points->size();
	if(count < 3) return;

	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	b2Body* body = world->CreateBody(&bodydef);

	b2PolygonShape polygon;
	b2Vec2* v = new b2Vec2[count];
	for (int i = 0; i < count; i++){ v[i].Set(points->at(i).x / RTM_RATIO, points->at(i).y/ RTM_RATIO); }
	polygon.Set(v, count);
	body->CreateFixture(&polygon, 0.5);

	BodyData * bodydata = new BodyData();
	bodydata->tag = world->GetBodyCount() - 1;
	if (!deleteBodysTag.empty()) {
		auto end = deleteBodysTag.end();  // end是终止符号，没有对应元素
		end--;
		bodydata->tag = *(end);
		deleteBodysTag.pop_back();
	}
	bodydata->name = "body" + num2str(bodydata->tag);
	body->SetUserData(bodydata);
}

/* XML 通用数据读取*/
void Box2DDesignerScene::readDataToXMLInAnyModel(Layer* layer, b2World* world, std::string filepath, bool setLayer){
	if (filepath == ""){
		if (defaultReadFilePath == "")
			filepath = FileUtils::getInstance()->getWritablePath() + "bos2ddesigner.xml";
		else
			filepath = defaultReadFilePath;
	}

	try{
		tinyxml2::XMLDocument* pDoc = new tinyxml2::XMLDocument();
		tinyxml2::XMLError errorId = pDoc->LoadFile(filepath.c_str());
		if (errorId != 0) {
			//xml格式错误
			cocos2d::log("xml read error");
			MessageBox(("cannot read file in " + filepath).c_str(), "Error XML File");
			return;
		}
		tinyxml2::XMLElement* designerElement = pDoc->RootElement();
		std::string model = designerElement->Attribute("model");
		if (model == "simple"){
			readDataFromXML(layer, world, filepath, setLayer);
		}
		else if (model == "complete"){
			readCompleteDataFromXML(layer, world, filepath, setLayer);
		}
		else{
			MessageBox(("model " + model + " is valid").c_str(), "Error XML File");
		}
	}
	catch (std::exception e){
		MessageBox(("xml read filed in " + filepath).c_str(), "Error XML File");
	}
}

/* XML 精简数据存储 */

void Box2DDesignerScene::saveDataToXML(std::string filepath){  // 子节点可以重名，attribute不能重名,attributute可以不是string
	if (filepath == "") filepath = FileUtils::getInstance()->getWritablePath() + "bos2ddesigner.xml";
	saving = true;

	if (pDoc) delete pDoc;
	pDoc = new tinyxml2::XMLDocument();
	// xml 声明（参数可选）
	tinyxml2::XMLDeclaration *pDel = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
	pDoc->LinkEndChild(pDel);

	//创建根节点
	tinyxml2::XMLElement* designerElement = pDoc->NewElement("box2ddesignerdata");
	pDoc->LinkEndChild( designerElement);
	designerElement->SetAttribute("model", "simple");

	// 添加layer元素以及他的属性（子元素） 到根
	addLayerDataToXML(designerElement);

	// 添加bodys元素 到根
	tinyxml2::XMLElement *bodylistElement = pDoc->NewElement("bodylist");
	designerElement->LinkEndChild(bodylistElement);

	// 添加body元素
	for (b2Body* body = world->GetBodyList(); body; body = body->GetNext()){
		tinyxml2::XMLElement* bodyElement = pDoc->NewElement("body");
		bodylistElement->LinkEndChild(bodyElement);

		BodyData* bodydata = (BodyData*)(body->GetUserData());
		bodyElement->SetAttribute("name", bodydata->name.c_str());

		/* 添加bodydef信息 */
		addBodyDefToXML(body, bodyElement);

		/* 添加fixturedef信息 */
		auto fixturelistElement = pDoc->NewElement("fixturelist");
		bodyElement->LinkEndChild (fixturelistElement);
		for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()){
			addFxitureDefToXML(fixture, fixturelistElement,body);
		}

		/* 添加userdata信息 */
		addUserDataToXML(body, bodyElement);
	}

	pDoc->SaveFile(filepath.c_str());
	pDoc->Print();
	//pDoc->SaveFile(filepath.c_str());
#ifndef NODEBUG
	
#endif
	cocos2d::log("%s",filepath.c_str());

	delete pDoc;
	pDoc = nullptr;
	saving = false;
	changed = false;
	framecount = 0;
}

void Box2DDesignerScene::addLayerDataToXML(tinyxml2::XMLElement* designerElement){
	// 添加layer元素以及他的属性（子元素） 到根
	tinyxml2::XMLElement *layerElement = pDoc->NewElement("layer");
	designerElement->LinkEndChild(layerElement);
	/*position*/
	tinyxml2::XMLElement *layerPosition = pDoc->NewElement("position");
	layerElement->LinkEndChild(layerPosition);
	layerPosition->SetAttribute("x", getPosition().x);
	layerPosition->SetAttribute("y", getPosition().y);
	/*scale*/
	layerElement->SetAttribute("scale", getScale());
}

void Box2DDesignerScene::addBodyDefToXML(b2Body* body, tinyxml2::XMLElement* bodyElement){
	tinyxml2::XMLElement* bodydefElement = pDoc->NewElement("bodydef");
	bodyElement->LinkEndChild(bodydefElement);

	tinyxml2::XMLElement* position = pDoc->NewElement("position");
	bodydefElement->LinkEndChild(position);
	position->SetAttribute("x", body->GetPosition().x);
	position->SetAttribute("y", body->GetPosition().y);

	bodydefElement->SetAttribute("type", body->GetType());
}

void Box2DDesignerScene::addFxitureDefToXML(b2Fixture* fixture, tinyxml2::XMLElement* bodyElement,b2Body* body){
	tinyxml2::XMLElement* fixturedefElement = pDoc->NewElement("fixturedef");
	bodyElement->LinkEndChild(fixturedefElement);

	tinyxml2::XMLElement* shapedefElement = pDoc->NewElement("shapedef");
	fixturedefElement->LinkEndChild(shapedefElement);
	shapedefElement->SetAttribute("type", fixture->GetType());
	if (fixture->GetType() == b2Shape::e_edge){
		b2EdgeShape* shape = (b2EdgeShape*)(fixture->GetShape());

		tinyxml2::XMLElement* verticesarrayElement = pDoc->NewElement("verticearray");
		shapedefElement->LinkEndChild(verticesarrayElement);

		tinyxml2::XMLElement* vertice1Element = pDoc->NewElement("vertice");
		verticesarrayElement->LinkEndChild(vertice1Element);
		vertice1Element->SetAttribute("x", shape->m_vertex1.x);
		vertice1Element->SetAttribute("y", shape->m_vertex1.y);

		tinyxml2::XMLElement* vertice2Element = pDoc->NewElement("vertice");
		verticesarrayElement->LinkEndChild(vertice2Element);
		vertice2Element->SetAttribute("x", shape->m_vertex2.x);
		vertice2Element->SetAttribute("y", shape->m_vertex2.y);
	}
	else if (fixture->GetType() == b2Shape::e_circle){
		b2CircleShape* shape = (b2CircleShape*)(fixture->GetShape());
		shapedefElement->SetAttribute("radius", shape->m_radius);

		tinyxml2::XMLElement* centerElement = pDoc->NewElement("center");
		shapedefElement->LinkEndChild(centerElement);
		centerElement->SetAttribute("x", shape->m_p.x);
		centerElement->SetAttribute("y", shape->m_p.y);
	}
	else if (fixture->GetType() == b2Shape::e_chain){
		b2ChainShape* shape = (b2ChainShape*)(fixture->GetShape());

		tinyxml2::XMLElement* verticesarrayElement = pDoc->NewElement("verticearray");
		shapedefElement->LinkEndChild(verticesarrayElement);

		BodyData* bodydata = (BodyData*)(body->GetUserData());
		shapedefElement->SetAttribute("ischainloop", bodydata->ischainloop);

		int count = shape->m_count;
		if (bodydata->ischainloop) count--; // Loop 会把起始点作为最后一个点，比edge多一个点, 但create时的两个点的间距不能低于0.05（物理世界大小）

		verticesarrayElement->SetAttribute("count", count);
		
		for (int i = 0; i < count; i++){
			tinyxml2::XMLElement* verticeElement = pDoc->NewElement("vertice");
			verticesarrayElement->LinkEndChild(verticeElement);
			verticeElement->SetAttribute("x", shape->m_vertices[i].x);
			verticeElement->SetAttribute("y", shape->m_vertices[i].y);
		}
	}
	else if (fixture->GetType() == b2Shape::e_polygon){
		b2PolygonShape* shape = (b2PolygonShape*)(fixture->GetShape());

		tinyxml2::XMLElement* verticesarrayElement = pDoc->NewElement("verticearray");
		shapedefElement->LinkEndChild(verticesarrayElement);

		verticesarrayElement->SetAttribute("count", shape->m_count);

		for (int i = 0; i < shape->m_count; i++){
			tinyxml2::XMLElement* verticeElement = pDoc->NewElement("vertice");
			verticesarrayElement->LinkEndChild(verticeElement);
			verticeElement->SetAttribute("x", shape->m_vertices[i].x);
			verticeElement->SetAttribute("y", shape->m_vertices[i].y);
		}
	}
}

void Box2DDesignerScene::addUserDataToXML(b2Body* body, tinyxml2::XMLElement* bodyElement) {
	tinyxml2::XMLElement* userdataElement = pDoc->NewElement("userdata");
	bodyElement->LinkEndChild(userdataElement);
	BodyData* bodydata = (BodyData*)(body->GetUserData());
}

/* XML 精简数据读取， 这里都是静态函数 */

void Box2DDesignerScene::readDataFromXML(Layer* layer, b2World* world, std::string filepath, bool setLayer){
	if (filepath == ""){
		if (defaultReadFilePath == "")
			filepath = FileUtils::getInstance()->getWritablePath() + "bos2ddesigner.xml";
		else
			filepath = defaultReadFilePath;
	}

	tinyxml2::XMLDocument* pDoc = new tinyxml2::XMLDocument();
	tinyxml2::XMLError errorId = pDoc->LoadFile(filepath.c_str());
	if (errorId != 0) {
		//xml格式错误
		cocos2d::log("xml read error");
		return;
	}

	/* 创建一个新的BodyMap，用于由name查找body */
	BodyMap* bodymap = new BodyMap();
	worlds->insert(std::pair<b2World*, BodyMap*>(world, bodymap));

	tinyxml2::XMLElement* designerElement = pDoc->RootElement();
	tinyxml2::XMLElement* layerElement = designerElement->FirstChildElement();
	if (setLayer){
		readLayerDataFromXML(layer, layerElement);
	}
	
	auto bodylistE = layerElement->NextSibling();
	b2BodyDef bodydef;
	for (auto bodyE = bodylistE->FirstChildElement(); bodyE; bodyE = bodyE->NextSiblingElement()){
		/* name */
		std::string bodyname(bodyE->FirstAttribute()->Value());

		/* bodydef */
		auto bodydefE = bodyE->FirstChildElement();
		readBodyDefFromXML(bodydef, bodydefE);

		b2Body* body = world->CreateBody(&bodydef);
		bodymap->insert(std::pair<std::string, b2Body*>(bodyname, body));

		/* fixture */
		auto fixturelistE = bodydefE->NextSiblingElement();
		for (auto fixturedefE = fixturelistE->FirstChildElement(); fixturedefE; fixturedefE = fixturedefE->NextSiblingElement()){
			auto shapedefE = fixturedefE->FirstChildElement();
			b2FixtureDef fixturedef;

			b2Shape* shape=nullptr;

			readShapeFromXML(shape, shapedefE);
			
			if (shape) {
				fixturedef.shape = shape;
				body->CreateFixture(&fixturedef);
			}
		}

		/* userdata */
		auto userdataE = fixturelistE->NextSiblingElement();
		BodyData* bodydata = new BodyData();
		readUserDataFromXML(bodydata, userdataE);
		body->SetUserData(bodydata);

	}
	delete pDoc;
}

void Box2DDesignerScene::readLayerDataFromXML(Layer* layer, tinyxml2::XMLElement* layerElement){
	tinyxml2::XMLElement* positionElement = layerElement->FirstChildElement();
	auto positionXA = positionElement->FirstAttribute();
	Vec2 position(positionXA->FloatValue(), positionXA->Next()->FloatValue());
	float scale = layerElement->FirstAttribute()->FloatValue();
	layer->setScale(scale);
	layer->setPosition(position);
}

void Box2DDesignerScene::readBodyDefFromXML(b2BodyDef& bodydef, tinyxml2::XMLElement* bodydefE){
	auto bodypositionE = bodydefE->FirstChildElement();
	auto bodypositionXA = bodypositionE->FirstAttribute();
	bodydef.position.Set(bodypositionXA->FloatValue(), bodypositionXA->Next()->FloatValue());

	auto bodytypeA = bodydefE->FirstAttribute();
	bodydef.type = (b2BodyType)(bodytypeA->IntValue());
}

void Box2DDesignerScene::readShapeFromXML(b2Shape*& shape, tinyxml2::XMLElement* shapedefE){
	auto shapetypeA = shapedefE->FirstAttribute();
	b2Shape::Type shapetype = (b2Shape::Type)(shapetypeA->IntValue());

	if (shapetype == b2Shape::e_circle){
		shape = new b2CircleShape();

		auto centerE = shapedefE->FirstChildElement();
		auto centerXA = centerE->FirstAttribute();
		b2Vec2 center(centerXA->FloatValue(), centerXA->Next()->FloatValue());
		((b2CircleShape*)shape)->m_p = center;

		auto radiusA = shapetypeA->Next();
		((b2CircleShape*)shape)->m_radius = radiusA->FloatValue();
	}
	else if (shapetype == b2Shape::e_edge){
		shape = new b2EdgeShape();

		auto verticelistE = shapedefE->FirstChildElement();
		auto vertice1E = verticelistE->FirstChildElement();
		auto vertice1XA = vertice1E->FirstAttribute();
		auto vertice2E = vertice1E->NextSiblingElement();
		auto vertice2XA = vertice2E->FirstAttribute();

		b2Vec2 vertice1(vertice1XA->FloatValue(), vertice1XA->Next()->FloatValue());
		b2Vec2 vertice2(vertice2XA->FloatValue(), vertice2XA->Next()->FloatValue());
		((b2EdgeShape*)shape)->Set(vertice1, vertice2);
	}
	else if (shapetype == b2Shape::e_chain){
		shape = new b2ChainShape();

		auto verticelistE = shapedefE->FirstChildElement();
		auto countA = verticelistE->FirstAttribute();
		const int count = countA->IntValue();
		b2Vec2* vertices = new b2Vec2[count];
	
		int i = 0;
		for (auto verticeE = verticelistE->FirstChildElement(); verticeE; verticeE = verticeE->NextSiblingElement()){
			auto verticeXA = verticeE->FirstAttribute();
			vertices[i] = b2Vec2(verticeXA->FloatValue(), verticeXA->Next()->FloatValue());
			i++;
			//cocos2d::log("vertices : %f %f" , vertices[i].x, vertices[i].y);
		}

		bool isloop = shapetypeA->Next()->BoolValue();

		if (!isloop) ((b2ChainShape*)(shape))->CreateChain(vertices, count);
		else {((b2ChainShape*)(shape))->CreateLoop(vertices, count);
		//cocos2d::log("loop");
		}
		//cocos2d::log("*************************************************");
	}
	else if (shapetype == b2Shape::e_polygon){
		shape = new b2PolygonShape();

		auto verticelistE = shapedefE->FirstChildElement();
		auto countA = verticelistE->FirstAttribute();
		const int count = countA->IntValue();
		b2Vec2* vertices = new b2Vec2[count];

		int i = 0;
		for (auto verticeE = verticelistE->FirstChildElement(); verticeE; verticeE = verticeE->NextSiblingElement()){
			auto verticeXA = verticeE->FirstAttribute();
			vertices[i] = b2Vec2(verticeXA->FloatValue(), verticeXA->Next()->FloatValue());
			i++;
		}

		((b2PolygonShape*)(shape))->Set(vertices, count);
	}
}

void Box2DDesignerScene::readUserDataFromXML(BodyData* bodydata, tinyxml2::XMLElement* userdataE){

}

/* XML 完全数据存储 */

void Box2DDesignerScene::saveCompleteDataToXML(std::string filepath){
	if (filepath == "") filepath = FileUtils::getInstance()->getWritablePath() + "bos2ddesigner.xml";
	saving = true;

	if (pDoc) delete pDoc;
	pDoc = new tinyxml2::XMLDocument();
	// xml 声明（参数可选）
	tinyxml2::XMLDeclaration *pDel = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
	pDoc->LinkEndChild(pDel);

	//创建根节点
	tinyxml2::XMLElement* designerElement = pDoc->NewElement("box2ddesignerdata");
	pDoc->LinkEndChild(designerElement);
	designerElement->SetAttribute("model","complete");

	// 添加layer元素以及他的属性（子元素） 到根
	addLayerDataToXML(designerElement);

	// 添加bodys元素 到根
	tinyxml2::XMLElement *bodylistElement = pDoc->NewElement("bodylist");
	designerElement->LinkEndChild(bodylistElement);

	// 添加body元素
	for (b2Body* body = world->GetBodyList(); body; body = body->GetNext()){
		tinyxml2::XMLElement* bodyElement = pDoc->NewElement("body");
		bodylistElement->LinkEndChild(bodyElement);

		BodyData* bodydata = (BodyData*)(body->GetUserData());
		bodyElement->SetAttribute("name", bodydata->name.c_str());

		/* 添加bodydef信息 */
		addCompleteBodyDefToXML(body, bodyElement);

		/* 添加fixturedef信息 */
		auto fixturelistElement = pDoc->NewElement("fixturelist");
		bodyElement->LinkEndChild(fixturelistElement);
		for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()){
			addCompleteFxitureDefToXML(fixture, fixturelistElement, body);
		}

		/* 添加userdata信息 */
		addUserDataToXML(body, bodyElement);
	}

	pDoc->SaveFile(filepath.c_str());
	pDoc->Print();
	//pDoc->SaveFile(filepath.c_str());
#ifndef NODEBUG

#endif
	cocos2d::log("%s",filepath.c_str());

	delete pDoc;
	pDoc = nullptr;
	saving = false;
	changed = false;
	framecount = 0;
}

void Box2DDesignerScene::addCompleteBodyDefToXML(b2Body* body, tinyxml2::XMLElement* bodyElement){
	tinyxml2::XMLElement* bodydefElement = pDoc->NewElement("bodydef");
	bodyElement->LinkEndChild(bodydefElement);

	tinyxml2::XMLElement* position = pDoc->NewElement("position");
	bodydefElement->LinkEndChild(position);
	position->SetAttribute("x", body->GetPosition().x);
	position->SetAttribute("y", body->GetPosition().y);

	bodydefElement->SetAttribute("type", body->GetType());
	bodydefElement->SetAttribute("angle", body->GetAngle());
	bodydefElement->SetAttribute("angulardumping", body->GetAngularDamping());
	bodydefElement->SetAttribute("angularvelocity", body->GetAngularVelocity());
	bodydefElement->SetAttribute("lineardumping", body->GetLinearDamping());

	auto linearVelocityElement = pDoc->NewElement("linearvelocity");
	bodydefElement->LinkEndChild(linearVelocityElement);
	linearVelocityElement->SetAttribute("x", body->GetLinearVelocity().x);
	linearVelocityElement->SetAttribute("y", body->GetLinearVelocity().y);

	bodydefElement->SetAttribute("gravityscale", body->GetGravityScale());

	b2MassData* massdata = new b2MassData();
	body->GetMassData(massdata);
	auto massdataElement = pDoc->NewElement("massdata");
	bodydefElement->LinkEndChild(massdataElement);
	auto masscenterElement = pDoc->NewElement("center");			// 质心
	massdataElement->LinkEndChild(masscenterElement);
	masscenterElement->SetAttribute("x", massdata->center.x);
	masscenterElement->SetAttribute("y", massdata->center.y);
	massdataElement->SetAttribute("I", massdata->I);          // 转动惯量
	massdataElement->SetAttribute("mass", massdata->mass);
	if(massdata) delete massdata;

	bodydefElement->SetAttribute("issleepingallowed", body->IsSleepingAllowed());
	bodydefElement->SetAttribute("isactive", body->IsActive());
	bodydefElement->SetAttribute("isawake", body->IsAwake());
	bodydefElement->SetAttribute("isbullet", body->IsBullet());
	bodydefElement->SetAttribute("isfixedrotation", body->IsFixedRotation());
}

void Box2DDesignerScene::addCompleteFxitureDefToXML(b2Fixture* fixture, tinyxml2::XMLElement* bodyElement, b2Body* body){
	tinyxml2::XMLElement* fixturedefElement = pDoc->NewElement("fixturedef");
	bodyElement->LinkEndChild(fixturedefElement);

	tinyxml2::XMLElement* shapedefElement = pDoc->NewElement("shapedef");
	fixturedefElement->LinkEndChild(shapedefElement);
	shapedefElement->SetAttribute("type", fixture->GetType());
	if (fixture->GetType() == b2Shape::e_edge){
		b2EdgeShape* shape = (b2EdgeShape*)(fixture->GetShape());

		tinyxml2::XMLElement* verticesarrayElement = pDoc->NewElement("verticearray");
		shapedefElement->LinkEndChild(verticesarrayElement);

		tinyxml2::XMLElement* vertice1Element = pDoc->NewElement("vertice");
		verticesarrayElement->LinkEndChild(vertice1Element);
		vertice1Element->SetAttribute("x", shape->m_vertex1.x);
		vertice1Element->SetAttribute("y", shape->m_vertex1.y);

		tinyxml2::XMLElement* vertice2Element = pDoc->NewElement("vertice");
		verticesarrayElement->LinkEndChild(vertice2Element);
		vertice2Element->SetAttribute("x", shape->m_vertex2.x);
		vertice2Element->SetAttribute("y", shape->m_vertex2.y);
	}
	else if (fixture->GetType() == b2Shape::e_circle){
		b2CircleShape* shape = (b2CircleShape*)(fixture->GetShape());
		shapedefElement->SetAttribute("radius", shape->m_radius);

		tinyxml2::XMLElement* centerElement = pDoc->NewElement("center");
		shapedefElement->LinkEndChild(centerElement);
		centerElement->SetAttribute("x", shape->m_p.x);
		centerElement->SetAttribute("y", shape->m_p.y);
	}
	else if (fixture->GetType() == b2Shape::e_chain){
		b2ChainShape* shape = (b2ChainShape*)(fixture->GetShape());

		tinyxml2::XMLElement* verticesarrayElement = pDoc->NewElement("verticearray");
		shapedefElement->LinkEndChild(verticesarrayElement);

		BodyData* bodydata = (BodyData*)(body->GetUserData());
		shapedefElement->SetAttribute("ischainloop", bodydata->ischainloop);

		int count = shape->m_count;
		if (bodydata->ischainloop) count--; // Loop 会把起始点作为最后一个点，比edge多一个点, 但create时的两个点的间距不能低于0.05（物理世界大小）

		verticesarrayElement->SetAttribute("count", count);

		for (int i = 0; i < count; i++){
			tinyxml2::XMLElement* verticeElement = pDoc->NewElement("vertice");
			verticesarrayElement->LinkEndChild(verticeElement);
			verticeElement->SetAttribute("x", shape->m_vertices[i].x);
			verticeElement->SetAttribute("y", shape->m_vertices[i].y);
		}
	}
	else if (fixture->GetType() == b2Shape::e_polygon){
		b2PolygonShape* shape = (b2PolygonShape*)(fixture->GetShape());

		tinyxml2::XMLElement* verticesarrayElement = pDoc->NewElement("verticearray");
		shapedefElement->LinkEndChild(verticesarrayElement);

		verticesarrayElement->SetAttribute("count", shape->m_count);

		for (int i = 0; i < shape->m_count; i++){
			tinyxml2::XMLElement* verticeElement = pDoc->NewElement("vertice");
			verticesarrayElement->LinkEndChild(verticeElement);
			verticeElement->SetAttribute("x", shape->m_vertices[i].x);
			verticeElement->SetAttribute("y", shape->m_vertices[i].y);
		}
	}

	auto filterElement = pDoc->NewElement("filter");
	fixturedefElement->LinkEndChild(filterElement);
	filterElement->SetAttribute("categorybits", fixture->GetFilterData().categoryBits);
	filterElement->SetAttribute("groundindex", fixture->GetFilterData().groupIndex);
	filterElement->SetAttribute("maskbits", fixture->GetFilterData().maskBits);

	fixturedefElement->SetAttribute("density", fixture->GetDensity());
	fixturedefElement->SetAttribute("friction", fixture->GetFriction());
	fixturedefElement->SetAttribute("restitutuion", fixture->GetRestitution());
	fixturedefElement->SetAttribute("isSensor", fixture->IsSensor());
}

/* XML 完全数据读取 */

void Box2DDesignerScene::readCompleteDataFromXML(Layer* layer, b2World* world, std::string filepath, bool setLayer){
	if (filepath == ""){
		if (defaultReadFilePath == "")
			filepath = FileUtils::getInstance()->getWritablePath() + "bos2ddesigner.xml";
		else
			filepath = defaultReadFilePath;
	}

	tinyxml2::XMLDocument* pDoc = new tinyxml2::XMLDocument();
	tinyxml2::XMLError errorId = pDoc->LoadFile(filepath.c_str());
	if (errorId != 0) {
		//xml格式错误
		cocos2d::log("xml read error");
		return;
	}

	/* 创建一个新的BodyMap，用于由name查找body */
	BodyMap* bodymap = new BodyMap();
	worlds->insert(std::pair<b2World*, BodyMap*>(world, bodymap));

	tinyxml2::XMLElement* designerElement = pDoc->RootElement();
	tinyxml2::XMLElement* layerElement = designerElement->FirstChildElement();
	if (setLayer){
		readLayerDataFromXML(layer, layerElement);
	}

	auto bodylistE = layerElement->NextSibling();
	b2BodyDef bodydef;
	for (auto bodyE = bodylistE->FirstChildElement(); bodyE; bodyE = bodyE->NextSiblingElement()){
		/* name */
		std::string bodyname(bodyE->FirstAttribute()->Value());

		/* bodydef */
		auto bodydefE = bodyE->FirstChildElement();
		auto massdata = readCompleteBodyDefFromXML(bodydef, bodydefE);

		b2Body* body = world->CreateBody(&bodydef);
		bodymap->insert(std::pair<std::string, b2Body*>(bodyname, body));
		body->SetMassData(massdata);
		delete massdata;

		/* fixture */
		auto fixturelistE = bodydefE->NextSiblingElement();
		for (auto fixturedefE = fixturelistE->FirstChildElement(); fixturedefE; fixturedefE = fixturedefE->NextSiblingElement()){
			auto shapedefE = fixturedefE->FirstChildElement();
			b2FixtureDef fixturedef;

			/* shape */
			b2Shape* shape = nullptr;

			readShapeFromXML(shape, shapedefE);

			/*  shape 之外的fixture */
			readFixtureNoShapeFromXML(fixturedef, fixturedefE);

			if (shape) {
				fixturedef.shape = shape;
				body->CreateFixture(&fixturedef);
			}
		}

		/* userdata */
		auto userdataE = fixturelistE->NextSiblingElement();
		BodyData* bodydata = new BodyData();
		readUserDataFromXML(bodydata, userdataE);
		body->SetUserData(bodydata);
		cocos2d::log("%f", body->GetFixtureList()->GetDensity());
	}
	
	delete pDoc;
}

b2MassData* Box2DDesignerScene::readCompleteBodyDefFromXML(b2BodyDef& bodydef, tinyxml2::XMLElement* bodydefE){
	auto bodydefSonE = bodydefE->FirstChildElement();
	auto bodypositionXA = bodydefSonE->FirstAttribute();
	bodydef.position.Set(bodypositionXA->FloatValue(), bodypositionXA->Next()->FloatValue());

	auto bodydefSonA = bodydefE->FirstAttribute();
	bodydef.type = (b2BodyType)(bodydefSonA->IntValue());
	bodydefSonA = bodydefSonA->Next();
	bodydef.angle = bodydefSonA->FloatValue();
	bodydefSonA = bodydefSonA->Next();
	bodydef.angularDamping = bodydefSonA->FloatValue();
	bodydefSonA = bodydefSonA->Next();
	bodydef.angularVelocity = bodydefSonA->FloatValue();
	bodydefSonA = bodydefSonA->Next();
	bodydef.linearDamping = bodydefSonA->FloatValue();

	bodydefSonE = bodydefSonE->NextSiblingElement();		//
	auto linearvecilotyXA = bodydefSonE->FirstAttribute();
	bodydef.linearVelocity.Set(linearvecilotyXA->FloatValue(), linearvecilotyXA->Next()->FloatValue());

	bodydefSonA = bodydefSonA->Next();
	bodydef.gravityScale = bodydefSonA->FloatValue();

	b2MassData* massdata = new b2MassData();
	bodydefSonE = bodydefSonE->NextSiblingElement();
	auto masscenterE = bodydefSonE->FirstChildElement();
	auto masscenterXA = masscenterE->FirstAttribute();
	massdata->center.Set(masscenterXA->FloatValue(), masscenterXA->Next()->FloatValue());
	auto IA = bodydefSonE->FirstAttribute();
	massdata->I = IA->FloatValue();
	massdata->mass = IA->Next()->FloatValue();

	bodydefSonA = bodydefSonA->Next();
	bodydef.allowSleep = bodydefSonA->BoolValue();
	bodydefSonA = bodydefSonA->Next();
	bodydef.active = bodydefSonA->BoolValue();
	bodydefSonA = bodydefSonA->Next();
	bodydef.awake = bodydefSonA->BoolValue();
	bodydefSonA = bodydefSonA->Next();
	bodydef.bullet = bodydefSonA->BoolValue();
	bodydefSonA = bodydefSonA->Next();
	bodydef.fixedRotation = bodydefSonA->BoolValue();

	return massdata;
}

void Box2DDesignerScene::readFixtureNoShapeFromXML(b2FixtureDef& fixturedef, tinyxml2::XMLElement* fixturedefE){
	auto filterE = fixturedefE->FirstChildElement()->NextSiblingElement();
	auto categorybitsA = filterE->FirstAttribute();
	fixturedef.filter.categoryBits = categorybitsA->IntValue();
	auto groundindexA = categorybitsA->Next();
	fixturedef.filter.groupIndex = groundindexA->IntValue();
	auto maskbitsA = groundindexA->Next();
	fixturedef.filter.maskBits = maskbitsA->IntValue();

	auto densityA = fixturedefE->FirstAttribute();
	fixturedef.density = densityA->FloatValue();
	auto frictionA = densityA->Next();
	fixturedef.friction = frictionA->FloatValue();
	auto restitutionA = frictionA->Next();
	fixturedef.restitution = restitutionA->FloatValue();
	auto issensorA = restitutionA->Next();
	fixturedef.isSensor = issensorA->BoolValue();
}

/* 通过名字获取body*/

std::map<b2World*, BodyMap*> * Box2DDesignerScene::worlds = new std::map<b2World*, BodyMap*>();

b2Body* Box2DDesignerScene::getBodyByName(std::string bodyname, b2World* world){
	try{
		if (worlds && !worlds->empty() && world){
			BodyMap* bodymap = worlds->at(world);
			/*for (auto b = bodymap->begin(); b != bodymap->end(); b++){
				cocos2d::log("%s, %p", b->first.c_str(), b->second);
			}*/
			if (bodymap && !bodymap->empty()) return bodymap->at(bodyname); // 越界会崩
		}
	} catch (std::exception e){
		return nullptr;
	}
	
	return nullptr;
}

void Box2DDesignerScene::printAllDataOfBody(b2Body* body){
	if (body){
		cocos2d::log("**********************************************************");
		cocos2d::log("type : %d",body->GetType());
		cocos2d::log("position : %f, %f", body->GetPosition().x, body->GetPosition().y);
		cocos2d::log("angle : %f", body->GetAngle());
		cocos2d::log("angularDumping : %f", body->GetAngularDamping());
		cocos2d::log("angularVelocity : %f", body->GetAngularVelocity());
		cocos2d::log("linearDumping : %f", body->GetLinearDamping());
		cocos2d::log("linearVelocity : %f, %f", body->GetLinearVelocity().x, body->GetLinearVelocity().y);
		cocos2d::log("gravityScale : %f", body->GetGravityScale());

		b2MassData* massdata = new b2MassData();
		body->GetMassData(massdata);
		cocos2d::log("massCenter : %f, %f", massdata->center.x, massdata->center.y);
		cocos2d::log("inertia : %f", massdata->I);
		cocos2d::log("mass : %f", massdata->mass);

		delete massdata;

		cocos2d::log("IsSleepingAllowed : %d", int(body->IsSleepingAllowed()));
		cocos2d::log("IsActive : %d", int(body->IsActive()));
		cocos2d::log("IsAwake : %d", int(body->IsAwake()));
		cocos2d::log("IsBullet : %d", int(body->IsBullet()));
		cocos2d::log("IsFixedRotation : %d", int(body->IsFixedRotation()));
		
		for (auto fixture = body->GetFixtureList(); fixture;fixture=fixture->GetNext()){
			auto shape = fixture->GetShape();

			cocos2d::log("Density : %f", fixture->GetDensity());
			cocos2d::log("Friction : %f", fixture->GetFriction());
			cocos2d::log("Restitution : %f", fixture->GetRestitution());
			cocos2d::log("IsSensor : %d", fixture->IsSensor());
		}
		cocos2d::log("**********************************************************");
	}
}

/* xml test*/

void Box2DDesignerScene::xmlstoretest(){
	std::string filePath = FileUtils::getInstance()->getWritablePath() + "test.xml"; 
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	//xml 声明（参数可选）
	tinyxml2::XMLDeclaration *pDel = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
	pDoc->LinkEndChild(pDel);
	//添加plist节点
	tinyxml2::XMLElement *plistElement = pDoc->NewElement("plist");
	plistElement->SetAttribute("version", "1.0");
	plistElement->SetAttribute("version", "2.0");
	pDoc->LinkEndChild(plistElement);
	tinyxml2::XMLComment *commentElement = pDoc->NewComment("this is xml comment");
	plistElement->LinkEndChild(commentElement);
	//添加dic节点
	tinyxml2::XMLElement *dicElement = pDoc->NewElement("dic");
	plistElement->LinkEndChild(dicElement);
	//添加key节点
	tinyxml2::XMLElement *keyElement = pDoc->NewElement("key");
	keyElement->LinkEndChild(pDoc->NewText("Text"));
	dicElement->LinkEndChild(keyElement);
	tinyxml2::XMLElement *arrayElement = pDoc->NewElement("array");
	dicElement->LinkEndChild(arrayElement);
	for (int i = 0; i<3; i++) {
		tinyxml2::XMLElement *elm = pDoc->NewElement("name");
		elm->LinkEndChild(pDoc->NewText("Cocos2d-x"));
		arrayElement->LinkEndChild(elm);
	}
	pDoc->SaveFile(filePath.c_str());
	pDoc->Print();
	delete pDoc;
}

void Box2DDesignerScene::xmlreadtest(){

	std::string filePath = FileUtils::getInstance()->getWritablePath() + "test.xml";
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	tinyxml2::XMLError errorId = pDoc->LoadFile(filePath.c_str());
	if (errorId != 0) {
		//xml格式错误
		cocos2d::log("xml read error");
		return;
	}
	tinyxml2::XMLElement *rootEle = pDoc->RootElement();
	//获取第一个节点属性
	const tinyxml2::XMLAttribute *attribute = rootEle->FirstAttribute();
	//打印节点属性名和值
	log("attributename = %s,attributevalue = %s", attribute->Name(), attribute->Value());

	tinyxml2::XMLElement *dicEle = rootEle->FirstChildElement("dic");
	tinyxml2::XMLElement *keyEle = dicEle->FirstChildElement("key");
	if (keyEle) {
		log("keyEle Text= %s", keyEle->GetText());
	}
	tinyxml2::XMLElement *arrayEle = keyEle->NextSiblingElement();
	tinyxml2::XMLElement *childEle = arrayEle->FirstChildElement();
	while (childEle) {
		log("childEle Text= %s", childEle->GetText());
		childEle = childEle->NextSiblingElement();
	}
	delete pDoc;
	
}
