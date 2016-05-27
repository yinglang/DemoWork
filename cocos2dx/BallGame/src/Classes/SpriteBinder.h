#ifndef __SPRITEBINDER_H__
#define __SPRITEBINDER_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "GLES-Render.h"
#include "tinyxml2\tinyxml2.h"
#include "ui\CocosGUI.h"
#include "Box2DDesignerScene.h"

USING_NS_CC;
#define RTM_RATIO 32

struct SpriteData{
	std::string name;
	std::string imagepath;
	float scaleX;
	float scaleY;
	bool isFlipX;
	bool isFlipY;
	Vec2 position;
	float rotation;
	SpriteData(){
		name = "";
		imagepath = "";
		scaleX = 1.0;
		scaleY = 1.0;
		isFlipX = false;
		isFlipY = false;
		position = Vec2::ZERO;
		rotation = 0;
	}
};

class SpriteBinder : Layer, public cocos2d::ui::EditBoxDelegate
{
public:
	static Scene* createScene();
	virtual bool init();
	virtual void onEnter();
	CREATE_FUNC(SpriteBinder);
	virtual void update(float dt);
	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);

	//editbox 事件
	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
	virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
	virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox);

	Size visibleSize;
	ui::ListView* topListView;
	ui::ListView* rightListView;
	ui::EditBox* _editName;

	float uiRightWidth;
	float uiLayerHeight;
	float maxScale;
	float minScale;
	float scaleDelta;

	static std::string* spriteImagepath;
	void setting();
protected:
	/* 物理世界和绘制调试线，以及绘图的成员 */
	b2World* world;
	GLESDebugDraw* debugdraw;
	CustomCommand _customCmd;
	void initPhysics();
	void myDraw(const Mat4 &transform, uint32_t flags);

	void initTopUI();
	void initRightUI();
	void loadBackground();
	void loadSprite();
	void outRightListView();
	void backRightListView(Vec2 currentselectedpoint);
	bool hadOutightListView;

	void saveEvent(Ref* psender, ui::Widget::TouchEventType type);
	void copyEvent(Ref* psender, ui::Widget::TouchEventType type);
	void deleteEvent(Ref* psender, ui::Widget::TouchEventType type);
	void rotateEvent(Ref* psender, ui::Widget::TouchEventType type);
	void scaleEvent(Ref* psender, ui::Widget::TouchEventType type);
	void flipEvent(Ref* psender, ui::Widget::TouchEventType type);
	void gobackEvent(Ref* psender, ui::Widget::TouchEventType type);

	int spriteCount;
	int nameCount;

	void setEnableAllButton(bool enable);

	float angleDelta;
	Sprite* currentSelectedSprite;
	Sprite* getSelectedSprite(Vec2 currentselectedpoint);
	bool isSeletedSprite(Vec2 startpoint, Vec2 endpoint, Vec2 targetpoint, float rangeLength);
protected:
	bool onTouchBegan(Touch* touch, Event* event);		// 返回false事件不会向后传递
	void onTouchMoved(Touch* touch, Event* event);      // 点击着移动
	void onTouchEnded(Touch* touch, Event* event);

	Vec2 pointRotate(Vec2 point, float angle);			// point 按原点逆时针旋转 angle (弧度制)后的坐标

public:
	static std::vector<SpriteData>* readSpriteDataFromXML(std::string filename="");
	static std::map<std::string, Sprite*>* readSpriteFromXML(std::string filename = "", Layer* layer = nullptr, int zOrder = 1);

protected:
	bool hadmoved;
protected:
	void saveSpriteDataToXML();

	static std::string num2str(double num){
		std::stringstream ss;
		ss << num;
		return ss.str();
	}
};

#endif