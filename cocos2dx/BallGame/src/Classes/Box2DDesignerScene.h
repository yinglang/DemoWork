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
//ʹ��std::map<const char*, b2Body*> // ʹmap���ұ���
#define BodyMap std::map<std::string, b2Body*>

struct BodyData{
	bool ischainloop;
	std::string name;
	int tag;
	BodyData(){
		ischainloop = false;
	}
	
};  // �洢body��Ϣ��UserData

// fixture.filter
/*	һ����������ж��fixture ����maskBits��groundIndex �Ŀؼ��д��Ľ�
	categoryBits ����Ⱥ�ţ�������2���ݴΣ���������λֻ��һ��Ϊ 1���������16����Ⱥ
	maskBits     �����һ��fixture��cateroryBits�ͱ�fixture��maskBits λ������Ϊ0�� ������֮�������ײ������������ײ��
	groundIndex  ��ײ��ã�ͬһ������ײ�������ײ��ͬһ����ײ��������ײ����ײ�����ȼ�������Ⱥ	

	��ͬһ����ײ�� ����
	(fixtureA.filter.category & fixtureB.filter.maskBits) && (fixtureB.filter.category & fixtureA.filter.maskBits)Ϊtrue
	�Żᷢ����ײ
*/

enum Attribute{ a_name, a_positionX, a_positionY, a_type, a_angle, 
	a_angularDumping, a_angularVelocity, a_linearDumping, a_linearVelocityX, a_linearVelocityY, 
	a_gravityScale, a_massCenterX, a_massCenterY, a_inertia, a_mass, 
	a_isAllowSleep, a_isActive, a_isAwake, a_isBullet, a_isFixedRotation,
	a_filter_categoryBits, a_filter_groundIndex, a_filter_maskBits, a_density, a_friction,
	a_restitution, a_isSensor, a_count};  // ���е�a_filter_groundIndexӦ��a_filter_grounpIndex

class UILayer : public Layer, public cocos2d::ui::EditBoxDelegate{
public:
	CREATE_FUNC(UILayer);
	virtual bool init();

	ShapeType shapetype;
	
	// ��designer���潻�������ݣ���Box2DDesignerScene ��init�г�ʼ��
	float uiLayerHeight;
	float uiRightWidth;
	Layer* designer;	 // Ҫ��ʼ��ΪBox2DDesignerScene����Ϊcpp��һ���ļ���ֻ��ʹ������ǰ����������

	float outrange;				// ��������߽�С�ڸ�ֵʱ��listView�ſ��ܻ�out
	bool topListViewOut;		// ״̬������ʾ��ǰtopListView�Ƿ������
	bool rightListViewOut;		// ״̬������ʾ��ǰrightListView�Ƿ������
	void OutTopListView();		// ʹ��topListView����
	void OutRightListView();	// ʹ��rightListView����
	void BackTopListView();		// ʹ��topListView����
	void BackRightListView();	// ʹ��rightListView����

	//editbox �¼�
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
	std::string currentOrignText;  // ��ǰѡ�е�TextField�޸�ǰ��text

	Size visiableSize;

	b2Body* currentRightShowBody; // ��ǰ�Ҳ����Կ���ʾ����Դ��body
	
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
	/* ���ߺ��� */
	std::string num2str(double num){
		std::stringstream ss;
		ss << num;
		return ss.str();
	}

	template <class Type>
	Type str2num(const std::string& str)	// ʹ�øú����������isDigit�ж�һ�£�������ڷ������ַ���ת���������
	{
		std::istringstream iss(str);
		Type num;
		iss >> num;
		return num;
	}

	bool isDigit(const std::string& str){		// ע���һ��Ϊ.2 �� +.2 �� -.2���ַ���������false
		if (str.length() <= 0) return false;
		int i = 0;
		if (str[i] == '+' || str[i] == '-') i++;
		if (str.length() > 1 && str[i] == '.') return false;
		int dotCount = 0;						// С����ĸ���
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

	std::string int16Tostr0x(unsigned short num)//16bit������ת��Ϊ16���Ƶ�str���
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
	/* ��������ͻ��Ƶ����ߣ��Լ���ͼ�ĳ�Ա */
	b2World* world;
	GLESDebugDraw* debugdraw;
	CustomCommand _customCmd;
public:
	int drawZorder;						// ��ֵ���Խ��debug��Խ���ᱻ��ס��Ĭ��Ϊ2
protected:
	void initPhysics();
	void myDraw(const Mat4 &transform, uint32_t flags);

protected:
	/* ������¼� */
	void onMouseDown(Event* evnet);
	void onMouseMove(Event* evnet);	//���������ƶ�
	void onMouseScroll(Event* evnet);
	void onMouseUp(Event* evnet);

	Vec2 getRalativePos(Vec2 pos);		// �������Ļ������ת��Ϊ���Layer������

	Points* selectedPoints;  // ѡ�е���Layer�����λ�ã����������Ļ��λ�ã�pos)
	Vec2 currentMousePoint;				// ��ǰ�����Layer�����λ�ã����������Ļ��λ�ã�pos)
	Vec2 currentSelectedPoint;			// ѡ�е���Layer�����λ�ã����������Ļ��λ�ã�pos)
	Vec2 LastPoint;
	enum ButtonMouse{LEFT, RIGHT, MIDDLE, NONE};
	ButtonMouse buttonOfDownMouse;
	bool hasMoved;	// �����������Ƿ��ƶ���
	float minMovedDistance;	//ֻ���ƶ����볬����ֵ����Ϊ���ƶ���
	float pointRange;  // ��ֵԽ��Խ������Ϊ������һ����
	float scaleRate;   // ���ֻ���һ����λ��������������
	struct ScaleRange{ float min; float max; } scalerange;
	Vec2 noScaleMovedPosition;			// ÿ��������ʼʱ��getPosition(),���������в��ƶ������������ΪgetPosition(). ��סÿ�η������ƶ�ʱ��Ҫ������
	float lastScaleAnchorRate;			// ��һ������ê�㣨���λ�ã������scale
public:
	bool disable;						// ������¼���Designer����ʧЧ

protected:

	/* ������� */
	// ��Ϊworld->GetBodyCount()��ɾ�������Ҳ���С���������ֱ���ø���������������ɾ���������½����������
	// �����������vector����ɾ��body��Tag�����������½������壬tag��������֤�Զ����ɵĸ���body���Ʋ��ظ��ı�־
	std::vector<int> deleteBodysTag;

	void addEdge(Vec2 startPoint, Vec2 endPoint);
	void addCircle(Vec2 center, float radius);
	void addChainLoop(std::vector<Vec2>* points);
	void addChainEdge(std::vector<Vec2>* points);
	void addPolygon(std::vector<Vec2>* points);
	//std::vector<b2Body*>* lineBodys;				// ����body����Edge, Chain; ����ɾ��ʱ������ѡ�У��ο�deleteSelectedBody

	/* �����Ӧ�¼� */
	void moveWorld(Vec2 delta);    /* ���δѡ������ʱ�϶��¼����ƶ����� */
	void moveBody(Vec2 delta);		/* ���ѡ������ʱ�϶����ƶ�����*/
	void rotateWorld();  /* �Ҽ��϶��¼�����ת���� */
	void addBodyOrPoint(EventMouse *); /* ��������¼�������selectedPonits���body����������� */
	void selectedBody(EventMouse *);/* selectedPointsΪ�ղ���ѡ����bodyʱ����������¼�������������*/
	b2Body* getSelectedBody();			/* ��ȡѡ�е�body�����û��ѡ���򷵻�nullptr */
	void cancellLastPoint(); /* selectedPoints��Ϊ��ʱ�ģ��Ҽ������¼���������һ��ѡ�еĵ� */
	float scaleWorld(Event* e);		 /* �����¼� �����Ϊ������������*/
	void deleteSelectedBody();				/* selectedPoints��Ϊ��ʱ�ģ��Ҽ������¼�ɾ��ѡ��body */
	ShapeType getShapeType(b2Shape* body);		/* �������ShpeType��ķ���ShapeType::tyep_count; */ /*�ú��� ��ʹ�� b2Shape��getType()����*/
	bool isNearLine(Vec2 startpoint, Vec2 endpoint, Vec2 targetpoint); /* �ж�targetpoint �Ƿ����ߣ�startpoint��endpoint)����������֮Ϊǰ�涨��ĵ�pointRange*/
	void middelMouseMoved();			/* �м����+�ƶ��¼�*/
	void sureToCreateChainEdge();		/* �м��̵���¼� */
public:
	b2Body* leftSelectedBody;			/* ���ѡ�е�body��û��ʱΪ�գ��Ҽ�ѡ�е�bodyû�����壬��Ϊѡ�к��ɾ���� */
protected:
	/* ����ѡ���b2Shape��� */
	
	/* UI ���*/
	Size visiableSize;
	UILayer* uiLayer;

	/* xml�洢 �Լ���ȡ ��ʽ */

public:
	void saveDataToXML(std::string filepath = "");
	void saveCompleteDataToXML(std::string filepath);
	bool usingSimpleSavingModel;
	bool saving;		// ÿ�α��濪ʼʱΪtrue, ÿ�α������false
	int saveOfFrames; // ��������ı䣨changed=true) , ÿ��saveOfFrames֡����һ�Σ�
	std::string defaultSaveFilepath;
	static std::string defaultReadFilePath;
	// setLayer=true���layer��position��scale���룬
	static void readDataToXMLInAnyModel(Layer* layer, b2World* world, std::string filepath = "", bool setLayer = false);
	static void readDataFromXML(Layer* layer, b2World* world, std::string filepath = "", bool setLayer=false); 
	static void readCompleteDataFromXML(Layer* layer, b2World* world, std::string filepath = "", bool setLayer = false);
	static void printAllDataOfBody(b2Body* body);
protected:
	static std::map<b2World*, BodyMap*> * worlds;

	tinyxml2::XMLDocument * pDoc;		// ���ڴ洢��ÿ�δ洢���ᵥ������һ������ʹ�ú�delete
	bool changed;      // ��ʾ�Ƿ����˸ı�,ÿ�η����ı䣬Ϊtrue�� ÿ�α��棨saveDataToXML�����Ϊfalse
	int framecount;		// �����ϴα�����˶���֡

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
	static void readShapeFromXML(b2Shape*& shape, tinyxml2::XMLElement* shapedefE);  // �����shape�ǿ�ָ�룬Ҫ�ں������޸ĵ�ָ�뱾����������ָ��Ķ���nullptr),����Ҫ������
	static void readFixtureNoShapeFromXML(b2FixtureDef& fixturedef, tinyxml2::XMLElement* fixturedefE);
	static void readUserDataFromXML(BodyData* bodydata, tinyxml2::XMLElement* userdataE);

public:
	static b2Body* getBodyByName(std::string bodyname, b2World* world); // ��Ϊ�����ж��world,����Ҫȷ�����ĸ�world,���������world������Ϊ�ǵ�һ��world(�������)
																				// û���ҵ�������nullptr��ע�����ĳ��body��������world��destroy��ʹ�øú��������᷵��nullptr,
																				// ����ָ���Ǹ����ͷ�����ķǷ�ָ�룬�����ڲ�ѯǰ��һ��Ҫȷ�ϸ�name��Ӧ��bodyδ��destroy�����δ���ڣ�����nullptr)
protected:
	/* ���ߺ��� */
	std::string num2str(double num){
		std::stringstream ss;
		ss << num;
		return ss.str();
	}
	
};

#endif
