#include "Fence.h"

Fence::Fence(b2World* world, const char * filename, Vec2 pos)
{
	life = 3;
	auto sprite = Sprite::create(filename);
	this->sprite = sprite;
	sprite->setPosition(pos);
	Size size = sprite->getContentSize();
	size.width *= sprite->getScaleX();
	size.height *= sprite->getScaleY();

	pos = pos /RTM_RATIO;
	size = size / RTM_RATIO;

	b2BodyDef bodyDef;
	bodyDef.position.Set(pos.x, pos.y);
	this->body = world->CreateBody(&bodyDef);

	b2PolygonShape edge;
	edge.SetAsBox(size.width / 3, size.height / 2);
	b2FixtureDef fixtureDef;
	fixtureDef.restitution = 0.05f;
	fixtureDef.friction = 0.1f;
	fixtureDef.shape = &edge;

	b2Filter filter;
	filter.categoryBits = 0x0002;
	filter.maskBits = 0xffff;
	fixtureDef.filter = filter;
	this->body->CreateFixture(&fixtureDef);

	MyUserData* userdata = new MyUserData();
	userdata->bd_type = bd_fence;
	userdata->user = this;
	body->SetUserData(userdata);
}

Fence::Fence(b2World* world, Sprite* sprite){
	life = 3;
	if (sprite == nullptr) cocos2d::log("the sprite is nullprr in Fence(b2World* world, Sprite* sprite, Vec2 pos)");
	this->sprite = sprite;

	Size size = sprite->getContentSize();
	size.width *= sprite->getScaleX();
	size.height *= sprite->getScaleY();

	Vec2 pos = sprite->getPosition();
	pos = pos / RTM_RATIO;
	size = size / RTM_RATIO;

	float angle = -CC_DEGREES_TO_RADIANS(sprite->getRotation());

	b2BodyDef bodyDef;
	bodyDef.position.Set(pos.x, pos.y);
	this->body = world->CreateBody(&bodyDef);

	b2PolygonShape edge;
	edge.SetAsBox(size.width / 3, size.height / 2);
	b2FixtureDef fixtureDef;
	fixtureDef.restitution = 0.05f;
	fixtureDef.friction = 0.1f;
	fixtureDef.shape = &edge;

	b2Filter filter;
	filter.categoryBits = 0x0002;
	filter.maskBits = 0xffff;
	fixtureDef.filter = filter;
	this->body->CreateFixture(&fixtureDef);

	body->SetTransform(body->GetPosition(), angle);

	MyUserData* userdata = new MyUserData();
	userdata->bd_type = bd_fence;
	userdata->user = this;
	body->SetUserData(userdata);
}

void Fence::broke(){
	delete this;
}
