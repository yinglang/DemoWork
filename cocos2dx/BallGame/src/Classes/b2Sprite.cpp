#include "b2Sprite.h"

// cocos2d-x�Զ��ڴ����
/*	ԭ�����£�
/*	autorelease()�������ӵ��ڴ�������ͳһ������������ֵ��Ϊ 1
/*  ÿһ֡��ʼʱ�����й��ڵ�ǰScene���ϵĶ�������ü��������� 1
/*  ÿһ֡����ʱ���������ڴ���еĶ������������ �� 1
/*	��������ڴ���еĶ���ļ�����������ֵΪ0ʱ�����øö�������������������ö�����ڴ�����Ƴ�
/*
/*	�ֶ�������	reatian()�ǽ�������+1�� release()�ǽ�������-1
/*	 autorelease();	// ʹ����������ڴ������cocos2d-x�� ����Ҫ��b2Sprite addChild()����ӦLayer��ɾ��ֻ�����layer��remove
					// �������ʹ��cocos2d-x���Զ��ڴ�����ֶ�delete,��ע�����*/

// ��̬��Ա����
/*
std::vector<b2Sprite*>* b2Sprite::b2sprites = new std::vector<b2Sprite*>();

void b2Sprite::updateAllEnableb2SpritePosWithBody(){
	for (int i = 0; i < int(b2sprites->size()); i++){
		b2sprites->at(i)->updatePosWithBody();
	}
}

void b2Sprite::deleteEnableb2Sprites(int index){

}
*/
// �Ǿ�̬��Ա����


// �����������Զ��̳еģ��γ������ൽ����ִ�еĵ�������������
b2Sprite::~b2Sprite()
{
	if (sprite) {
		auto parent = sprite->getParent();
		if (parent)
			parent->removeChild(sprite);
		sprite = nullptr;
	}
	if (body){
		auto world = body->GetWorld();
		if (world)
			world->DestroyBody(body);
		body = nullptr;
	}
}

// �ù��캯��Ĭ�ϻ�̳У������ָ���̳еĹ��캯���Ļ����γ��Ը�����ִ�еĹ��캯������
b2Sprite::b2Sprite(){
	sprite = nullptr;
	body = nullptr;
	enableBox2d = true;

	/*needDelete = false;
	numinb2sprites = b2sprites->size();
	b2sprites->push_back(this);*/
}

b2Sprite::b2Sprite(b2World* world, const char* filename){
	Sprite* sprite = Sprite::create(filename);
	initB2Sprite(world, sprite);
	enableBox2d = true;

	/*needDelete = false;
	numinb2sprites = b2sprites->size();
	b2sprites->push_back(this);*/
}

b2Sprite::b2Sprite(b2World* world, Sprite* sprite){
	initB2Sprite(world, sprite);
	enableBox2d = true;

	/*needDelete = false;
	numinb2sprites = b2sprites->size();
	b2sprites->push_back(this);*/
}

b2Sprite::b2Sprite(b2Body* body, Sprite* sprite){
	this->body = body;
	this->sprite = sprite;
	enableBox2d = true;

	/*needDelete = false;
	numinb2sprites = b2sprites->size();
	b2sprites->push_back(this);*/
}

void b2Sprite::initB2Sprite(b2World* world, Sprite* sprite){
	if (sprite == nullptr) { cocos2d::log("the sprite is nullptr"); }
	this->sprite = sprite;
	Size size = sprite->getContentSize();
	size.width *= sprite->getScaleX();
	size.height *= sprite->getScaleY();
	Vec2 pos = sprite->getPosition();
	float angle = -CC_DEGREES_TO_RADIANS(sprite->getRotation());

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	// bodyDef.angularDamping = angularDamping;  //dumpingֵһ����0.0.-0.1֮�䣬 һ�㲻���ã���Ϊ��ʹ�����忴������Ʈ�� Ĭ����0
	// bodyDef.linearDamping = carSideLinearDumping;
	bodyDef.position.Set(pos.x / RTM_RATIO, pos.y / RTM_RATIO);
	this->body = world->CreateBody(&bodyDef);

	b2PolygonShape shapeDef;
	shapeDef.SetAsBox(size.width / 2 / RTM_RATIO, size.height / 2 / RTM_RATIO);
	b2FixtureDef fixtureDef;
	// fixtureDef.restitution = 0.05f;
	// fixtureDef.density = 0.5f;
	// fixtureDef.friction = 0.1f;
	fixtureDef.shape = &shapeDef;
	this->body->CreateFixture(&fixtureDef);
	body->SetTransform(body->GetPosition(), angle);
}

void b2Sprite::updatePosWithBody(){
	if (enableBox2d && body && sprite){
		b2Vec2 pos = body->GetPosition();
		float angle = body->GetAngle();
		sprite->setPosition(pos.x * RTM_RATIO, pos.y * RTM_RATIO);
		sprite->setRotation(-CC_RADIANS_TO_DEGREES(angle));
	}
}

Sprite* b2Sprite::getSprite(){
	return sprite;
}

b2Body* b2Sprite::getBody(){
	return body;
}

void b2Sprite::setPosition(Vec2 pos){
	sprite->setPosition(pos);
	if(body) body->SetTransform(b2Vec2(pos.x / RTM_RATIO, pos.y / RTM_RATIO), 
		-CC_DEGREES_TO_RADIANS(sprite->getRotation()));
}

void b2Sprite::setPosition(float x, float y){
	sprite->setPosition(x, y);
	if(body) body->SetTransform(b2Vec2(x / RTM_RATIO, y / RTM_RATIO),
		-CC_DEGREES_TO_RADIANS(sprite->getRotation()));
}

void b2Sprite::setBox2DEnable(bool enable){
	if (enable == enableBox2d) return;
	enableBox2d = enable;
	if (body){
		if (enable){	// false to true
			setPosition(sprite->getPosition());
			for (auto fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()){
				fixture->SetSensor(false);
			}
		}
		else{
			for (auto fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()){
				fixture->SetSensor(true);
			}
		}
	}
}

void b2Sprite::destroyBody(){
	if (body){
		auto world = body->GetWorld();
		world->DestroyBody(body);
		body = nullptr;
	}
}
