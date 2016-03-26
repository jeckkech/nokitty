#include "Vase.h"
#include "Definitions.h"

USING_NS_CC;

Vase::Vase() {
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
}
void Vase::SpawnVase(cocos2d::Layer *layer, cocos2d::EventListenerTouchOneByOne *touchListener, float colHeight, float colWidth, float colY, float colScale) {
	int vaseId = rand()%3+1;

	char buff[11];
	snprintf(buff, sizeof(buff), "vase_%i.png", vaseId);
	std::string buffAsStdStr = buff;

	auto vase = Sprite::create(buffAsStdStr);
	auto vaseBody = PhysicsBody::createBox(vase->getContentSize(), *new PhysicsMaterial(10, 10, 10));
	vaseBody->setDynamic(true);
	vaseBody->setGravityEnable(false);
	vaseBody->setCollisionBitmask(VASE_COLLISION_BITMASK);
	vaseBody->setContactTestBitmask(true);
	vase->setName("VaseElement");
	
	vase->setPhysicsBody(vaseBody);

	float vaseScale = visibleSize.height / 3 / vase->getContentSize().height / 2;
	vase->setPosition(Point(visibleSize.width + colWidth + origin.x, colY-((colHeight-vase->getContentSize().height*vaseScale)/2)+colHeight));
	vase->setScale(vaseScale);
	layer->addChild(vase, 2);
	CCLOG("SPAWN VASE %f", vase->getContentSize().width);

	auto columnAction = MoveBy::create(COL_MOVEMENT_SPEED * visibleSize.width, Point(-visibleSize.width*1.5, 0));
	vase->runAction(Sequence::create(columnAction, RemoveSelf::create(true), nullptr));	


	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, vase);
}