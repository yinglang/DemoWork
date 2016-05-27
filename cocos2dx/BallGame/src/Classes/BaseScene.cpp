#include "BaseScene.h"
#include "SceneDesigner.h"
#include "SpriteBinder.h"
#include "GameChoose.h"

/****************************************************************************************************/
int BaseScene::scenenum = -1;
int BaseScene::lock = 0;

bool BaseScene::init(){
	if (!Layer::init()) return false; 
	macroDefineInInit();

	visibleSize = Director::getInstance()->getVisibleSize();
	/* ��ӱ߽� */
	addEdge(Vec2(0, 0), Vec2(visibleSize.width,0));
	addEdge(Vec2(0, visibleSize.height), visibleSize);
	addEdge(Vec2(0, 0),	Vec2(0, visibleSize.height));
	addEdge(Vec2(visibleSize.width, 0),	visibleSize);

	lock = 0;
	score = 1000;
	scoreLabelPos = Vec2(visibleSize) - Vec2(50, 50);
	scoreLabel = LabelTTF::create(MyTool::num2str(score), "", 40);
	scoreLabel->setPosition(scoreLabelPos);
	scoreLabel->setColor(Color3B(125,125,0));
	addChild(scoreLabel);

	/* Ϊ�����¼����� */
	for (int i = 0; i < 4; i++){
		pressed[i] = false;
	}

	auto debugger = LabelTTF::create("debugger","",25);
	debugger->setString("");
	debugger->setPosition(visibleSize.width/2, visibleSize.height - 80);
	debugger->setName("debugger");
	//addChild(debugger,4);

	//particleTest();

#ifndef NODESIGNER
	points = new std::vector<Vec2>(); 
	//SpriteBinder::readSpriteFromXML(this);
#else
	// ���عؿ�
	SceneDesigner::createScene(scenenum, this, true);
#endif

	this->scheduleUpdate();
	return true;
}

void BaseScene::update(float dt){
	if (dt <= 0) dt = Director::getInstance()->getAnimationInterval();

	// �붴���
	ballInRightHole();

	// ���̴���
	if (SceneDesigner::balls){
		for (int i = 0; i < SceneDesigner::balls->size(); i++)
			SceneDesigner::balls->at(i)->applyKeepForce(pressed);
	}

	// �ؼ�����
	score -= dt * 10;
	scoreLabel->setString(MyTool::num2str(int(score)));

	// ����������ͳ�������
	electricBallAreaEffect();

	// physics update
#ifndef NOBOX2D
	world->Step(dt, 8, 3);
#endif

	dealafterContact();

	if (SceneDesigner::balls){
		std::string s = "";
		for (int i = 0; i < int(SceneDesigner::balls->size()); i++){
			auto ball = SceneDesigner::balls->at(i);
			ball->updatePosWithBody();
			ball->getBody()->SetAngularVelocity(ball->getVelocityValue() / RTM_RATIO / 2 /ball->radius);
		
			if (score < 700){
				/*s+= "s : (" + MyTool::num2str(ball->getSprite()->getPosition().x);
				s += " ," + MyTool::num2str(ball->getSprite()->getPosition().y) + ")		";
				s += "b : (" + MyTool::num2str(ball->getBody()->GetPosition().x * 32);
				s += " ," + MyTool::num2str(ball->getBody()->GetPosition().y * 32) + ")";*/
				s += "( e : " + MyTool::num2str(int(ball->enableBox2d)) + ", s : ";
				if(ball->getSprite()) s += "y , b : ";
				else s += "n , b : ";
				if (ball->getBody()) s += "y ";
				else s += "n ";
				s += " )";
			}
		}
		if (score < 700) {
			auto debugger = ((LabelTTF*)getChildByName("debugger"));
			debugger->setString(s);
		}
	}

	if (lock == 0 && checkOver()){ // ����Ƿ�����ɽ�
		int targetScore = 0;
		if (score > targetScore) nextScene();
		else gameover();
	}

	// ��Ϸ����
	if (lock == 0 && score <= 0){
		gameover();
	}
}

Scene* BaseScene::createScene(){
	Scene* scene = Scene::create();
	BaseScene* layer = BaseScene::create();
	scene->addChild(layer);
	return scene;
}

void BaseScene::addEdge(Vec2 startpoint, Vec2 endpoint){
	startpoint = startpoint / RTM_RATIO;
	endpoint = endpoint / RTM_RATIO;
	Vec2 middlepoint = (startpoint + endpoint) / 2;
	b2BodyDef bodydef;
	bodydef.position.Set(middlepoint.x, middlepoint.y);
	auto ground = world->CreateBody(&bodydef);
	b2EdgeShape shape;
	shape.Set(b2Vec2(startpoint.x - middlepoint.x, startpoint.y - middlepoint.y),
			b2Vec2(endpoint.x - middlepoint.x, endpoint.y - middlepoint.y));

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	b2Filter filter;
	filter.categoryBits = 0x0001;
	filter.maskBits = 0xffff;
	fixtureDef.filter = filter;

	ground->CreateFixture(&fixtureDef);

	MyUserData* mytype = new MyUserData();
	mytype->bd_type = bd_ground;
	ground->SetUserData(mytype);
}

void BaseScene::ballInRightHole(){
	if (SceneDesigner::balls == nullptr || SceneDesigner::holes == nullptr) return;

	std::vector<int> removeballindex;
	std::vector<int> removeholeindex;

	for (int i = 0; i < int(SceneDesigner::balls->size()); i++){
		auto ball = SceneDesigner::balls->at(i);
		for (int j = 0; j < int(SceneDesigner::holes->size()); j++){
			auto hole = SceneDesigner::holes->at(j);
			// �������
			if (ball->getSprite()->getPosition().distance(hole->sprite->getPosition()) < (hole->holesize - ball->radius / 3 * 2) ||
				ball->getSprite()->getPosition().distance(hole->sprite->getPosition()) < hole->holesize / 2){
				
				if (int(ball->type) == int(hole->type)	// ���Զ���
					|| hole->type == HoleType::h_blackhole_small || hole->type == HoleType::h_blackhole_mid		// �ڶ�
					|| ball->type == BallType::b_doubleball_mid || ball->type == BallType::b_doubleball_small	// ˫����
					){ 
					addScore(500, hole->sprite->getPosition());
					removeballindex.push_back(i);
					removeholeindex.push_back(j);
				}
				else if (ball->type == BallType::b_eggball_mid || ball->type == BallType::b_eggball_small){		// eggBall �����ˣ�Ҳ��200��
					addScore(200, hole->sprite->getPosition());
					removeballindex.push_back(i);
					removeholeindex.push_back(j);
				}
			}
		}
	}

	SceneDesigner::removeBallAndHole(removeballindex, removeholeindex);
}

void BaseScene::addScore(int _score, Vec2 pos){
	LabelTTF* label = LabelTTF::create(MyTool::num2str(_score), "", 50);
	label->setPosition(pos);
	label->setColor(Color3B(255,0,0));
	label->setName("ASL"); // AddAcoreLabel
	addChild(label, 3);
	float dt = scoreLabelPos.distance(pos) / 200;
	dt = (dt > 0.5) ? 0.5 : dt;
	dt = (dt < 0.3) ? 0.3 : dt;
	ScaleTo* st = ScaleTo::create(dt, 0.4);
	MoveTo* mt = MoveTo::create(dt, scoreLabelPos);
	Spawn* ac = Spawn::create(st, mt, nullptr);

	/* callFunc��һ��������û���õ�������ľֲ�����,��Ȼ����������������������������������δ����ʼ����  */
	actiondata.intdata = _score;  
	//actiondata.pointdata = this;///��Ȼ�ܴ��룬����ǰһ��ûɾ��ʱ���ִ����ˣ���ǰһ���ڴ�й¶
	Sequence* allac = Sequence::create(ac, CallFuncN::create([&](Node* sender){
		((BaseScene*)(sender->getParent()))->removeChild(sender);
		score += actiondata.intdata; 
		lock--;
	}), nullptr);
	lock++;
	label->runAction(allac);
}

void BaseScene::gameover(){
	MessageBox("game over, you lost","");
	scenenum = -1;
	Director::getInstance()->replaceScene(GameChoose::createScene());
}

void BaseScene::nextScene(){
	if (scenenum < SceneDesigner::sceneCount){
		MessageBox("you pass to next scene", "Conagratulation");
		SceneDesigner::createScene(++scenenum, this);
	}
	else{
		MessageBox("you pass to all scene", "Conagratulation");
		scenenum = -1;
		Director::getInstance()->replaceScene(GameChoose::createScene());
	}
}

bool BaseScene::checkOver(){
	if (SceneDesigner::balls->empty() || SceneDesigner::holes->empty()) return true;  // ���˽���
	
	for (int i = 0; i < int(SceneDesigner::balls->size()); i++)
		if (SceneDesigner::balls->at(i)->type == BallType::b_doubleball_small ||
			SceneDesigner::balls->at(i)->type == BallType::b_doubleball_mid) return false;  // ˫�������

	for (int i = 0; i < int(SceneDesigner::holes->size()); i++)
		if (SceneDesigner::holes->at(i)->type == HoleType::h_blackhole_mid ||
			SceneDesigner::holes->at(i)->type == HoleType::h_blackhole_small) return false;  // �ڶ�����

	for (int i = 0; i < int(SceneDesigner::balls->size()); i++){		// ���ڲ�����ĸ�Ⱦ���ԣ����ܴ��ڲ����򣬵������ڲ�����
		auto ball = SceneDesigner::balls->at(i);
		if (ball->type == BallType::b_virusball_mid || ball->type == BallType::b_virusball_small){
			for (int j = 0; j < int(SceneDesigner::holes->size()); j++){
				auto hole = SceneDesigner::holes->at(j);
				if (hole->type == ball->type) return true;
			}
		}
	}
	
	return SceneDesigner::otherOverCondition();
}

BaseScene::BaseScene()
{
}

BaseScene::~BaseScene()
{
	
}

void BaseScene::ironContactFence(void* userdata){ // ����������ɺ,�ƻ���ɺ
	MyUserData* fencedata = (MyUserData*)userdata;
	if (!fencedata) return;
	auto fence = (Fence*)fencedata->user;
	if (!fence) return;

	fence->life--;
	SceneDesigner::tobebrokenFences->push_back(fence);
}

void BaseScene::virusContactBall(void* userdata){ // ���������������򣬽��������ɲ�����
	MyUserData* mydata = (MyUserData*)userdata;
	if (mydata == nullptr) return;
	// �����ǲ����򣬲�������
	if (mydata->b_type == b_virusball_mid || mydata->b_type == b_virusball_small) return;
	auto ball = (BaseBall*)(mydata->user);
	if (!ball) return;

	SceneDesigner::tobeVirusBalls->push_back(ball);
}

void BaseScene::glassContact(void* _mydataA, void* _mydataB){	 // ��������ײ������դ������������������飬remove()
	MyUserData* mydataA = (MyUserData*)_mydataA;
	MyUserData* mydataB = (MyUserData*)_mydataB;
	if (!mydataA || !mydataB) return;
	// ��ײ����ɺ����������
	if (mydataA->bd_type == bd_fence || mydataB->bd_type == bd_fence) return;
	// û������ɺ��һ��������
	if (mydataA->b_type == b_glassball_mid || mydataA->b_type == b_glassball_small){
		auto ballA = (BaseBall*)mydataA->user;
		if(ballA) SceneDesigner::removeBalls->push_back(ballA);
	}
	if (mydataB->b_type == b_glassball_mid || mydataB->b_type == b_glassball_small){
		auto ballB = (BaseBall*)mydataB->user;
		if (ballB) SceneDesigner::removeBalls->push_back(ballB);
	}
}

void BaseScene::bigCollision(Ref* bA, Ref* bB){	 // ������������ٶȺܴ���ײ����100��
	auto ballA = (BaseBall*)bA;
	auto ballB = (BaseBall*)bB;
	Vec2 direction = ballA->getSprite()->getPosition() - ballB->getSprite()->getPosition();
	if (abs(ballA->getVelocityValue(direction) - ballB->getVelocityValue(direction)) > 500){
		cocos2d::log("%f, %f", ballA->getVelocityValue(direction), ballB->getVelocityValue(direction));
		
		LabelTTF* c_word = LabelTTF::create("Big Collision", "", 40);
		c_word->setPosition((ballA->getSprite()->getPosition() + ballB->getSprite()->getPosition()) / 2);
		c_word->setName("BCW");	// Big Contact Word
		c_word->setColor(Color3B(0, 255, 0));
		FadeOut* fd = FadeOut::create(0.5);
		actiondata.pointdata = this;
		Sequence* ac = Sequence::create(fd, CallFuncN::create([&](Node* sender){
			((BaseScene*)(actiondata.pointdata))->removeChild(sender);
		}), nullptr);
		c_word->runAction(ac);
		addChild(c_word);
		addScore(100, c_word->getPosition());
	}
}

void BaseScene::dealafterContact(){
	// world->step �в�����Ҫɾ������balls����ײ����������粽�������ﲻ��ɾ��body
	if (SceneDesigner::removeBalls){
		auto removeballs = SceneDesigner::removeBalls;
		for (int i = 0; i < int(removeballs->size()); i++){
			SceneDesigner::removeBall(removeballs->at(i));
		}
		removeballs->clear();
	}

	// contanct��Ҫ���virusball��balls
	if (SceneDesigner::tobeVirusBalls){
		auto tobevirusballs = SceneDesigner::tobeVirusBalls;
		for (int i = 0; i < int(tobevirusballs->size()); i++){
			auto ball = tobevirusballs->at(i);

			BallType type = b_virusball_small;
			if ((ball->type) % 2 == 0) type = b_virusball_mid;

			auto newball = BallFactory::createBallWithOtherBallState(world, type, ball);
			addChild(newball->getSprite()); // һ��Ҫ�ҵ�scene���ϣ������ڴ治�ͷ�
			SceneDesigner::balls->push_back(newball);
			SceneDesigner::removeBall(ball);
		}
		tobevirusballs->clear();
	}

	// contact �б�ironBall ײ����դ��
	if (SceneDesigner::tobebrokenFences){
		auto tobebrokenfences = SceneDesigner::tobebrokenFences;
		for (int i = 0; i < int(tobebrokenfences->size()); i++){
			Fence* fence = tobebrokenfences->at(i);
			if (fence && fence->life <= 0)	SceneDesigner::removeFence(fence, true);
		}
		tobebrokenfences->clear();
	}
}

void BaseScene::electricBallAreaEffect(){
	auto balls = SceneDesigner::balls;
	for (int i = 0; i < int(balls->size()); i++){
		if (balls->at(i)->type == b_electricballball_small || balls->at(i)->type == b_electricball_mid){
			auto e_ball = balls->at(i);
			for (int j = 0; j < int(balls->size()); j++){
				auto ball = balls->at(j);
				if (e_ball != ball){

					float distanceSquar = e_ball->getSprite()->getPosition().distanceSquared(ball->getSprite()->getPosition());

					float forceValue = e_ball->effectRadius *RTM_RATIO * RTM_RATIO * e_ball->effectRadius / distanceSquar;

					if (forceValue >= 1){ //  ����������

						forceValue = forceValue * 0.3; // ��������Զ�������ٶ�Ϊ0.3
						// �����ķ���
						Vec2 direction = e_ball->getSprite()->getPosition() - ball->getSprite()->getPosition();
						if (direction == Vec2(0, 0)) continue;
						direction = direction / direction.distance(Vec2::ZERO);
						// �������������
						if (ball->type == b_ironball_mid || ball->type == b_ironball_small ||
							ball->type == b_electricballball_small || ball->type == b_electricball_mid){
							ball->applyForce(forceValue * direction, true);
							e_ball->applyForce(-forceValue * direction, true);
						}
						// �ų�������
						else{
							direction *= -1;
							ball->applyForce(forceValue * direction, true);
							e_ball->applyForce(-forceValue * direction, true);
						}
					}
				}
			}
		}
	}
}

void BaseScene::particleTest(){
	auto s = Director::getInstance()->getVisibleSize();

	particletype = 0; 
	/*_emitter = createParticle(particletype);
	addChild(_emitter, 10);
	//_emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("Images/stars.png"));
	_emitter->setPosition(s / 2);
	particletype = (particletype + 1) % 11;*/
	
	_emitter = createComplexParticle(0);

}

// ParticleSystemQuad : ParticleSystem : Node, (TextureProtocol :  BlendProtocol)
// ParticleBatchNode : Node, (TextureProtocol :  BlendProtocol)
ParticleSystemQuad* BaseScene::createParticle(int type){
	int i = 0;
	ParticleSystemQuad* particle = nullptr;
	if (type == i++) {
		particle = ParticleExplosion::create();
		particle->setAutoRemoveOnFinish(true);
	}
	else if (type == i++) particle = ParticleFire::create();
	else if (type == i++) particle = ParticleFireworks::create();
	else if (type == i++) particle = ParticleFlower::create();
	else if (type == i++) particle = ParticleGalaxy::create();
	else if (type == i++) particle = ParticleMeteor::create();
	else if (type == i++) particle = ParticleRain::create();
	else if (type == i++) particle = ParticleSmoke::create();
	else if (type == i++) particle = ParticleSnow::create();
	else if (type == i++) particle = ParticleSpiral::create();
	else if (type == i++) particle = ParticleSun::create();
	return particle;
}

ParticleSystemQuad* BaseScene::createComplexParticle(int type){
	if (type == 0){
		_emitter = ParticleSystemQuad::createWithTotalParticles(50);

		addChild(_emitter, 10);

		_emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("Images/stars.png"));

		_emitter->setDuration(-1);

		// gravity
		_emitter->setGravity(Vec2::ZERO);

		// angle
		_emitter->setAngle(90);
		_emitter->setAngleVar(360);

		// speed of particles
		_emitter->setSpeed(160);
		_emitter->setSpeedVar(20);

		// radial
		_emitter->setRadialAccel(-120);
		_emitter->setRadialAccelVar(0);

		// tagential
		_emitter->setTangentialAccel(30);
		_emitter->setTangentialAccelVar(0);

		// emitter position
		_emitter->setPosition(Vec2(160, 240));
		_emitter->setPosVar(Vec2::ZERO);

		// life of particles
		_emitter->setLife(4);
		_emitter->setLifeVar(1);

		// spin of particles
		_emitter->setStartSpin(0);
		_emitter->setStartSizeVar(0);
		_emitter->setEndSpin(0);
		_emitter->setEndSpinVar(0);

		// color of particles
		Color4F startColor(0.5f, 0.5f, 0.5f, 1.0f);
		_emitter->setStartColor(startColor);

		Color4F startColorVar(0.5f, 0.5f, 0.5f, 1.0f);
		_emitter->setStartColorVar(startColorVar);

		Color4F endColor(0.1f, 0.1f, 0.1f, 0.2f);
		_emitter->setEndColor(endColor);

		Color4F endColorVar(0.1f, 0.1f, 0.1f, 0.2f);
		_emitter->setEndColorVar(endColorVar);

		// size, in pixels
		_emitter->setStartSize(80.0f);
		_emitter->setStartSizeVar(40.0f);
		_emitter->setEndSize(ParticleSystem::START_SIZE_EQUAL_TO_END_SIZE);

		// emits per second
		_emitter->setEmissionRate(_emitter->getTotalParticles() / _emitter->getLife());

		// additive
		_emitter->setBlendAdditive(true);
	}
	else if (type == 1){
		_emitter = ParticleSystemQuad::createWithTotalParticles(300);
		_emitter->retain();

		addChild(_emitter, 10);
		////_emitter->release();    // win32 : Remove this line
		_emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("Images/stars.png"));

		// duration
		_emitter->setDuration(-1);

		// gravity
		_emitter->setGravity(Vec2::ZERO);

		// angle
		_emitter->setAngle(90);
		_emitter->setAngleVar(360);

		// speed of particles
		_emitter->setSpeed(160);
		_emitter->setSpeedVar(20);

		// radial
		_emitter->setRadialAccel(-120);
		_emitter->setRadialAccelVar(0);

		// tagential
		_emitter->setTangentialAccel(30);
		_emitter->setTangentialAccelVar(0);

		// emitter position
		_emitter->setPosition(Vec2(160, 240));
		_emitter->setPosVar(Vec2::ZERO);

		// life of particles
		_emitter->setLife(3);
		_emitter->setLifeVar(1);

		// spin of particles
		_emitter->setStartSpin(0);
		_emitter->setStartSpinVar(0);
		_emitter->setEndSpin(0);
		_emitter->setEndSpinVar(2000);

		// color of particles
		Color4F startColor(0.5f, 0.5f, 0.5f, 1.0f);
		_emitter->setStartColor(startColor);

		Color4F startColorVar(0.5f, 0.5f, 0.5f, 1.0f);
		_emitter->setStartColorVar(startColorVar);

		Color4F endColor(0.1f, 0.1f, 0.1f, 0.2f);
		_emitter->setEndColor(endColor);

		Color4F endColorVar(0.1f, 0.1f, 0.1f, 0.2f);
		_emitter->setEndColorVar(endColorVar);

		// size, in pixels
		_emitter->setStartSize(30.0f);
		_emitter->setStartSizeVar(00.0f);
		_emitter->setEndSize(ParticleSystem::START_SIZE_EQUAL_TO_END_SIZE);

		// emits per second
		_emitter->setEmissionRate(_emitter->getTotalParticles() / _emitter->getLife());

		// additive
		_emitter->setBlendAdditive(false);
	}

	return _emitter;
}

/****************************************************************************************************/
/* ����Ϊ�¼����������������Ҫ�õ��Ĵ����� */

#ifndef NOBOX2D1

void BaseScene::BeginContact(b2Contact* contact){
	auto bodyA = contact->GetFixtureA()->GetBody();
	auto bodyB = contact->GetFixtureB()->GetBody();
	MyUserData* mydataA = (MyUserData*)bodyA->GetUserData();
	MyUserData* mydataB = (MyUserData*)bodyB->GetUserData();

	/* ��ײ */
	if (mydataA && mydataB){
		// ���ж������Ƿ��������ײ
		if (mydataA->bd_type == bd_ball && mydataB->bd_type == bd_ball) {
			auto ballA = (BaseBall*)mydataA->user;
			auto ballB = (BaseBall*)mydataB->user;
			if (ballA && ballB){
				bigCollision(ballA,ballB);
			}
		}
		
		// �ж��Ƿ���glassball����ײ
		if (mydataA->b_type == b_glassball_mid || mydataA->b_type == b_glassball_small ||
			mydataB->b_type == b_glassball_mid || mydataB->b_type == b_glassball_small){
			glassContact(mydataA, mydataB);
		}
		// �ж��Ƿ�virusball��ײ����
		else if ((mydataA->b_type == b_virusball_mid || mydataA->b_type == b_virusball_small)
			&& (mydataB->bd_type == bd_ball)){
			virusContactBall(mydataB);
		}
		else if ((mydataB->b_type == b_virusball_mid || mydataB->b_type == b_virusball_small)
			&& (mydataA->bd_type == bd_ball)){
			virusContactBall(mydataA);
		}

		// �ж��Ƿ�ironball������ɺ
		else if ((mydataA->b_type == b_ironball_mid || mydataA->b_type == b_ironball_small)
				&& (mydataB->bd_type == bd_fence)){
			auto ball = (BaseBall*)mydataA->user;
			if (ball){
				ironContactFence(mydataB);
			}
		}
		else if ((mydataB->b_type == b_ironball_mid || mydataB->b_type == b_ironball_small)
				&& (mydataA->bd_type == bd_fence)){
			auto ball = (BaseBall*)mydataA->user;
			if (ball){
				ironContactFence(mydataA);
			}
		}
		
	}
}

void BaseScene::PreSolve(b2Contact* contact, const b2Manifold* oldManifold){
}

void BaseScene::EndContact(b2Contact* contact){

}

void BaseScene::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse){
	
}

/* ��������޸�world�ĳ�ʼ�� */
void BaseScene::initPhysics(){
	b2Vec2 gravity;
	gravity.Set(0.0f, 0.0f);
	world = new b2World(gravity);
	world->SetAllowSleeping(true);			// ���������������״̬
	world->SetContinuousPhysics(false);      // ʹ������������ײ���
	world->SetContactListener(this);			// ������ײ������

#ifndef NOBOX2DDEBUG
	// ����debug �ߵĳ�ʼ����Ϣ
	debugdraw = new GLESDebugDraw(RTM_RATIO);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	//flags += b2Draw::e_jointBit; 
	//flags += b2Draw::e_aabbBit; 
	//flags += b2Draw::e_pairBit; 
	flags += b2Draw::e_centerOfMassBit;
	debugdraw->SetFlags(flags);
	world->SetDebugDraw(debugdraw);
#endif
}

#endif

#ifndef NOKEYBOARDEVENT1
void BaseScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
	//cocos2d::log("pressed");
	switch (keyCode){
	case EventKeyboard::KeyCode::KEY_UP_ARROW:	pressed[UP] = true;	break; 

	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:	pressed[DOWN] = true;	break;

	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:	pressed[LEFT] = true;	break;

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:	pressed[DOWN] = true;	break;
	}
}

void BaseScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	cocos2d::log("released");
	switch (keyCode){
	case EventKeyboard::KeyCode::KEY_UP_ARROW:	pressed[UP] = false; break; 
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:	pressed[DOWN] = false; break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:	pressed[LEFT] = false; break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:	pressed[DOWN] = false;	break;

	// �ֻ����˼�,ֻ��releaseʱ�ᴥ����pressʱ���ᴥ��

	case EventKeyboard::KeyCode::KEY_ESCAPE:	// no break	// �ֻ����˼�
	case EventKeyboard::KeyCode::KEY_BACKSPACE: scenenum = -1; Director::getInstance()->replaceScene(GameChoose::createScene()); break;// backspace��
	}
}
#endif

#ifndef NOMOUSEEVENT
void BaseScene::onMouseDown(Event* event){
	cocos2d::log("down");	
	EventMouse * e = (EventMouse*)event;
	if(e->getMouseButton() == 0) cocos2d::log("left mouse");
	else if(e->getMouseButton() == 1) cocos2d::log("right mouse");
	else if(e->getMouseButton() == 2) cocos2d::log("middle mouse");

	/* ���ǻ�ȡ��ǰ�����꣬��e->getDelta()���Ͻ���ԭ�㣬y�����£�
	/* e->getCursorX(), e->getCursorY()���Ͻ���ԭ�㣬y������  */
}

void BaseScene::onMouseMove(Event* event){cocos2d::log("move");}
void BaseScene::onMouseScroll(Event* event){cocos2d::log("scroll");}
void BaseScene::onMouseUp(Event* event){cocos2d::log("up");}
#endif

#ifndef NOTOUCHONEBYONEEVENT1
bool BaseScene::onTouchBegan(Touch* touch, Event* event){ 
	cocos2d::log("began");

	/*removeChild(_emitter);
	_emitter = createParticle(particletype);
	addChild(_emitter, 10);
	//_emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("Images/fire.png"));
	_emitter->setPosition(visibleSize / 2);
	particletype = (particletype + 1) % 11;*/

	return true;
}//����false���¼��������´� 
void BaseScene::onTouchMoved(Touch* touch, Event* event){ 
	cocos2d::log("moved");
	//setPosition(getPosition() + touch->getDelta());
}
void BaseScene::onTouchCancelled(Touch* touch, Event* event){ cocos2d::log("cancelled"); }	// ���Ѵ���������������ϷͻȻ���˵绰
void BaseScene::onTouchEnded(Touch* touch, Event* event){ cocos2d::log("ended"); }
#endif

#ifndef NOTOUCHALLATONCEEVENT
void BaseScene::onTouchesBegan(const std::vector<Touch*>& touches, Event* event){cocos2d::log("began");}	
void BaseScene::onTouchesMoved(const std::vector<Touch*>& touches, Event* event){cocos2d::log("moved");}      // ������ƶ�
void BaseScene::onTouchesCancelled(const std::vector<Touch*>& touches, Event* event){cocos2d::log("cancelled");}  // // ���Ѵ���������������ϷͻȻ���˵绰
void BaseScene::onTouchesEnded(const std::vector<Touch*>& touches, Event* event){cocos2d::log("ended");}
#endif

#ifndef NOACCELERATIONEVENT1
void BaseScene::onAcceleration(Acceleration* acc, Event* event){
	cocos2d::log("acceleration");
	/*if (debugger) debugger->setString("x :" + MyTool::num2str(acc->x) + 
		", y : " + MyTool::num2str(acc->y) + ", z : " + MyTool::num2str(acc->z) + ",	timestamp : " + MyTool::num2str(acc->timestamp));*/

	if (SceneDesigner::balls){
		for (int i = 0; i < SceneDesigner::balls->size(); i++)
			SceneDesigner::balls->at(i)->applyForce(Vec2(10 * acc->x, 10 * acc->y));
	}
}
#endif

#ifndef NOCUSTOMEVENT
void BaseScene::onEventCustom(EventCustom*event){
	cocos2d::log("custom");
}
#endif

#ifndef NODRAW
void BaseScene::myDraw(const Mat4 &transform, uint32_t flags){
	kmGLPushMatrix();
	kmGLLoadMatrix(&transform);

	//draw
	CHECK_GL_ERROR_DEBUG();

	// draw a simple line
	// The default state is:
	// Line Width: 1
	// color: 255,255,255,255 (white, non-transparent)
	// Anti-Aliased
	//  glEnable(GL_LINE_SMOOTH);
	DrawPrimitives::drawLine(VisibleRect::leftBottom(), VisibleRect::rightTop());

	CHECK_GL_ERROR_DEBUG();

	// line: color, width, aliased
	// glLineWidth > 1 and GL_LINE_SMOOTH are not compatible
	// GL_SMOOTH_LINE_WIDTH_RANGE = (1,1) on iPhone
	//  glDisable(GL_LINE_SMOOTH);
	glLineWidth(5.0f);
	DrawPrimitives::setDrawColor4B(255, 0, 0, 255);
	DrawPrimitives::drawLine(VisibleRect::leftTop(), VisibleRect::rightBottom());

	CHECK_GL_ERROR_DEBUG();

	// TIP:
	// If you are going to use always thde same color or width, you don't
	// need to call it before every draw
	//
	// Remember: OpenGL is a state-machine.

	// draw big point in the center
	DrawPrimitives::setPointSize(64);
	DrawPrimitives::setDrawColor4B(0, 0, 255, 128);
	DrawPrimitives::drawPoint(VisibleRect::center());

	CHECK_GL_ERROR_DEBUG();

	// draw 4 small points
	Point points[] = { Point(60, 60), Point(70, 70), Point(60, 70), Point(70, 60) };
	DrawPrimitives::setPointSize(4);
	DrawPrimitives::setDrawColor4B(0, 255, 255, 255);
	DrawPrimitives::drawPoints(points, 4);

	CHECK_GL_ERROR_DEBUG();

	// draw a green circle with 10 segments
	glLineWidth(16);
	DrawPrimitives::setDrawColor4B(0, 255, 0, 255);
	DrawPrimitives::drawCircle(VisibleRect::center(), 100, 0, 10, false);

	CHECK_GL_ERROR_DEBUG();

	// draw a green circle with 50 segments with line to center
	glLineWidth(2);
	DrawPrimitives::setDrawColor4B(0, 255, 255, 255);
	DrawPrimitives::drawCircle(VisibleRect::center(), 50, CC_DEGREES_TO_RADIANS(90), 50, true);

	CHECK_GL_ERROR_DEBUG();

	// draw a pink solid circle with 50 segments
	glLineWidth(2);
	DrawPrimitives::setDrawColor4B(255, 0, 255, 255);
	DrawPrimitives::drawSolidCircle(VisibleRect::center() + Point(140, 0), 40, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f);

	CHECK_GL_ERROR_DEBUG();

	// open yellow poly
	DrawPrimitives::setDrawColor4B(255, 255, 0, 255);
	glLineWidth(10);
	Point vertices[] = { Point(0, 0), Point(50, 50), Point(100, 50), Point(100, 100), Point(50, 100) };
	DrawPrimitives::drawPoly(vertices, 5, false);

	CHECK_GL_ERROR_DEBUG();

	// filled poly
	glLineWidth(1);
	Point filledVertices[] = { Point(0, 120), Point(50, 120), Point(50, 170), Point(25, 200), Point(0, 170) };
	DrawPrimitives::drawSolidPoly(filledVertices, 5, Color4F(0.5f, 0.5f, 1, 1));


	// closed purble poly
	DrawPrimitives::setDrawColor4B(255, 0, 255, 255);
	glLineWidth(2);
	Point vertices2[] = { Point(30, 130), Point(30, 230), Point(50, 200) };
	DrawPrimitives::drawPoly(vertices2, 3, true);

	CHECK_GL_ERROR_DEBUG();

	// draw quad bezier path
	DrawPrimitives::drawQuadBezier(VisibleRect::leftTop(), VisibleRect::center(), VisibleRect::rightTop(), 50);

	CHECK_GL_ERROR_DEBUG();

	// draw cubic bezier path
	DrawPrimitives::drawCubicBezier(VisibleRect::center(), Point(VisibleRect::center().x + 30, VisibleRect::center().y + 50), Point(VisibleRect::center().x + 60, VisibleRect::center().y - 50), VisibleRect::right(), 100);

	CHECK_GL_ERROR_DEBUG();

	//draw a solid polygon
	Point vertices3[] = { Point(60, 160), Point(70, 190), Point(100, 190), Point(90, 160) };
	DrawPrimitives::drawSolidPoly(vertices3, 4, Color4F(1, 1, 0, 1));

	// restore original values
	glLineWidth(1);
	DrawPrimitives::setDrawColor4B(255, 255, 255, 255);
	DrawPrimitives::setPointSize(1);

	CHECK_GL_ERROR_DEBUG();

	//end draw
	kmGLPopMatrix();
}
#endif



/****************************************************************************************************/
/* �����ĸ�����һ������Ķ� */

void BaseScene::draw(Renderer *renderer, const Mat4& transform, uint32_t flags){
	Layer::draw(renderer, transform, flags);

#ifndef NOBOX2D
#ifndef NOBOX2DDEBUG
	world->DrawDebugData();
	/* ���Ҫ���û���ͼ��Zorder�������myDraw�У�����������_customCmd.init(_globalZOrder, transform, flags);
	�ĵ�һ��������Ĭ��Ϊ0 ���Խ��Խ����*/
#endif
#endif

#ifndef NODRAW
	_customCmd.init(_globalZOrder, transform, flags); /* ��һ�������������û�ͼ��Zorder��Ĭ��Ϊ0 ���Խ��Խ���� */
	_customCmd.func = CC_CALLBACK_0(BaseScene::myDraw, this, transform, flags);
	renderer->addCommand(&_customCmd);
	// follow code can instand of this
	// myDraw(transform, flags);
#endif
}

void BaseScene::onEnter(){
	Layer::onEnter();

#ifndef NOKEYBOARDEVENT1
	auto listenerKeyboard = EventListenerKeyboard::create();
	listenerKeyboard->onKeyPressed = CC_CALLBACK_2(BaseScene::onKeyPressed, this);
	listenerKeyboard->onKeyReleased = CC_CALLBACK_2(BaseScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerKeyboard, this);
#endif

#ifndef NOMOUSEEVENT
	auto listenerMouse = EventListenerMouse::create();
	listenerMouse->onMouseDown = CC_CALLBACK_1(BaseScene::onMouseDown, this);
	listenerMouse->onMouseMove = CC_CALLBACK_1(BaseScene::onMouseMove, this);
	listenerMouse->onMouseScroll = CC_CALLBACK_1(BaseScene::onMouseScroll, this);
	listenerMouse->onMouseUp = CC_CALLBACK_1(BaseScene::onMouseUp, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerMouse, this);
#endif

#ifndef NOTOUCHONEBYONEEVENT1
	auto listenerTouchOneByOne = EventListenerTouchOneByOne::create();
	listenerTouchOneByOne->onTouchBegan = CC_CALLBACK_2(BaseScene::onTouchBegan, this);
	listenerTouchOneByOne->onTouchMoved = CC_CALLBACK_2(BaseScene::onTouchMoved, this);
	listenerTouchOneByOne->onTouchCancelled = CC_CALLBACK_2(BaseScene::onTouchCancelled, this);
	listenerTouchOneByOne->onTouchEnded = CC_CALLBACK_2(BaseScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerTouchOneByOne, this);
#endif

#ifndef NOTOUCHALLATONCEEVENT
	auto listenerTouchAllAtOnce = EventListenerTouchAllAtOnce::create();
	listenerTouchAllAtOnce->onTouchesBegan = CC_CALLBACK_2(BaseScene::onTouchesBegan, this);
	listenerTouchAllAtOnce->onTouchesMoved = CC_CALLBACK_2(BaseScene::onTouchesMoved, this);
	listenerTouchAllAtOnce->onTouchesCancelled = CC_CALLBACK_2(BaseScene::onTouchesCancelled, this);
	listenerTouchAllAtOnce->onTouchesEnded = CC_CALLBACK_2(BaseScene::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerTouchAllAtOnce, this);
#endif

#ifndef NOACCELERATIONEVENT1
	Device::setAccelerometerEnabled(true);
	auto listenerAcceleration = EventListenerAcceleration::create(CC_CALLBACK_2(BaseScene::onAcceleration, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerAcceleration, this);
#endif

#ifndef NOCUSTOMEVENT
	auto _listener = EventListenerCustom::create("custom_event", CC_CALLBACK_1(BaseScene::onEventCustom, this));
	_eventDispatcher->addEventListenerWithFixedPriority(_listener, 1);
	EventCustom event("custom_event");
	//event.setUserData(void*);  // ������BaseScene::onEventCustom()ʹ��event.getUserData()��ǿת
	_eventDispatcher->dispatchEvent(&event);  // ��仰�������BaseScene::onEventCustom()
#endif

	setKeypadEnabled(true);

}

void BaseScene::onExit(){
	Layer::onExit();
	cocos2d::log("exit");
	_eventDispatcher->removeAllEventListeners();
}

void BaseScene::macroDefineInInit(){
#ifndef NOBOX2D1
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

#ifndef NOTOUCHONEBYONEEVENT1
	// ��δ������дҲ����
	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
#endif

#ifndef NOTOUCHALLATONCEEVENT
	// ��δ������дҲ����
	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ALL_AT_ONCE);
#endif

}
