#include "GameScene.h"
#include "Definitions.h"
#include "Kitty.h"
#include "Popup.h"
#include "SimpleAudioEngine.h"
#include "AdmobHelper.h"
#include "MainMenuScene.h"
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

bool GameScene::onLabelTouch(cocos2d::Touch *touch, cocos2d::Event *event) {
	Director::getInstance()->setNotificationNode(nullptr);
	this->removeChildByName("ManualSpriteNode");
	this->removeChildByName("ManualSpriteNodeAnimation");
	this->removeChildByName("TapToStartLabel");
	startListener->setEnabled(false);
	GameScene::BeginGame();
	return true;
}

void GameScene::initializeManView() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto scene = Popup::createScene();
	Director::getInstance()->setNotificationNode(scene);
	
	auto tapToStartLabel = Label::createWithTTF("TAP TO START", "fonts/Gamegirl.ttf", visibleSize.height * SCORE_FONT_SIZE);
	tapToStartLabel->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height * 0.8);
	ActionInterval *blinkLabelAction = CCBlink::create(1, 1);
	this->addChild(tapToStartLabel, 1001);
	tapToStartLabel->runAction(RepeatForever::create(blinkLabelAction));
	tapToStartLabel->setName("TapToStartLabel");
	cocos2d::SpriteBatchNode* spritebatch = SpriteBatchNode::create("common/manual/man.png");
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("common/manual/man.plist");

	auto manSprite = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName("man1.png"));

	Vector<SpriteFrame*> animFrames(4);

	for (int i = 1; i <= 4; i++) {
		char str[100] = { 0 };
		sprintf(str, "man%i.png", i);
		auto spr = Sprite::create(str);
		auto frame = cache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	float manScale = visibleSize.height / CCSprite::create("common/manual/man1.png")->getContentSize().height;
	manSprite->setName("ManualSpriteNode");
	spritebatch->addChild(manSprite);
	this->addChild(spritebatch, 1000);
	
	manSprite->setAnchorPoint(Point(0, 0));
	manSprite->setPosition(visibleSize.width / 2 + origin.x - (manSprite->getContentSize().width * manScale / 2), origin.y);
	spritebatch->setAnchorPoint(Point(0, 0));
	manSprite->setScale(manScale);
	auto animation = Animation::createWithSpriteFrames(animFrames, 1.0f);
	manSprite->runAction(RepeatForever::create(Animate::create(animation)));
	spritebatch->setName("ManualSpriteNodeAnimation");
	startListener = EventListenerTouchOneByOne::create();
	startListener->setSwallowTouches(true);
	startListener->onTouchBegan = CC_CALLBACK_2(GameScene::onLabelTouch, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(startListener, this);
}

void GameScene::BeginGame() {
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto backgroundAction1 = MoveBy::create(COL_MOVEMENT_SPEED*visibleSizeWidth, Point(-visibleSizeWidth*1.5, 0));
	auto backgroundAction2 = MoveBy::create(COL_MOVEMENT_SPEED*visibleSizeWidth, Point(-visibleSizeWidth*1.5, 0));
	auto backgroundAction3 = MoveBy::create(COL_MOVEMENT_SPEED*visibleSizeWidth, Point(-visibleSizeWidth*1.5, 0));

	backgroundSprite1->runAction(RepeatForever::create(backgroundAction1));
	backgroundSprite2->runAction(RepeatForever::create(backgroundAction2));
	backgroundSprite3->runAction(RepeatForever::create(backgroundAction3));

	auto menuLabel = CCSprite::create("buttons/menu_btn.png");
	menuLabel->setAnchorPoint(Point(0, 0));
	auto menuLabelItem = MenuItemSprite::create(menuLabel, menuLabel, menuLabel);
	//CCLOG("BUTTON SCALE %f", (visibleSize.height * SCORE_FONT_SIZE * 1.3) / menuLabel->getContentSize().height);
	float menuLabelScale = (visibleSizeHeight * SCORE_FONT_SIZE * 1.3) / menuLabel->getContentSize().height;
	menuLabel->setScale(menuLabelScale);
	//menuLabel->setContentSize(Size(menuLabel->getContentSize().width, (visibleSize.height * SCORE_FONT_SIZE * 1.3)));
//	(visibleSizeHeight - highScoreLabel->getContentSize().height) + origin.y)
	menuLabelItem->setPosition(visibleSizeWidth - (menuLabel->getContentSize().width * menuLabelScale / 2) + origin.x,
		visibleSizeHeight - (menuLabel->getContentSize().height * menuLabelScale / 2) + origin.y);

	menuLabelItem->setCallback(CC_CALLBACK_1(GameScene::MainMenuPrompt, this));
	auto menu = Menu::create(menuLabelItem, nullptr);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 1001);

	auto edgeBody = PhysicsBody::createEdgeBox(Size(visibleSizeWidth, visibleSizeHeight), PHYSICSBODY_MATERIAL_DEFAULT, 3);
	auto edgeNode = Node::create();
	edgeBody->setCollisionBitmask(FLOOR_COLLISION_BITMASK);
	edgeBody->setContactTestBitmask(true);
	edgeBody->setEnabled(false);
	edgeNode->setPosition(Point(visibleSizeWidth / 2 + origin.x, visibleSizeHeight / 2 + origin.y));
	edgeNode->setPhysicsBody(edgeBody);
	this->addChild(edgeNode);

	this->schedule(schedule_selector(GameScene::SpawnCol), COL_SPAWN_FREQUENCY * visibleSizeWidth);
	totalScore = 0;

	__String *tempScore = __String::createWithFormat("%i", totalScore);
	scoreLabel = Label::createWithTTF(tempScore->getCString(), "fonts/Gamegirl.ttf", visibleSizeHeight * SCORE_FONT_SIZE);
	scoreLabel->setPosition(visibleSizeWidth / 2 + origin.x, ((visibleSizeHeight - scoreLabel->getContentSize().height) + origin.y));
	this->addChild(scoreLabel);

	int savedHighScore = UserDefault::getInstance()->getIntegerForKey("high_score", 0);
	__String *tempHighScore = String::createWithFormat("HI %i", savedHighScore);
	auto highScoreLabel = Label::createWithTTF(tempHighScore->getCString(), "fonts/Gamegirl.ttf", visibleSizeHeight * SCORE_FONT_SIZE);
	highScoreLabel->setPosition(Point(highScoreLabel->getContentSize().width + origin.x, (visibleSizeHeight - highScoreLabel->getContentSize().height) + origin.y));
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
	impulsePower = 200;

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/kitty_jump.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/vase_put.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/vase_fall.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/vase_hit.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.5);

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

	if (UserDefault::getInstance()->getBoolForKey("game_started")) {
		GameScene::BeginGame();
	}
	else {
		GameScene::initializeManView();
	}
    return true;
}

bool GameScene::onContactBegin(cocos2d::PhysicsContact &contact) {
	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();

	if (VASE_COLLISION_BITMASK == a->getCollisionBitmask() && KITTY_COLLISION_BITMASK == b->getCollisionBitmask()) {
		a->getNode()->stopAllActions();
		a->setGravityEnable(true);
		a->setDynamic(true);
		a->getNode()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener->clone(), a->getNode());
		a->applyImpulse(Vec2(cocos2d::random(-(impulsePower), impulsePower), cocos2d::random(-150, 150)), Vec2::ZERO);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/vase_hit.wav");
	}
	else if (VASE_COLLISION_BITMASK == b->getCollisionBitmask() && KITTY_COLLISION_BITMASK == a->getCollisionBitmask()) {
		b->getNode()->stopAllActions();
		b->setGravityEnable(true);
		b->setDynamic(true);
		b->getNode()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener->clone(), b->getNode());
		b->applyImpulse(Vec2(cocos2d::random(-(impulsePower), impulsePower), cocos2d::random(-150, 150)), Vec2::ZERO);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/vase_hit.wav");
	}
	else if (VASE_COLLISION_BITMASK == a->getCollisionBitmask() && VASE_COLLISION_BITMASK == b->getCollisionBitmask()) {
		a->setGravityEnable(true);
		b->setGravityEnable(true);
		a->getNode()->stopAllActions();
		b->getNode()->stopAllActions();
		a->getNode()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener->clone(), a->getNode());
		b->getNode()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener->clone(), b->getNode());
		a->applyImpulse(Vec2(cocos2d::random(-(impulsePower), impulsePower), cocos2d::random(-150, 150)), Vec2::ZERO);
		b->applyImpulse(Vec2(cocos2d::random(-(impulsePower), impulsePower), cocos2d::random(-150, 150)), Vec2::ZERO);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/vase_hit.wav");
	} 
	else if (VASE_COLLISION_BITMASK == a->getCollisionBitmask()) {
		a->getNode()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener->clone(), a->getNode());
		//a->applyImpulse(Vec2(cocos2d::random(-(impulsePower), impulsePower), cocos2d::random(-150, 150)), Vec2::ZERO);
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/vase_hit.wav");
	}
	else if (VASE_COLLISION_BITMASK == b->getCollisionBitmask()) {
		b->getNode()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener->clone(), b->getNode());
		//b->applyImpulse(Vec2(cocos2d::random(-(impulsePower), impulsePower), cocos2d::random(-150, 150)), Vec2::ZERO);
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/vase_hit.wav");
	}
}

bool GameScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
	auto node = event->getCurrentTarget();

	if (node && !gameOverInitiated) {
		std::string nodeName = node->getName().c_str();
		if (node->getBoundingBox().containsPoint(touch->getLocation()) && nodeName.find("VaseElement") != std::string::npos) {
			movingNode = node;
			node->stopAllActions();
			node->getPhysicsBody()->resetForces();
			node->setPosition(touch->getLocation() + touch->getDelta());
		}
	}
}

bool GameScene::onTouchMove(cocos2d::Touch *touch, cocos2d::Event *event) {
	auto node = event->getCurrentTarget();
	auto body = node->getPhysicsBody();
	
	if (body && !gameOverInitiated) {
		std::string nodeName = node->getName().c_str();
		if (node->getBoundingBox().containsPoint(touch->getLocation()) && 
			nodeName.find("VaseElement") != std::string::npos && movingNode == node) {
			node->setPosition(touch->getLocation() + touch->getDelta());
		}
	}
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
				//movingNode = nullptr;
				Vec2 origin = Director::getInstance()->getVisibleOrigin();
				for (int i = 0; i < columnList.size(); i++) {
					if (columnList.at(i)->getBoundingBox().containsPoint(touch->getLocation()) && !strcmp(columnList.at(i)->getName().c_str(), "ColumnCollided")) {
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/vase_put.wav");
						Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(node);
						float colScale = visibleSizeHeight / 3 / columnList.at(i)->getContentSize().height;
						float vaseScale = visibleSizeHeight / 3 / node->getContentSize().height / 2;

						body->setGravityEnable(false);
						body->resetForces();
						body->setDynamic(false);
						body->setEnabled(false);
						body->getNode()->setRotation(0);

						float colHeight = columnList.at(i)->getContentSize().height;
					
						if (hasEnding(node->getName().c_str(), "1")) {
							vaseScale = vaseScale*1.5;
						}
						else if (hasEnding(node->getName().c_str(), "2")) {
							vaseScale = vaseScale*1.2;
						}
						
						node->setPositionX(columnList.at(i)->getPositionX() + origin.x );
						node->setPositionY(columnList.at(i)->getPositionY() - ((colHeight*colScale - node->getContentSize().height*vaseScale) / 2) + colHeight*colScale);
						body->getNode()->stopAllActions();
						
							totalScore = totalScore + 1;
							__String *tempScore = __String::createWithFormat("%i", totalScore);
							scoreLabel->setString(tempScore->getCString());

							float speedInc = (totalScore / 10 * 0.0005);
							if (totalScore % 10 == 0 && speedInc < COL_SPAWN_FREQUENCY) {
								this->unschedule(schedule_selector(GameScene::SpawnCol));
								this->schedule(schedule_selector(GameScene::SpawnCol), (COL_SPAWN_FREQUENCY - speedInc) * visibleSizeWidth);
								
								/*impulsePower = 200 * floor(totalScore / 5);
							//	COLUMN_SPEED_MULTIPLIER += 0.1;
								/*float speed = COL_SPAWN_FREQUENCY;
								#undef COL_SPAWN_FREQUENCY
								#define COL_SPAWN_FREQUENCY COL_SPAWN_FREQUENCY_CONST - totalScore / 10 * 0.0005*/
							}
						auto columnAction = MoveBy::create(COL_MOVEMENT_SPEED * visibleSizeWidth, Point(-visibleSizeWidth*1.5, 0));
						node->runAction(Sequence::create(columnAction, [=]() {node->stopAllActions(); node->removeFromParentAndCleanup(true); }, nullptr));
						break;
					}
				}
			}
	}
	
}


void GameScene::EndGame(cocos2d::Ref *sender) {
	Director::getInstance()->end();
}

void GameScene::update(float dt) {
	for (int i = 0; i < columnList.size(); i++) {
		if (columnList.at(i)->getBoundingBox().containsPoint(kitty->GetPosition())) {
			columnList.at(i)->setName("ColumnCollided");
		}
	}

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
	
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float colScale = visibleSizeHeight / 3 / 87;
	for (int i = 0; i < columnList.size(); i++) {
		float diff = columnList.at(i)->getPositionX() + origin.x  - this->kitty->GetPosition().x;
		if (diff > 0 && diff <= columnList.at(i)->getContentSize().width * colScale * 2) {
			this->KittyJump(dt);
			break;
		}
	}
	}

	for (int i = 0; i < vaseList.size(); i++) {
		if (vaseList.at(i)->getPositionY() <= 0 && !gameOverInitiated) {
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/vase_fall.wav");
			UserDefault::getInstance()->setBoolForKey("game_started", true);
			contactListener->setEnabled(false);
			touchListener->setEnabled(false);
		
			auto scene = Popup::createScene();
			UserDefault *def = UserDefault::getInstance();
			int savedHighScore = def->getIntegerForKey("high_score", 0);
			if (savedHighScore < totalScore) {
				
				auto newHighScoreLabel = Label::createWithTTF("NEW HIGH SCORE!", "fonts/Gamegirl.ttf", visibleSizeHeight * SCORE_FONT_SIZE);
				newHighScoreLabel->setPosition(Point(visibleSizeWidth / 2, visibleSizeHeight - (newHighScoreLabel->getContentSize().height * 3)));
				this->addChild(newHighScoreLabel);
				def->setIntegerForKey("high_score", totalScore);
			}

			gameOverInitiated = true;

			Director::getInstance()->setNotificationNode(scene);

			if (!AdmobHelper::isAdShowing) {
				AdmobHelper::showAd();
			}
			auto label = Label::createWithTTF("GAME OVER", "fonts/Gamegirl.ttf", visibleSizeHeight * SCORE_FONT_SIZE);
			label->setPosition(Point(visibleSizeWidth / 2, visibleSizeHeight*0.3));
			this->addChild(label, 1001);

			auto repeatLabel = CCSprite::create("buttons/restart_arr.png");
			repeatLabel->setAnchorPoint(Point(0, 0));
			auto repeatMenuItem = MenuItemSprite::create(repeatLabel, repeatLabel, repeatLabel);
			repeatLabel->setScale(visibleSizeHeight / 5 / repeatLabel->getContentSize().height);
			repeatMenuItem->setPosition(Point(visibleSizeWidth / 4, visibleSizeHeight*0.6));
			repeatMenuItem->setCallback(CC_CALLBACK_1(GameScene::RestartGame, this));

			auto exitLabel = CCSprite::create("buttons/exit_btn.png");
			auto exitLabelActive = CCSprite::create("buttons/exit_btn_active.png");

			/*exitLabel->setContentSize(Size(200, 200));
			exitLabel->setContentSize(Size(200, 200));
			*/
			exitLabel->setAnchorPoint(Point(0, 0));
			exitLabelActive->setAnchorPoint(Point(0, 0));
			auto exitMenuItem = MenuItemSprite::create(exitLabel, exitLabelActive, exitLabel);
			exitLabel->setScale(visibleSizeHeight / 5 / exitLabel->getContentSize().height);
			exitLabelActive->setScale(visibleSizeHeight / 5 / exitLabel->getContentSize().height);
			exitMenuItem->setPosition(Point(visibleSizeWidth / 4 * 3, visibleSizeHeight * 0.6));
			exitMenuItem->setCallback(CC_CALLBACK_1(GameScene::EndGame, this));

			Vector<MenuItem*> menuItems;

			menuItems.pushBack(repeatMenuItem);
			menuItems.pushBack(exitMenuItem);

			auto menu = Menu::createWithArray(menuItems);
			menu->setPosition(Point::ZERO);
			this->addChild(menu, 1001);
		}
	}
	kitty->Animate();
}

void GameScene::GoToMainMenu(cocos2d::Ref *sender) {
	auto menuScene = MainMenuScene::createScene();
	Director::getInstance()->setNotificationNode(nullptr);
	Director::getInstance()->replaceScene(menuScene);
}

void GameScene::CleanupSceneSequence(cocos2d::Ref *sender) {
	if(!gameOverInitiated){
		Director::getInstance()->setNotificationNode(nullptr);
	}
	this->removeChildByName("PromptMenu", true);
	this->removeChildByName("PromptTitle", true);
}
void GameScene::MainMenuPrompt(cocos2d::Ref *sender) {
	if (!gameOverInitiated) {
		auto scene = Popup::createScene();
		Director::getInstance()->setNotificationNode(scene);
		auto label = Label::createWithTTF("TO THE MAIN MENU?", "fonts/Gamegirl.ttf", visibleSizeHeight * SCORE_FONT_SIZE);
		label->setPosition(Point(visibleSizeWidth / 2, visibleSizeHeight*0.8));
		label->setName("PromptTitle");
		this->addChild(label, 1001);

		auto menuItemLabel = MenuItemFont::create();
		auto exitMenuItemLabel = MenuItemFont::create();

		exitMenuItemLabel->setLabel(Label::createWithTTF("YES", "fonts/Gamegirl.ttf", 32));
		exitMenuItemLabel->setCallback(CC_CALLBACK_1(GameScene::GoToMainMenu, this));

		auto menu = Menu::create();
		menuItemLabel->setLabel(Label::createWithTTF("NO", "fonts/Gamegirl.ttf", 32));
		menuItemLabel->setCallback(CC_CALLBACK_1(GameScene::CleanupSceneSequence, this));

		exitMenuItemLabel->setPosition(Point(visibleSizeWidth / 2, visibleSizeHeight*0.6));
		menuItemLabel->setPosition(Point(visibleSizeWidth / 2, visibleSizeHeight*0.4));
		Vector<MenuItem*> menuItems;

		menuItems.pushBack(menuItemLabel);
		menuItems.pushBack(exitMenuItemLabel);

		menu->setName("PromptMenu");
		menu->initWithArray(menuItems);
		menu->setPosition(Point::ZERO);
		this->addChild(menu, 1001);
	}
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

void GameScene::onExit()
{
	UserDefault::getInstance()->setBoolForKey("game_started", false);
	CCLayer::onExit();
}

void GameScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
