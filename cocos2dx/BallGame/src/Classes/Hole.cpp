#include "Hole.h"


Hole::Hole()
{
	type = HoleType::h_basehole_small;
	sprite = Sprite::create(BaseBall::holeimagepath[type]);
	Size size = sprite->getContentSize() * sprite->getScale();
	holesize = (size.height + size.width) / 2;
}

Hole::Hole(HoleType _type)
{
	type = _type;
	sprite = Sprite::create(BaseBall::holeimagepath[type]);
	Size size = sprite->getContentSize() * sprite->getScale();
	holesize = (size.height + size.width) / 2;
}

Hole::Hole(Sprite* _sprite, HoleType _type){
	type = _type;
	this->sprite = _sprite;
	Size size = sprite->getContentSize() * sprite->getScale();
	holesize = (size.height + size.width) / 2;
}

Hole::~Hole()
{
	if (sprite){
		auto parent = sprite->getParent();
		parent->removeChild(sprite);
		sprite = nullptr;
	}
}

void Hole::ballIn(int& lock){
	DelayTime* dl = DelayTime::create(1);
	Sequence* sq = Sequence::create(dl, CallFunc::create([&](){
		delete this;
		lock--;
	}),nullptr);
	lock++;
	sprite->runAction(sq);
}
