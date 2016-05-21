#include "MainMenuScene.h"
#include "Definitions.h"
#include "GameScene.h"


USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto backgroundSprite = CCSprite::create("backgrouns/menu_bg1.png");

	backgroundSprite->setAnchorPoint(Point(0, 0));

	float bgPartScale = visibleSize.height / backgroundSprite->getContentSize().height;
	
	backgroundSprite->setScale(bgPartScale);

	backgroundSprite->setPosition(origin.x, origin.y);
	
	this->addChild(backgroundSprite, 0);

	auto label = Label::createWithTTF("START", "fonts/Gamegirl.ttf", 32);
	auto exitLabel = Label::createWithTTF("EXIT", "fonts/Gamegirl.ttf", 32);

	auto menuItemLabel = MenuItemFont::create();
	auto exitMenuItemLabel = MenuItemFont::create();

	exitMenuItemLabel->setLabel(Label::createWithTTF("EXIT", "fonts/Gamegirl.ttf", 32));
	exitMenuItemLabel->setCallback(CC_CALLBACK_1(MainMenuScene::EndGame, this));

	menuItemLabel->setLabel(Label::createWithTTF("START", "fonts/Gamegirl.ttf", 32));
	menuItemLabel->setCallback(CC_CALLBACK_1(MainMenuScene::GoToGameScene, this));
	
	menuItemLabel->setPosition(Point(visibleSize.width - label->getContentSize().width / 2 - 30, visibleSize.height*0.6));
	exitMenuItemLabel->setPosition(Point(visibleSize.width - exitLabel->getContentSize().width / 2 - 30, visibleSize.height*0.4));
	Vector<MenuItem*> menuItems;

	menuItems.pushBack(menuItemLabel);
	menuItems.pushBack(exitMenuItemLabel);
	
	auto menu = Menu::createWithArray(menuItems);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 1);
    return true;
}

void MainMenuScene::EndGame(cocos2d::Ref *sender){
	Director::getInstance()->end();
}
void MainMenuScene::GoToGameScene(cocos2d::Ref *sender) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto scene = GameScene::createScene();
	auto backgroundSprite = CCSprite::create("backgrouns/menu_bg2.png");
	backgroundSprite->setAnchorPoint(Point(0, 0));
	float bgPartScale = visibleSize.height / backgroundSprite->getContentSize().height;
	backgroundSprite->setScale(bgPartScale);
	backgroundSprite->setPosition(origin.x, origin.y);

	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void MainMenuScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
