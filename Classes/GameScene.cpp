#include "GameScene.h"
#include "Definitions.h"
#include "Kitty.h"

USING_NS_CC;

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
	Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	auto backgroundSprite = CCSprite::create("bg_part_big.png");

	float bgPartScale = visibleSize.height / backgroundSprite->getContentSize().height;
	int bgPartCounter = ceilf(visibleSize.width / (backgroundSprite->getContentSize().width*bgPartScale)) + 1;
	for (int i = 0; i < bgPartCounter; i++) {
		auto backgroundSprite = CCSprite::create("bg_part_big.png");
		backgroundSprite->setPosition(Point(backgroundSprite->getContentSize().width * bgPartScale * i, visibleSize.height / 2 + origin.y));
		backgroundSprite->setScale(bgPartScale);
		this->addChild(backgroundSprite, 0);
		//auto backgroundAction = MoveBy::create(COL_MOVEMENT_SPEED*visibleSize.width*bgPartCounter, Point(-visibleSize.width*bgPartCounter*1.5, 0));
		//backgroundSprite->runAction(backgroundAction);
	}

	//this->scheduleOnce(schedule_selector(GameScene::SpawnBg), COL_MOVEMENT_SPEED*(backgroundSprite->getContentSize().width*bgPartScale*bgPartCounter-visibleSize.width*2));
	//this->scheduleOnce(schedule_selector(GameScene::ScheduleSpawnBg), COL_MOVEMENT_SPEED*(backgroundSprite->getContentSize().width*bgPartScale*bgPartCounter -visibleSize.width*2));

	auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	auto edgeNode = Node::create();
	edgeBody->setCollisionBitmask(FLOOR_COLLISION_BITMASK);
	edgeBody->setContactTestBitmask(true);
	edgeNode->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	edgeNode->setPhysicsBody(edgeBody);
	this->addChild(edgeNode);

	this->schedule(schedule_selector(GameScene::SpawnCol), COL_SPAWN_FREQUENCY * visibleSize.width);


	kitty = new Kitty(this);
	this->scheduleUpdate();

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
	
	touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}
bool GameScene::onContactBegin(cocos2d::PhysicsContact &contact) {
	CCLOG("HERE COMES THE CONTACT!!!!!!!!!!!!");
	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (VASE_COLLISION_BITMASK == a->getCollisionBitmask() && KITTY_COLLISION_BITMASK == b->getCollisionBitmask()) {
		a->setGravityEnable(true);
		a->getNode()->stopAllActions();
		a->applyImpulse(*new Vect(visibleSize.width, visibleSize.height), *new Vec2(visibleSize.width, visibleSize.height));
		//a->getNode()->removeFromParentAndCleanup(true);
		//a->addMoment(10);
		//a->addMass(10);
	}
	else if (VASE_COLLISION_BITMASK == b->getCollisionBitmask() && KITTY_COLLISION_BITMASK == a->getCollisionBitmask()) {
		//b->getNode()->removeFromParentAndCleanup(true);
		b->setGravityEnable(true);
		b->getNode()->stopAllActions();
		b->applyImpulse(*new Vect(visibleSize.width, visibleSize.height), *new Vec2(visibleSize.width, visibleSize.height));
		//a->addMoment(10);
		//a->addMass(10);
	}
	else if (VASE_COLLISION_BITMASK == a->getCollisionBitmask() && VASE_COLLISION_BITMASK == b->getCollisionBitmask()) {
		a->setGravityEnable(true);
		b->setGravityEnable(true);
		a->getNode()->stopAllActions();
		b->getNode()->stopAllActions();
		a->applyImpulse(*new Vect(visibleSize.width, visibleSize.height), *new Vec2(visibleSize.width, visibleSize.height));
		b->applyImpulse(*new Vect(visibleSize.width, visibleSize.height), *new Vec2(visibleSize.width, visibleSize.height));
	}
}

bool GameScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
	//event->getCurrentTarget()->removeFromParent();
	auto body = event->getCurrentTarget()->getPhysicsBody();
	if (body) {
		if (body->getNode()->getBoundingBox().containsPoint(touch->getLocation())) {
			body->getNode()->stopAllActions();
			body->getNode()->setPosition(touch->getLocation());
			//body->getNode()->removeFromParentAndCleanup(true);
		}
	}
}

void GameScene::update(float dt) {
	this->KittyJump(dt);
}

void GameScene::ScheduleSpawnBg(float dt) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto backgroundSprite = CCSprite::create("bg_part_big.png");
	float bgPartScale = visibleSize.height / backgroundSprite->getContentSize().height;
	this->schedule(schedule_selector(GameScene::SpawnBg), COL_MOVEMENT_SPEED*(backgroundSprite->getContentSize().width*bgPartScale));
}

void GameScene::SpawnCol(float dt) {
	column.SpawnColumn(this, touchListener);
	kitty->isInJump = true;
}

void GameScene::KittyJump(float dt) {
	CCLOG("JUMP CALLED");
	kitty->Jump();
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
