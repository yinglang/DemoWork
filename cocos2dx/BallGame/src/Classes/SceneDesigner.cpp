#include "SceneDesigner.h"
#include "SpriteBinder.h"

std::vector<BaseBall*>* SceneDesigner::balls = new std::vector<BaseBall*>();
std::vector<Hole*>* SceneDesigner::holes = new std::vector<Hole*>();
std::vector<Fence*>* SceneDesigner::fences = new std::vector<Fence*>();

std::vector<BaseBall*>* SceneDesigner::removeBalls = new std::vector<BaseBall*>(); 
std::vector<BaseBall*>* SceneDesigner::tobeVirusBalls = new std::vector<BaseBall*>();
std::vector<Fence*>* SceneDesigner::tobebrokenFences = new std::vector<Fence*>();

int SceneDesigner::sceneCount = 4;

void SceneDesigner::createScene(int scenenum, GameScene* layer, bool isreplace){
	clearScene(layer, isreplace);  // 第一次进入layer时
	initSceneData(layer);
	if (scenenum <= sceneCount && scenenum > 0){
		std::string filename = "ballscene/SpriteData";
		auto sprites = SpriteBinder::readSpriteFromXML(filename + MyTool::num2str(scenenum) + ".xml", layer, 1);
		if (sprites){
			for (auto iterator = sprites->begin(); iterator != sprites->end(); iterator++){
				createb2Sprite(iterator, layer->world);
			}
		}
	}
}

void SceneDesigner::createb2Sprite(std::map<std::string, Sprite*>::iterator iterator, b2World* world){
	if (iterator->first.length() == 7 && iterator->second){
		if (iterator->first[4] == 'e'){ // fence
			fences->push_back(new Fence(world, iterator->second));
		}
		else if(iterator->first[4] == 'b'){ // ball
			BaseBall* ball = nullptr;
			switch (iterator->first[0]){
			case 'b': ball = (BallFactory::createBall(world, iterator->second, BallType::b_baseball_small)); break;
			case 'd':ball = (BallFactory::createBall(world, iterator->second, BallType::b_doubleball_small)); break;
			case 'e':{
				if (iterator->first[1] == 'g'){
					ball = (BallFactory::createBall(world, iterator->second, BallType::b_eggball_small));
				}
				else if (iterator->first[1] == 'l'){
					ball = (BallFactory::createBall(world, iterator->second, BallType::b_electricballball_small));
				}
			}
				break;
			case 'g':{
				if (iterator->first[1] == 'h'){
					ball = (BallFactory::createBall(world, iterator->second, BallType::b_ghostball_small));
				}
				else if (iterator->first[1] == 'l'){
					ball = (BallFactory::createBall(world, iterator->second, BallType::b_glassball_small));
				}
			}
				break;
			case 'i': ball = (BallFactory::createBall(world, iterator->second, BallType::b_ironball_small)); break;
			case 'u': ball = (BallFactory::createBall(world, iterator->second, BallType::b_ungravityball_small)); break;
			case 'v': ball = (BallFactory::createBall(world, iterator->second, BallType::b_virusball_small)); break;
			}
			if (ball){
				ball->getSprite()->setZOrder(2);
				balls->push_back(ball);
			}
		}
		else if (iterator->first[4] == 'h'){
			Hole* hole = nullptr;
			switch (iterator->first[0]){
			case 'b': {
				if (iterator->first[1] == 'a')
					hole = new Hole(iterator->second, HoleType::h_basehole_small); 
				else if (iterator->first[1] == 'l')
					hole = new Hole(iterator->second, HoleType::h_blackhole_small); 
			}
				break;
			case 'e':{
				if (iterator->first[1] == 'g'){
					hole = new Hole(iterator->second, HoleType::h_egghole_small);
				}
				else if (iterator->first[1] == 'l'){
					hole = new Hole(iterator->second, HoleType::h_electrichole_small);
				}
			}
				break;
			case 'g':{
				if (iterator->first[1] == 'h'){
					hole = new Hole(iterator->second, HoleType::h_ghosthole_small);
				}
				else if (iterator->first[1] == 'l'){
					hole = new Hole(iterator->second, HoleType::h_glasshole_small);
				}
			}
				break;
			case 'i': hole = new Hole(iterator->second, HoleType::h_ironhole_small); break;
			case 'u': hole = new Hole(iterator->second, HoleType::h_ungravityhole_small); break;
			case 'v': hole = new Hole(iterator->second, HoleType::h_virushole_small); break;
			}
			if (hole) {
				holes->push_back(hole);
			}
		}
	}
}

void SceneDesigner::createFirstScene(GameScene* layer){
	/* 添加小球 */
	BaseBall* ball = BallFactory::createBall(layer->world, BallType::b_electricballball_small);
	ball->setPosition(layer->visibleSize / 4);
	layer->addChild(ball->getSprite(), 2);
	balls->push_back(ball);

	BaseBall* ball2 = BallFactory::createBall(layer->world, BallType::b_ironball_small);
	ball2->setPosition(layer->visibleSize / 4 * 3);
	layer->addChild(ball2->getSprite(), 2);
	balls->push_back(ball2);

	BaseBall* ball3 = BallFactory::createBall(layer->world, BallType::b_ungravityball_small);
	ball3->setPosition(layer->visibleSize / 2);
	layer->addChild(ball3->getSprite(), 2);
	balls->push_back(ball3);

	/* 添加洞 */
	Hole* hole = new Hole(HoleType::h_basehole_small);
	hole->sprite->setPosition(layer->visibleSize / 2);
	layer->addChild(hole->sprite, 1);
	holes->push_back(hole);

	/* 添加栅栏 */
	Fence* fence = new Fence(layer->world, "fence.png", layer->visibleSize / 5);
	layer->addChild(fence->getSprite());
	fences->push_back(fence);
}

void SceneDesigner::clearScene(GameScene* layer, bool isreplace){
	if (!isreplace){
		for (auto ite = balls->begin(); ite != balls->end(); ite++){
			BaseBall* ball = *ite;
			delete ball;
		}
		for (auto ite = holes->begin(); ite != holes->end(); ite++){
			auto hole = *ite;
			delete hole;
		}
		for (auto ite = fences->begin(); ite != fences->end(); ite++){
			auto fence = *ite;
			delete fence;
		}
	}

	if (balls){
		balls->clear();
	}
	if (holes){
		holes->clear();
	}
	if (fences){
		fences->clear();
	}
	if (removeBalls){
		removeBalls->clear();
	}
	if (tobebrokenFences){
		tobebrokenFences->clear();
	}
	if (tobeVirusBalls){
		tobeVirusBalls->clear();
	}
}

void SceneDesigner::removeBallAndHole(std::vector<int> removeballindex, std::vector<int> removeholeindex){
	for (int i = 0; i < int(removeballindex.size()); i++){
		ballInHole(balls->at(removeballindex.at(i)), holes->at(removeholeindex.at(i)));
	}
	// 必须分开，因为remove会影响index
	for (int i = int(removeballindex.size()) - 1; i >= 0; i--){ // 一定要倒序，因为删除影响后面
		int index = removeballindex.at(i);
		balls->at(index) = balls->at(balls->size() - 1);
		balls->pop_back();
		
		index = removeholeindex.at(i);
		holes->at(index) = holes->at(holes->size() - 1);
		holes->pop_back();
	}
}

void SceneDesigner::ballInHole(BaseBall* ball, Hole* hole){
	ball->inHole(hole->sprite->getPosition(), BaseScene::lock);
	hole->ballIn(BaseScene::lock);
}

bool SceneDesigner::removeBall(BaseBall* ball){
	for (int i = 0; i < int(balls->size()); i++){
		if (balls->at(i) == ball){
			balls->at(i) = balls->at(balls->size() - 1);
			balls->pop_back();
			delete ball;
			return true;
		}
	}
	return false;
}

bool SceneDesigner::removeFence(Fence* fence, bool isbrokenByIronBall){
	for (int i = 0; i < int(fences->size()); i++){
		if (fences->at(i) == fence){
			fences->at(i) = fences->at(fences->size() - 1);
			fences->pop_back();
			if(isbrokenByIronBall) fence->broke();
			else delete fence;
			return true;
		}
	}
	return false;
}

void SceneDesigner::initSceneData(GameScene* layer){
	layer->score = 1000; // 100 second
	layer->lock = 0;
}

bool SceneDesigner::otherOverCondition(){
	if (GameScene::scenenum == 3){
		for (int i = 0; i < int(balls->size()); i++){
			if (balls->at(i)->type == h_ironhole_mid || balls->at(i)->type == h_ironhole_small){
				return false;
			}
		}
		return true;
	}
	else if(GameScene::scenenum == 4){
		if (balls->size() == 1 && balls->at(0) && 
			(balls->at(0)->type == BallType::b_ungravityball_mid ||
			balls->at(0)->type == BallType::b_ungravityball_small))
			return true;
	}

	return false;
}
