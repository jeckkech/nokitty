#include "GameScene.h"
#include "Definitions.h"
#include "Kitty.h"
#include "Popup.h"
#include "SimpleAudioEngine.h"
#include "AdmobHelper.h"
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

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/kitty_jump.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/vase_put.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/vase_fall.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/vase_hit.wav");


	if (AdmobHelper::isAdShowing) {
		AdmobHelper::hideBanner();
		CCLOG("HIDE ADMOB");
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

	backgroundSprite1->setPosition(origin.x, origin.y);
	backgroundSprite2->setPosition(backgroundSprite1->getContentSize().width*bgPartScale + origin.x, origin.y);
	backgroundSprite3->setPosition(backgroundSprite1->getContentSize().width*bgPartScale*2 + origin.x, origin.y);

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


	int savedHighScore = UserDefault::getInstance()->getIntegerForKey("high_score", 0);
	String *tempHighScore = String::createWithFormat("HI %i", savedHighScore);
	auto highScoreLabel = Label::createWithTTF(tempHighScore->getCString(), "fonts/Gamegirl.ttf", visibleSizeHeight * SCORE_FONT_SIZE);
	highScoreLabel->setPosition(Point(highScoreLabel->getContentSize().width, visibleSizeHeight - (highScoreLabel->getHeight())));
	this->addChild(highScoreLabel);

	kitty = new Kitty(this);
	this->scheduleUpdate();

	contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
	
	touchListener = EventListenerTouchOneByOne::create();
	
	touchListener->setSwallowTouches(false);
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMove, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchStop, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}
bool GameScene::onContactBegin(cocos2d::PhysicsContact &contact) {
	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();

	if (VASE_COLLISION_BITMASK == a->getCollisionBitmask() && KITTY_COLLISION_BITMASK == b->getCollisionBitmask()) {
		a->setGravityEnable(true);
		a->setDynamic(true);
		a->getNode()->stopAllActions();
		a->applyImpulse(Vect(rand() % 200 + (-100), rand() % 300 + (-150)));
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/vase_hit.wav");
		CCLOG("IMPULSE APPLIED");
		a->getNode()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener->clone(), a->getNode());
	}
	else if (VASE_COLLISION_BITMASK == b->getCollisionBitmask() && KITTY_COLLISION_BITMASK == a->getCollisionBitmask()) {
		b->setGravityEnable(true);
		b->setDynamic(true);
		b->getNode()->stopAllActions();
		b->applyImpulse(Vect(rand() % 200 + (-100), rand() % 300 + (-150)));
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/vase_hit.wav");
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
	/*else if (KITTY_COLLISION_BITMASK == a->getCollisionBitmask() && COLUMN_COLLISION_BITMASK == b->getCollisionBitmask()) {
		b->getNode()->setName("ColumnActive");
	}
	else if (KITTY_COLLISION_BITMASK == b->getCollisionBitmask() && COLUMN_COLLISION_BITMASK == a->getCollisionBitmask()) {
		a->getNode()->setName("ColumnActive");
	}*/
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

	if (node && !gameOverInitiated) {
		std::string nodeName = node->getName().c_str();
		if (node->getBoundingBox().containsPoint(touch->getLocation()) && nodeName.find("VaseElement") != std::string::npos) {
			movingNode = node;
			node->getPhysicsBody()->resetForces();
			node->stopAllActions();
			node->setPosition(touch->getLocation() + touch->getDelta());
		}
	}
}

bool GameScene::onTouchMove(cocos2d::Touch *touch, cocos2d::Event *event) {
	auto node = event->getCurrentTarget();
	auto body = node->getPhysicsBody();

	if (body && !gameOverInitiated) {
		std::string nodeName = node->getName().c_str();
		if (node->getBoundingBox().containsPoint(touch->getLocation()) && nodeName.find("VaseElement") != std::string::npos && movingNode == node) {
			node->setPosition(touch->getLocation() + touch->getDelta());
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
		if (node && !gameOverInitiated) {
			std::string nodeName = node->getName().c_str();
			if (node->getBoundingBox().containsPoint(touch->getLocation()) && nodeName.find("VaseElement") != std::string::npos) {
				movingNode = nullptr;
				Vec2 origin = Director::getInstance()->getVisibleOrigin();
				CCLOG("COLUMNS NUMBER0: %i", columnList.size());
				for (int i = 0; i < columnList.size(); i++) {
					/*std::string colName = columnList.at(i)->getName().c_str();

					CCLOG("COLUMN NAME %s", columnList.at(i)->getName().c_str());*/
					if (columnList.at(i)->getBoundingBox().containsPoint(touch->getLocation())) {
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/vase_put.wav");
						Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(node);
						float colScale = visibleSizeHeight / 3 / columnList.at(i)->getContentSize().height;
						float vaseScale = visibleSizeHeight / 3 / node->getContentSize().height / 2;

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
						node->runAction(Sequence::create(columnAction, [=]() {node->stopAllActions(); node->removeFromParentAndCleanup(true); }, nullptr));
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
	if(!gameOverInitiated){
	for (int i = 0; i < columnList.size(); i++) {
		float diff = columnList.at(i)->getPositionX() - this->kitty->GetPosition().x;
		if (diff > 0 && diff <= columnList.at(i)->getContentSize().width * 2) {
			this->KittyJump(dt);
			break;
		}
	}
	}

	for (int i = 0; i < vaseList.size(); i++) {
		if (vaseList.at(i)->getPositionY() <= 0 && !gameOverInitiated) {
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/vase_fall.wav");
			CCLOG("GAME OVER!");
			//Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
			contactListener->setEnabled(false);
			touchListener->setEnabled(false);
		
			auto scene = Popup::createScene();
			UserDefault *def = UserDefault::getInstance();
			int savedHighScore = def->getIntegerForKey("high_score", 0);
			if (savedHighScore < totalScore) {
				CCLOG("NEW HIGH SCORE!!!");
				
				auto newHighScoreLabel = Label::createWithTTF("NEW HIGH SCORE!", "fonts/Gamegirl.ttf", visibleSizeHeight * SCORE_FONT_SIZE);
				newHighScoreLabel->setPosition(Point(visibleSizeWidth / 2, visibleSizeHeight - (newHighScoreLabel->getContentSize().height * 3)));
				this->addChild(newHighScoreLabel);
				def->setIntegerForKey("high_score", totalScore);
			}
			/*
			auto label = Label::createWithTTF("GAME OVER2", "fonts/Gamegirl.ttf", 16);
			label->setPosition(Point(visibleSizeWidth / 2, visibleSizeHeight / 2));
			this->addChild(label);
			*/
			/*scoreLabel->setVisible(false);

			__String *tempScore = __String::createWithFormat("%i", totalScore);
			auto highScoreLabel = Label::createWithTTF(tempScore->getCString(), "fonts/Gamegirl.ttf", visibleSizeHeight * SCORE_FONT_SIZE);
			highScoreLabel->setPosition(Point(visibleSizeWidth / 2, visibleSizeHeight - (highScoreLabel->getContentSize().height * 2)));
			this->addChild(highScoreLabel, 1001);*/

			gameOverInitiated = true;

			Director::getInstance()->setNotificationNode(scene);
			auto repeatLabel = CCSprite::create("restart_arr.png");
			auto repeatMenuItem = MenuItemSprite::create(repeatLabel, repeatLabel, repeatLabel);
			repeatLabel->setScale(visibleSizeHeight / 5 / repeatLabel->getContentSize().height);
			repeatMenuItem->setPosition(Point(visibleSizeWidth / 4, visibleSizeHeight*0.6));
			repeatMenuItem->setCallback(CC_CALLBACK_1(GameScene::RestartGame, this));
			auto menu = Menu::create(repeatMenuItem, nullptr);
			menu->setPosition(Point::ZERO);
			this->addChild(menu, 1001);
		}
	}
	kitty->Animate();
}

void GameScene::RestartGame(cocos2d::Ref *sender) {
	gameOverInitiated = false;
	auto newGame = GameScene::createScene();
	this->pause();
	Director::getInstance()->startAnimation();
	Director::getInstance()->resume();
	Director::getInstance()->setNotificationNode(Node::create());
	Director::getInstance()->replaceScene(newGame);
	if (!AdmobHelper::isAdShowing) {
		AdmobHelper::showBanner();
	}
}

void GameScene::SpawnCol(float dt) {
	column.SpawnColumn(this, &columnList, &vaseList, &columnsOnScreen, gameOverInitiated);
}

void GameScene::KittyJump(float dt) {
	kitty->Jump();
	kitty->isInJump = true;
}

void GameScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
