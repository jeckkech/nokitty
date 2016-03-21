#include "Vase.h"
#include "Definitions.h"

USING_NS_CC;

Vase::Vase() {
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
}

void Vase::SpawnVase(cocos2d::Layer *layer, float colHeight, float colWidth, float colY, float colScale) {
	int vaseId = rand()%3+1;

	char buff[11];
	snprintf(buff, sizeof(buff), "vase_%i.png", vaseId);
	std::string buffAsStdStr = buff;

	auto vase = Sprite::create(buffAsStdStr);
	auto vaseBody = PhysicsBody::createBox(vase->getContentSize());
	vaseBody->setDynamic(true);
	vase->setPhysicsBody(vaseBody);

	float vaseScale = visibleSize.height / 3 / vase->getContentSize().height / 2;
	vase->setPosition(Point(visibleSize.width + colWidth + origin.x, colY-((colHeight-vase->getContentSize().height*vaseScale)/2)+colHeight));
	vase->setScale(vaseScale);
	layer->addChild(vase, 2);
	CCLOG("SPAWN VASE %f", vase->getContentSize().width);

	auto columnAction = MoveBy::create(COL_MOVEMENT_SPEED * visibleSize.width, Point(-visibleSize.width*1.5, 0));
	vase->runAction(columnAction);
	layer->scheduleUpdate(); //??? Should it work?
}

void Vase::update(float dt) {
	CCLOG("VASE POSITION FALL: %f", this->getPosition().y);
	if (this->getPosition().y <= 200) {
		this->removeFromPhysicsWorld();
	}
}