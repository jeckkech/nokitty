#include "GameScene.h"
#include "Definitions.h"
#include "Kitty.h"
#include <string>

USING_NS_CC;
//using namespace std;

std::string to_string(int *param)
{
	std::string str = "";
	for (str = ""; *param; *param /= 10)
		str += (char)('0' + *param % 10);
	std::reverse(str.begin(), str.end());
	return str;
}

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vect(0, -100));
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
	layer->SetPhysicsWorld(scene->getPhysicsWorld());
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	Size visibleSize = Director::getInstance()->getVisibleSize();
	visibleSizeWidth = visibleSize.width;
	visibleSizeHeight = visibleSize.height;
	Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	backgroundSprite1 = CCSprite::create("bg_part_big.png");
	backgroundSprite2 = CCSprite::create("bg_part_big.png");
	backgroundSprite3 = CCSprite::create("bg_part_big.png");
	float bgPartScale = visibleSize.height / backgroundSprite1->getContentSize().height;
	backgroundSprite1->setAnchorPoint(Point(0, 0));
	backgroundSprite2->setAnchorPoint(Point(0, 0));
	backgroundSprite3->setAnchorPoint(Point(0, 0));

	backgroundSprite1->setScale(bgPartScale);
	backgroundSprite2->setScale(bgPartScale);
	backgroundSprite3->setScale(bgPartScale);

	backgroundSprite1->setPositionX(0);
	backgroundSprite2->setPositionX(backgroundSprite1->getContentSize().width*bgPartScale);
	backgroundSprite3->setPositionX(backgroundSprite1->getContentSize().width*bgPartScale*2);

	this -> addChild(backgroundSprite1, 0);
	this -> addChild(backgroundSprite2, 0);
	this -> addChild(backgroundSprite3, 0);

	auto backgroundAction1 = MoveBy::create(COL_MOVEMENT_SPEED*visibleSize.width, Point(-visibleSize.width*1.5, 0));
	auto backgroundAction2 = MoveBy::create(COL_MOVEMENT_SPEED*visibleSize.width, Point(-visibleSize.width*1.5, 0));
	auto backgroundAction3 = MoveBy::create(COL_MOVEMENT_SPEED*visibleSize.width, Point(-visibleSize.width*1.5, 0));

	backgroundSprite1->runAction(RepeatForever::create(backgroundAction1));
	backgroundSprite2->runAction(RepeatForever::create(backgroundAction2));
	backgroundSprite3->runAction(RepeatForever::create(backgroundAction3));

	auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	auto edgeNode = Node::create();
	edgeBody->setCollisionBitmask(FLOOR_COLLISION_BITMASK);
	edgeBody->setContactTestBitmask(true);
	edgeBody->setEnable(false);
	edgeNode->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	edgeNode->setPhysicsBody(edgeBody);
	this->addChild(edgeNode);

	this->schedule(schedule_selector(GameScene::SpawnCol), COL_SPAWN_FREQUENCY * visibleSize.width);
	totalScore = 0;

	__String *tempScore = __String::createWithFormat("%i", totalScore);
	scoreLabel = Label::createWithTTF(tempScore->getCString(), "fonts/Gamegirl.ttf", visibleSize.height * SCORE_FONT_SIZE);
	scoreLabel->setPosition(visibleSizeWidth / 2 + origin.x, visibleSizeHeight - scoreLabel->getHeight());
	this->addChild(scoreLabel);

	kitty = new Kitty(this);
	this->scheduleUpdate();

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
	
	touchListener = EventListenerTouchOneByOne::create();
	
	touchListener->setSwallowTouches(false);
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMove, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchStop, this);
	//touchListener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancel, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}
bool GameScene::onContactBegin(cocos2d::PhysicsContact &contact) {
	CCLOG("HERE COMES THE CONTACT!!!!!!!!!!!!");
	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();

	if (VASE_COLLISION_BITMASK == a->getCollisionBitmask() && KITTY_COLLISION_BITMASK == b->getCollisionBitmask()) {
		a->setGravityEnable(true);
		a->setDynamic(true);
		a->getNode()->stopAllActions();
		a->applyImpulse(Vect(rand() % 100, rand() % 100));
		CCLOG("IMPULSE APPLIED");
		a->getNode()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener->clone(), a->getNode());
	}
	else if (VASE_COLLISION_BITMASK == b->getCollisionBitmask() && KITTY_COLLISION_BITMASK == a->getCollisionBitmask()) {
		b->setGravityEnable(true);
		b->setDynamic(true);
		b->getNode()->stopAllActions();
		b->applyImpulse(Vect(rand() % 200 + (-100), rand() % 300 + (-150)));
		CCLOG("IMPULSE APPLIED2");
		b->getNode()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener->clone(), b->getNode());
	}
	else if (VASE_COLLISION_BITMASK == a->getCollisionBitmask() && VASE_COLLISION_BITMASK == b->getCollisionBitmask()) {
		a->setGravityEnable(true);
		b->setGravityEnable(true);
		a->getNode()->stopAllActions();
		b->getNode()->stopAllActions();
		a->getNode()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener->clone()->clone(), a->getNode());
		b->getNode()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener->clone(), b->getNode());
	}
	else if (VASE_COLLISION_BITMASK == a->getCollisionBitmask()) {
		a->getNode()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener->clone(), a->getNode());
	}
	else if (VASE_COLLISION_BITMASK == b->getCollisionBitmask()) {
		b->getNode()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener->clone(), b->getNode());
	}
/*	if (movingNode) { //TODO comment this block if performance drop
		
		//movingNode->getPhysicsBody()->setGravityEnable(true);
		//movingNode->getPhysicsBody()->setDynamic(true);
		movingNode->getPhysicsBody()->setEnable(true);
		Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(movingNode);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener->clone(), movingNode);
		movingNode = nullptr;
	}*/
}

bool GameScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
	auto node = event->getCurrentTarget();
	CCLOG("VASE TAP");
	CCLOG("%s", node->getName().c_str());

	if (node) {
		std::string nodeName = node->getName().c_str();
		if (node->getBoundingBox().containsPoint(touch->getLocation()) && nodeName.find("VaseElement") != std::string::npos) {
			movingNode = node;
			node->getPhysicsBody()->resetForces();
			CCLOG("VASE TOUCHED");
			node->stopAllActions();
			node->setPosition(touch->getLocation() + touch->getDelta());
			/*node->setPositionX(touch->getLocation().x + node->getContentSize().width / 2);
			node->setPositionY(touch->getLocation().y + node->getContentSize().height / 2);*/
		}
	}
}

bool GameScene::onTouchMove(cocos2d::Touch *touch, cocos2d::Event *event) {
	auto node = event->getCurrentTarget();
	auto body = node->getPhysicsBody();

	if (body) {
		std::string nodeName = node->getName().c_str();
		if (node->getBoundingBox().containsPoint(touch->getLocation()) && nodeName.find("VaseElement") != std::string::npos && movingNode == node) {
			CCLOG("VASE MOVED");
			//body->setGravityEnable(false);
			//body->setDynamic(false);
			//body->setEnable(false); //TODO set to true if performance drop
			
			node->setPosition(touch->getLocation() + touch->getDelta());
			/*node->setPositionX(touch->getLocation().x + node->getContentSize().width / 2);
			node->setPositionY(touch->getLocation().y + node->getContentSize().height / 2);*/
			//movingNode = node;
		}
	}
}

bool GameScene::onTouchCancel(cocos2d::Touch *touch, cocos2d::Event *event) {
	CCLOG("TOUCH WAS CANCELLED!!!!");
}

bool hasEnding(std::string const &fullString, std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}

bool GameScene::onTouchStop(cocos2d::Touch *touch, cocos2d::Event *event) {
	
		auto node = event->getCurrentTarget();
		auto body = node->getPhysicsBody();
		if (node) {
			std::string nodeName = node->getName().c_str();
			if (node->getBoundingBox().containsPoint(touch->getLocation()) && nodeName.find("VaseElement") != std::string::npos) {
				movingNode = nullptr;
				CCLOG("VASE RELEASED");
				
			//	body->setGravityEnable(true);
			//	body->setDynamic(true);
			//	body->setEnable(true);

				//node->getEventDispatcher()->removeAllEventListeners();
				//Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener->clone(), node);
				Vec2 origin = Director::getInstance()->getVisibleOrigin();
				CCLOG("COLUMNS NUMBER0: %i", columnList.size());
				for (int i = 0; i < columnList.size(); i++) {
					if (columnList.at(i)->getBoundingBox().containsPoint(touch->getLocation())) {

						Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(node);
						float colScale = visibleSizeHeight / 3 / columnList.at(i)->getContentSize().height;
						float vaseScale = visibleSizeHeight / 3 / node->getContentSize().height / 2;

						CCLOG("IM INSIDE!");
						body->setGravityEnable(false);
						body->resetForces();
						body->setDynamic(false);
						body->getNode()->removeFromPhysicsWorld();
						body->getNode()->setRotation(0);

						

						float colHeight = columnList.at(i)->getContentSize().height;

						std::string segment;
						
						CCLOG("NODE NAME!!11");
						CCLOG("%s", node->getName().c_str());
						if (hasEnding(node->getName().c_str(), "1")) {
							CCLOG("HAS NAME 1");
							vaseScale = vaseScale*1.5;
						}
						else if (hasEnding(node->getName().c_str(), "2")) {
							CCLOG("HAS NAME 2");
							vaseScale = vaseScale*1.2;
						}
						
						node->setPositionX(columnList.at(i)->getPositionX() + origin.x );

						//colY - ((colHeight - vase->getContentSize().height*vaseScale*1.2) / 2) + colHeight)

						node->setPositionY(columnList.at(i)->getPositionY() - ((colHeight*colScale - node->getContentSize().height*vaseScale) / 2) + colHeight*colScale);
						//body->getNode()->setPositionY(columnList.at(i)->getPositionY() - (colHeight));
						body->getNode()->stopAllActions();
						
							totalScore = totalScore + 1;
							__String *tempScore = __String::createWithFormat("%i", totalScore);
							scoreLabel->setString(tempScore->getCString());

						auto columnAction = MoveBy::create(COL_MOVEMENT_SPEED * visibleSizeWidth, Point(-visibleSizeWidth*1.5, 0));
						node->runAction(columnAction);
						break;
					}
				}
			}
	}
	
}


void GameScene::update(float dt) {
	float bgSize = backgroundSprite1->getContentSize().width*backgroundSprite1->getScale();
	if (backgroundSprite1->getPositionX() <= -bgSize) {
		backgroundSprite1->setPositionX(backgroundSprite3->getPositionX() + bgSize);
	}
	else if (backgroundSprite2->getPositionX() <= -bgSize) {
		backgroundSprite2->setPositionX(backgroundSprite1->getPositionX() + bgSize);
	}
	else if (backgroundSprite3->getPositionX() <= -bgSize) {
		backgroundSprite3->setPositionX(backgroundSprite2->getPositionX() + bgSize);
	}

	for (int i = 0; i < columnList.size(); i++) {
		float diff = columnList.at(i)->getPositionX() - this->kitty->GetPosition().x;
		if (diff > 0 && diff <= columnList.at(i)->getContentSize().width * 2) {
			this->KittyJump(dt);
			break;
		}
	}

	for (int i = 0; i < vaseList.size(); i++) {
		if (vaseList.at(i)->getPositionY() <= 0) {
			CCLOG("GAME OVER!");
			auto newGame = GameScene::createScene();
			Director::getInstance()->replaceScene(newGame);
		}
	}
	kitty->Animate();
}

void GameScene::ScheduleSpawnBg(float dt) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto backgroundSprite = CCSprite::create("bg_part_big.png");
	float bgPartScale = visibleSize.height / backgroundSprite->getContentSize().height;
	this->schedule(schedule_selector(GameScene::SpawnBg), COL_MOVEMENT_SPEED*(backgroundSprite->getContentSize().width*bgPartScale));
}

void GameScene::SpawnCol(float dt) {
	column.SpawnColumn(this, &columnList, &vaseList, &columnsOnScreen);
}

void GameScene::KittyJump(float dt) {
	kitty->Jump();
	kitty->isInJump = true;
}

void GameScene::SpawnBg(float dt) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	auto backgroundSprite = CCSprite::create("bg_part_big.png");

	float bgPartScale = visibleSize.height / backgroundSprite->getContentSize().height;
	int bgPartCounter = ceilf(visibleSize.width / (backgroundSprite->getContentSize().width*bgPartScale)) + 1;
	backgroundSprite->setPosition(Point(backgroundSprite->getContentSize().width * bgPartScale * bgPartCounter, visibleSize.height / 2 + origin.y));
	backgroundSprite->setScale(bgPartScale);
	this->addChild(backgroundSprite, 0);
	auto backgroundAction = MoveBy::create(COL_MOVEMENT_SPEED*visibleSize.width*bgPartCounter, Point(-visibleSize.width*bgPartCounter*1.5, 0));
	backgroundSprite->runAction(backgroundAction);

	//this->scheduleOnce(schedule_selector(GameScene::SpawnBg), COL_MOVEMENT_SPEED*(backgroundSprite->getContentSize().width));
}

void GameScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
