#include "Vase.h"
#include "Definitions.h"

USING_NS_CC;

Vase::Vase() {
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
}
void Vase::SpawnVase(cocos2d::Layer *layer, float colHeight, float colWidth, float colY, float colScale, std::deque<cocos2d::Sprite*> *vaseList, bool gameOverInitiated) {
	int vaseId = rand()%3+1;

	char buff[20];
	snprintf(buff, sizeof(buff), "vase_%i.png", vaseId);
	std::string buffAsStdStr = buff;

	auto vase = Sprite::create(buffAsStdStr);
	auto vaseBody = PhysicsBody::createBox(vase->getContentSize(), *new PhysicsMaterial(0, 1, 0));
	vaseBody->setDynamic(!gameOverInitiated);
	vaseBody->setGravityEnable(false);
	vaseBody->setCollisionBitmask(VASE_COLLISION_BITMASK);
	vaseBody->setContactTestBitmask(true);
	
	snprintf(buff, sizeof(buff), "VaseElement_%i", vaseId);
	std::string vaseStringId = buff;
	vase->setName(vaseStringId);
	vase->setPhysicsBody(vaseBody);
	vase->setVertexRect(Rect(0, 0, vase->getContentSize().width * 3, vase->getContentSize().height * 3));

	float vaseScale = visibleSize.height / 3 / vase->getContentSize().height / 2;
	
	if(vaseId == 1){
		float vaseWidth = vase->getContentSize().width * 3;
		Rect vaseRect = *new Rect(-vaseWidth/2+vaseWidth/6, 0, vase->getContentSize().width * 3, vase->getContentSize().height);
		//vase->setContentSize(*new Size(vase->getContentSize().width * 3, vase->getContentSize().height));
		vaseBody = PhysicsBody::createBox(*new Size(vase->getContentSize().width*3, vase->getContentSize().height), *new PhysicsMaterial(0, 1, 0));
		vase->setPhysicsBody(vaseBody);
		vase->setScale(vaseScale * 1.5);
		vase->setPosition(Point(visibleSize.width + colWidth + origin.x, colY - ((colHeight - vase->getContentSize().height*vaseScale*1.5) / 2) + colHeight));
		
		vaseBody->setDynamic(!gameOverInitiated);
		vaseBody->setGravityEnable(false);
		vaseBody->setCollisionBitmask(VASE_COLLISION_BITMASK);
		vaseBody->setContactTestBitmask(true);
		//vase->setTextureRect(vaseRect);
	}
	else if (vaseId == 2) {
		vase->setPosition(Point(visibleSize.width + colWidth + origin.x, colY - ((colHeight - vase->getContentSize().height*vaseScale*1.2) / 2) + colHeight));
		vase->setScale(vaseScale*1.2);
	}
	else {
		vase->setPosition(Point(visibleSize.width + colWidth + origin.x, colY - ((colHeight - vase->getContentSize().height*vaseScale) / 2) + colHeight));
		vase->setScale(vaseScale);
	}

	layer->addChild(vase, 100);
	vaseList->push_front(vase);
	CCLOG("VASES SPAWNED: %i", vaseList->size());
	CCLOG("SPAWN VASE %f", vase->getContentSize().width);
	if (vaseList->size() > 20) {
		vaseList->pop_back();
	}

	auto columnAction = MoveBy::create(COL_MOVEMENT_SPEED * visibleSize.width, Point(-visibleSize.width*1.5, 0));
	vase->runAction(Sequence::create(columnAction, [=]() {vase->stopAllActions(); vase->removeFromParentAndCleanup(true);}, nullptr));
}