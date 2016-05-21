#include "Column.h"
#include "Definitions.h"

USING_NS_CC;

Column::Column(){
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
}

void Column::SpawnColumn(cocos2d::Layer *layer, std::deque<cocos2d::Sprite*> *columnList, std::deque<cocos2d::Sprite*> *vaseList, int *columnsOnScreen, bool gameOverInitiated) {
	auto column = Sprite::create("col_1.png");

	auto colBody = PhysicsBody::createBox(column->getContentSize());
	colBody->setDynamic(false);
	colBody->setCollisionBitmask(COLUMN_COLLISION_BITMASK);
	colBody->setContactTestBitmask(true);
	column->setPhysicsBody(colBody);

	float colScale = visibleSize.height / 3 / column->getContentSize().height;
	column->setPosition(Point(visibleSize.width + column->getContentSize().width*colScale + origin.x, visibleSize.height/3));
	column->setScale(colScale);
	layer->addChild(column, 1);
	columnList->push_back(column);
	*columnsOnScreen = columnList->size();
	CCLOG("SIZE OF QUEUE: %i", columnList->size());
	vase.SpawnVase(layer, column->getContentSize().height * colScale, column->getContentSize().width * colScale, column->getPositionY(), colScale, vaseList, gameOverInitiated);
	auto columnAction = MoveBy::create(COL_MOVEMENT_SPEED * visibleSize.width, Point(-visibleSize.width*1.5, 0));
	column->runAction(Sequence::create(columnAction, [=]() {column->removeFromParentAndCleanup(true); }, nullptr));
	
	if (columnList->size() > 5) {
		columnList->pop_front();
	}
	column->setName("Column");
}