#include "b2Sprite.h"

// cocos2d-x自动内存管理
/*	原理如下：
/*	autorelease()，是他加到内存管理池中统一管理，并将计数值设为 1
/*  每一帧开始时，所有挂在当前Scene树上的对象的引用计数器都加 1
/*  每一帧结束时，所有在内存池中的对象的引用数都 减 1
/*	检查所有内存池中的对象的计数器，当该值为0时，调用该对象的析构函数，并将该对象从内存池中移除
/*
/*	手动操作：	reatian()是将计数器+1， release()是将计数器-1
/*	 autorelease();	// 使用这个，把内存管理交给cocos2d-x， 不过要将b2Sprite addChild()进相应Layer，删除只需调用layer的remove
					// 如果不想使用cocos2d-x的自动内存管理，手动delete,请注释这句*/

// 静态成员部分
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
// 非静态成员部分


// 析构函数是自动继承的，形成自子类到父类执行的的析构函数链子
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

// 该构造函数默认会继承，如果不指定继承的构造函数的话，形成自父到子执行的构造函数链子
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
	// bodyDef.angularDamping = angularDamping;  //dumping值一般在0.0.-0.1之间， 一般不设置，因为会使得物体看起来发飘， 默认是0
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
