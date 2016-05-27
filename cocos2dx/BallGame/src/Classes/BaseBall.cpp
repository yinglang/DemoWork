#include "BaseBall.h"
#include "MyTool.h"

// 定义图片路径
#if 1
std::string* BaseBall::ballimagepath = new std::string[18]{
		"ball/baseball_mid.png",
		"ball/baseball_small.png",
		"ball/eggball_mid.png",
		"ball/eggball_small.png",
		"ball/electricball_mid.png",
		"ball/electricballball_small.png",
		"ball/ghostball_mid.png",
		"ball/ghostball_small.png",
		"ball/glassball_mid.png",
		"ball/glassball_small.png",
		"ball/ironball_mid.png",
		"ball/ironball_small.png",
		"ball/ungravityball_mid.png",
		"ball/ungravityball_small.png",
		"ball/virusball_mid.png",
		"ball/virusball_small.png",

		"ball/doubleball_mid.png",
		"ball/doubleball_small.png"
};
std::string* BaseBall::holeimagepath = new std::string[18]{
		"hole/basehole_mid.png",
		"hole/basehole_small.png",
		"hole/egghole_mid.png",
		"hole/egghole_small.png",
		"hole/electrichole_mid.png",
		"hole/electrichole_small.png",
		"hole/ghosthole_mid.png",
		"hole/ghosthole_small.png",
		"hole/glasshole_mid.png",
		"hole/glasshole_small.png",
		"hole/ironhole_mid.png",
		"hole/ironhole_small.png",
		"hole/ungravityhole_mid.png",
		"hole/ungravityhole_small.png",
		"hole/virushole_mid.png",
		"hole/virushole_small.png",

		"hole/blackhole_mid.png",
		"hole/blackhole_small.png"
};
#endif

BaseBall::BaseBall(b2World* world, const char* filename, BallType _type)
{
	type = _type;
	auto sprite = Sprite::create(filename);
	if (sprite == nullptr) { cocos2d::log("the sprite is nullptr"); }
	this->sprite = sprite;
	Size size = sprite->getContentSize();
	Vec2 pos = sprite->getPosition();

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.angularDamping = 0.05;  //dumping值一般在0.0.-0.1之间， 一般不设置，因为会使得物体看起来发飘， 默认是0
	bodyDef.linearDamping = 0.5f;
	bodyDef.position.Set(pos.x / RTM_RATIO, pos.y / RTM_RATIO);
	this->body = world->CreateBody(&bodyDef);

	b2CircleShape shapeDef;
	shapeDef.m_radius = size.width / 2 / RTM_RATIO;
	radius = shapeDef.m_radius;
	b2FixtureDef fixtureDef;
	fixtureDef.restitution = 0.9f;
	// fixtureDef.density = 0.5f;
	// fixtureDef.friction = 0.1f;
	fixtureDef.shape = &shapeDef;
	
	b2Filter filter;
	
	filter.categoryBits = 0x0004;
	filter.maskBits = 0xffff;
	fixtureDef.filter = filter;
	
	this->body->CreateFixture(&fixtureDef);

	MyUserData* mytype = new MyUserData();
	mytype->bd_type = bd_ball;
	mytype->b_type = type;
	mytype->user = this;
	body->SetUserData(mytype);

	effectRadius = radius * 8;
}

BaseBall::BaseBall(b2World* world, Sprite* sprite, BallType _type){
	type = _type;
	if (sprite == nullptr) { cocos2d::log("the sprite is nullptr"); }
	this->sprite = sprite;
	Size size = sprite->getContentSize();
	size.width *= sprite->getScaleX();
	size.height *= sprite->getScaleY();
	Vec2 pos = sprite->getPosition();
	float angle = -CC_DEGREES_TO_RADIANS(sprite->getRotation());

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.angularDamping = 0.05;  //dumping值一般在0.0.-0.1之间， 一般不设置，因为会使得物体看起来发飘， 默认是0
	bodyDef.linearDamping = 0.5f;
	bodyDef.position.Set(pos.x / RTM_RATIO, pos.y / RTM_RATIO);
	this->body = world->CreateBody(&bodyDef);

	b2CircleShape shapeDef;
	shapeDef.m_radius = size.width / 2 / RTM_RATIO;
	radius = shapeDef.m_radius;
	b2FixtureDef fixtureDef;
	fixtureDef.restitution = 0.9f;
	// fixtureDef.density = 0.5f;
	// fixtureDef.friction = 0.1f;
	fixtureDef.shape = &shapeDef;

	b2Filter filter;

	filter.categoryBits = 0x0004;
	filter.maskBits = 0xffff;
	fixtureDef.filter = filter;

	this->body->CreateFixture(&fixtureDef);

	body->SetTransform(body->GetPosition(), angle);

	MyUserData* mytype = new MyUserData();
	mytype->bd_type = bd_ball;
	mytype->b_type = type;
	mytype->user = this;
	body->SetUserData(mytype);

	effectRadius = radius * 8;
}

BaseBall::~BaseBall()
{
	
}

void BaseBall::inHole(Vec2 holePos, int& lock){
	destroyBody();

	MoveTo* mt = MoveTo::create(0.3, holePos);
	ScaleTo* st = ScaleTo::create(1, 0.1);
	// Spawn* ac = Spawn::create(fd, st, NULL);
	Sequence* allact = Sequence::create(mt, st, CallFunc::create([&]{
		delete this;
		lock--;
	}), nullptr);
	lock++;
	sprite->runAction(allact);
}

void BaseBall::applyKeepForce(bool* pressed){
	if (pressed[UP])	applyForce(Vec2(0, 5));
	if(pressed[DOWN])	applyForce(Vec2(0, -5));
	if(pressed[LEFT])	applyForce(Vec2(-5, 0));
	if(pressed[DOWN])	applyForce(Vec2(5, 0));
}

void BaseBall::applyForce(Vec2 acceleration, bool musthaveEffect){
	if (!musthaveEffect && (type == b_ungravityball_mid || type == b_ungravityball_small)) return;
	float mass = body->GetMass();
	b2Vec2 force (acceleration.x * mass, acceleration.y * mass);
	body->ApplyForce(force, body->GetPosition(),true);
}

float BaseBall::getVelocityValue(Vec2 direction){
	b2Vec2 velocity = body->GetLinearVelocity();
	if (direction != Vec2::ZERO)
	{
		float length = direction.distance(Vec2::ZERO);
		float _cos = direction.x / length;
		float _sin = direction.y / length;
		return (velocity.x * _cos + velocity.y * _sin) * RTM_RATIO;
	}
	else 
		return sqrt(velocity.x * velocity.x + velocity.y * velocity.y) * RTM_RATIO;
}

void BaseBall::onContact(){

}