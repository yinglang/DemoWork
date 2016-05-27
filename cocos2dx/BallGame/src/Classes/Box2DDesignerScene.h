#ifndef __BOX2DDESIGNERSCENE_H__
#define __BOX2DDESIGNERSCENE_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "GLES-Render.h"
#include "tinyxml2\tinyxml2.h"
#include "ui\CocosGUI.h"

USING_NS_CC;

#ifndef RTM_RATIO
#define RTM_RATIO 32
#endif

#define NODEBUG
#define DrawZOrder 2

enum ShapeType{ EDGE, CIRCLE, CHAIN, POLYGON, tyep_count };
#define Points std::vector<Vec2>
//使用std::map<const char*, b2Body*> // 使map查找崩溃
#define BodyMap std::map<std::string, b2Body*>

struct BodyData{
	bool ischainloop;
	std::string name;
	int tag;
	BodyData(){
		ischainloop = false;
	}
	
};  // 存储body信息到UserData

// fixture.filter
/*	一个物体可以有多个fixture 设置maskBits和groundIndex 的控件有待改进
	categoryBits 是种群号，必须是2的幂次，即二进制位只有一个为 1，做多可设16个种群
	maskBits     如果另一个fixture的cateroryBits和本fixture的maskBits 位与结果不为0， 则他们之间可以碰撞，否则永不碰撞，
	groundIndex  碰撞组好，同一个正碰撞组可以碰撞，同一负碰撞组永不碰撞，碰撞组优先级高于种群	

	在同一个碰撞组 或者
	(fixtureA.filter.category & fixtureB.filter.maskBits) && (fixtureB.filter.category & fixtureA.filter.maskBits)为true
	才会发生碰撞
*/

enum Attribute{ a_name, a_positionX, a_positionY, a_type, a_angle, 
	a_angularDumping, a_angularVelocity, a_linearDumping, a_linearVelocityX, a_linearVelocityY, 
	a_gravityScale, a_massCenterX, a_massCenterY, a_inertia, a_mass, 
	a_isAllowSleep, a_isActive, a_isAwake, a_isBullet, a_isFixedRotation,
	a_filter_categoryBits, a_filter_groundIndex, a_filter_maskBits, a_density, a_friction,
	a_restitution, a_isSensor, a_count};  // 其中的a_filter_groundIndex应是a_filter_grounpIndex

class UILayer : public Layer, public cocos2d::ui::EditBoxDelegate{
public:
	CREATE_FUNC(UILayer);
	virtual bool init();

	ShapeType shapetype;
	
	// 与designer界面交互的数据，在Box2DDesignerScene 的init中初始化
	float uiLayerHeight;
	float uiRightWidth;
	Layer* designer;	 // 要初始化为Box2DDesignerScene，因为cpp在一个文件中只能使用在它前面声明的类

	float outrange;				// 当鼠标距离边界小于该值时，listView才可能会out
	bool topListViewOut;		// 状态量，表示当前topListView是否出来了
	bool rightListViewOut;		// 状态量，表示当前rightListView是否出来了
	void OutTopListView();		// 使得topListView下拉
	void OutRightListView();	// 使得rightListView左拉
	void BackTopListView();		// 使得topListView回拉
	void BackRightListView();	// 使得rightListView右拉

	//editbox 事件
	std::string savefilename;
	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
	virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
	virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox);

protected:
	ui::ListView* topListView;
	ui::ListView* rightListView;

	ui::Widget** inputs;
	ui::TextField* currentSelectedTextField;
	std::string currentOrignText;  // 当前选中的TextField修改前的text

	Size visiableSize;

	b2Body* currentRightShowBody; // 当前右侧属性框显示数据源的body
	
	void initTopListView();
	void initRightListView();
	ui::Widget* addLayoutToRightListView(const std::string&, Attribute attribute, int type = 0);

	/* event */
	void shapeEvent(Ref* psender, ui::Widget::TouchEventType type);
	void saveEvent(Ref* psender, ui::Widget::TouchEventType type);
	void gravityEvent(Ref* psender, ui::Widget::TouchEventType type);
	void textFieldEvent(Ref *pSender, ui::TextField::EventType type);
	void setBodyData(Attribute attribute, float num);
	void buttonEvent(Ref* psender, ui::Widget::TouchEventType type);
	void checkBoxEvent(Ref *pSender, ui::CheckBox::EventType type);

public:
	void initRightListViewWithSelectedBody();
	/* 工具函数 */
	std::string num2str(double num){
		std::stringstream ss;
		ss << num;
		return ss.str();
	}

	template <class Type>
	Type str2num(const std::string& str)	// 使用该函数最好先用isDigit判断一下，否则对于非数字字符的转化会出问题
	{
		std::istringstream iss(str);
		Type num;
		iss >> num;
		return num;
	}

	bool isDigit(const std::string& str){		// 注意第一个为.2 和 +.2 和 -.2的字符串都返回false
		if (str.length() <= 0) return false;
		int i = 0;
		if (str[i] == '+' || str[i] == '-') i++;
		if (str.length() > 1 && str[i] == '.') return false;
		int dotCount = 0;						// 小数点的个数
		for (i; i < str.length(); i++){
			if (str[i] > '9' || str[i] < '0'){
				if (str[i] == '.') dotCount++;
				else return false;
			}
			if (dotCount > 1) return false;
		}
		return true;
	}

	bool isInt(const std::string& str){
		if (str.length() <= 0) return false;
		int i = 0;
		if (str[i] == '+' || str[i] == '-') i++;
		for (; i < str.length(); i++){
			if (str[i] > '9' || str[i] < '0'){
				return false;
			}
		}
		return true;
	}

	std::string int16Tostr0x(unsigned short num)//16bit的整数转化为16进制的str输出
	{
		const char hexbit[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
		std::string s;
		while (num != 0){
			unsigned short b = num % 16;
			num /= 16;
			s += hexbit[b];
		}
		std::string rs(s);
		for (int i = s.length() - 1; i >= 0; i--){
			rs[i] = s[s.length() - 1 - i];
		}
		return std::string("0x" + rs);
	}
};

class Box2DDesignerScene : public Layer, public b2ContactListener
{
public:
	friend class UILayer;

	static Scene* createScene();
	virtual bool init();
	virtual void onEnter();
	CREATE_FUNC(Box2DDesignerScene);
	virtual void update(float dt);
	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);

	void setting();
	void loadBackGroundIamge();

	~Box2DDesignerScene();

protected:
	/* 物理世界和绘制调试线，以及绘图的成员 */
	b2World* world;
	GLESDebugDraw* debugdraw;
	CustomCommand _customCmd;
public:
	int drawZorder;						// 该值设的越大，debug线越不会被遮住，默认为2
protected:
	void initPhysics();
	void myDraw(const Mat4 &transform, uint32_t flags);

protected:
	/* 鼠标点击事件 */
	void onMouseDown(Event* evnet);
	void onMouseMove(Event* evnet);	//不需点击的移动
	void onMouseScroll(Event* evnet);
	void onMouseUp(Event* evnet);

	Vec2 getRalativePos(Vec2 pos);		// 把相对屏幕的坐标转化为相对Layer的坐标

	Points* selectedPoints;  // 选中点在Layer在相对位置，不是相对屏幕的位置（pos)
	Vec2 currentMousePoint;				// 当前鼠标在Layer在相对位置，不是相对屏幕的位置（pos)
	Vec2 currentSelectedPoint;			// 选中点在Layer在相对位置，不是相对屏幕的位置（pos)
	Vec2 LastPoint;
	enum ButtonMouse{LEFT, RIGHT, MIDDLE, NONE};
	ButtonMouse buttonOfDownMouse;
	bool hasMoved;	// 按键过程中是否移动了
	float minMovedDistance;	//只有移动距离超过该值才认为是移动了
	float pointRange;  // 该值越大越容易认为两点是一个点
	float scaleRate;   // 滑轮滑动一个单位，世界伸缩比率
	struct ScaleRange{ float min; float max; } scalerange;
	Vec2 noScaleMovedPosition;			// 每次伸缩开始时的getPosition(),伸缩过程中不移动，伸缩后更新为getPosition(). 记住每次非伸缩移动时都要更新它
	float lastScaleAnchorRate;			// 上一个伸缩锚点（鼠标位置）的最后scale
public:
	bool disable;						// 鼠标点击事件对Designer界面失效

protected:

	/* 物理对象 */
	// 因为world->GetBodyCount()在删除物体后也会变小，所以如果直接用该数字命名，会在删除物体又新建物体后重名
	// 因此引用以下vector，被删除body的Tag，用来赋给新建的物体，tag是用来保证自动生成的各个body名称不重复的标志
	std::vector<int> deleteBodysTag;

	void addEdge(Vec2 startPoint, Vec2 endPoint);
	void addCircle(Vec2 center, float radius);
	void addChainLoop(std::vector<Vec2>* points);
	void addChainEdge(std::vector<Vec2>* points);
	void addPolygon(std::vector<Vec2>* points);
	//std::vector<b2Body*>* lineBodys;				// 线性body包括Edge, Chain; 用于删除时更容易选中，参考deleteSelectedBody

	/* 鼠标响应事件 */
	void moveWorld(Vec2 delta);    /* 左键未选中物体时拖动事件，移动世界 */
	void moveBody(Vec2 delta);		/* 左键选中物体时拖动，移动物体*/
	void rotateWorld();  /* 右键拖动事件，旋转世界 */
	void addBodyOrPoint(EventMouse *); /* 左键单击事件，根据selectedPonits添加body，或继续绘制 */
	void selectedBody(EventMouse *);/* selectedPoints为空并且选中了body时的左键单击事件，打开属性设置*/
	b2Body* getSelectedBody();			/* 获取选中的body，如果没有选中则返回nullptr */
	void cancellLastPoint(); /* selectedPoints不为空时的，右键单击事件，撤销上一个选中的点 */
	float scaleWorld(Event* e);		 /* 滚轮事件 以鼠标为中心缩放世界*/
	void deleteSelectedBody();				/* selectedPoints不为空时的，右键单击事件删除选中body */
	ShapeType getShapeType(b2Shape* body);		/* 如果不是ShpeType里的反回ShapeType::tyep_count; */ /*该函数 已使用 b2Shape的getType()代替*/
	bool isNearLine(Vec2 startpoint, Vec2 endpoint, Vec2 targetpoint); /* 判断targetpoint 是否在线（startpoint，endpoint)附近，靠近之为前面定义的的pointRange*/
	void middelMouseMoved();			/* 中键点击+移动事件*/
	void sureToCreateChainEdge();		/* 中键盘点击事件 */
public:
	b2Body* leftSelectedBody;			/* 左键选中的body，没有时为空，右键选中的body没有意义，因为选中后就删除了 */
protected:
	/* 定义选择的b2Shape类别 */
	
	/* UI 相关*/
	Size visiableSize;
	UILayer* uiLayer;

	/* xml存储 以及读取 方式 */

public:
	void saveDataToXML(std::string filepath = "");
	void saveCompleteDataToXML(std::string filepath);
	bool usingSimpleSavingModel;
	bool saving;		// 每次保存开始时为true, 每次保存后卫false
	int saveOfFrames; // 如果发生改变（changed=true) , 每隔saveOfFrames帧保存一次，
	std::string defaultSaveFilepath;
	static std::string defaultReadFilePath;
	// setLayer=true会把layer的position和scale存入，
	static void readDataToXMLInAnyModel(Layer* layer, b2World* world, std::string filepath = "", bool setLayer = false);
	static void readDataFromXML(Layer* layer, b2World* world, std::string filepath = "", bool setLayer=false); 
	static void readCompleteDataFromXML(Layer* layer, b2World* world, std::string filepath = "", bool setLayer = false);
	static void printAllDataOfBody(b2Body* body);
protected:
	static std::map<b2World*, BodyMap*> * worlds;

	tinyxml2::XMLDocument * pDoc;		// 用于存储，每次存储都会单独创建一个对象，使用后delete
	bool changed;      // 表示是否发生了改变,每次发生改变，为true， 每次保存（saveDataToXML）完成为false
	int framecount;		// 距离上次保存过了多少帧

	void xmlstoretest();
	void xmlreadtest();

	void addLayerDataToXML(tinyxml2::XMLElement* designerElement);
	void addBodyDefToXML(b2Body* body, tinyxml2::XMLElement* bodyElement);
	void addCompleteBodyDefToXML(b2Body* body, tinyxml2::XMLElement* bodyElement);
	void addFxitureDefToXML(b2Fixture* fixture, tinyxml2::XMLElement* bodyElement, b2Body* body);
	void addCompleteFxitureDefToXML(b2Fixture* fixture, tinyxml2::XMLElement* bodyElement, b2Body* body);
	void addUserDataToXML(b2Body* body, tinyxml2::XMLElement* bodyElement);

	static void readLayerDataFromXML(Layer* layer, tinyxml2::XMLElement* layerElement);
	static void readBodyDefFromXML(b2BodyDef& bodydef, tinyxml2::XMLElement* bodydefE);
	static b2MassData* readCompleteBodyDefFromXML(b2BodyDef& bodydef, tinyxml2::XMLElement* bodydefE);
	static void readShapeFromXML(b2Shape*& shape, tinyxml2::XMLElement* shapedefE);  // 传入的shape是空指针，要在函数里修改的指针本身，而不是他指向的对象（nullptr),所以要传引用
	static void readFixtureNoShapeFromXML(b2FixtureDef& fixturedef, tinyxml2::XMLElement* fixturedefE);
	static void readUserDataFromXML(BodyData* bodydata, tinyxml2::XMLElement* userdataE);

public:
	static b2Body* getBodyByName(std::string bodyname, b2World* world); // 因为可能有多个world,所以要确定是哪个world,如果不给出world，则认为是第一个world(如果存在)
																				// 没有找到，返回nullptr，注意如果某个body已在它的world中destroy，使用该函数并不会返回nullptr,
																				// 而是指向那个已释放区域的非法指针，所以在查询前，一定要确认该name对应的body未被destroy，或从未存在（返回nullptr)
protected:
	/* 工具函数 */
	std::string num2str(double num){
		std::stringstream ss;
		ss << num;
		return ss.str();
	}
	
};

#endif
