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

	auto backgroundSprite = CCSprite::create("bg_part.png");

	float bgPartScale = visibleSize.height / backgroundSprite->getContentSize().height;
	int bgPartCounter = ceilf(visibleSize.width / (backgroundSprite->getContentSize().width*bgPartScale))+1;
	for (int i = 0; i < bgPartCounter; i++){
		auto backgroundSprite = CCSprite::create("bg_part.png");
		backgroundSprite->setPosition(Point(backgroundSprite->getContentSize().width * bgPartScale * i, visibleSize.height / 2 + origin.y));
		backgroundSprite->setScale(visibleSize.height / backgroundSprite->getContentSize().height);
		this->addChild(backgroundSprite, 0);
	}

	auto label = Label::createWithTTF("TAP TO START", "fonts/Gamegirl.ttf", 32);
	
	auto menuItemLabel = MenuItemFont::create();
	menuItemLabel->setLabel(Label::createWithTTF("TAP TO START", "fonts/Gamegirl.ttf", 32));
	menuItemLabel->setCallback(CC_CALLBACK_1(MainMenuScene::GoToGameScene, this));
	menuItemLabel->setPosition(Point(visibleSize.width / 2, visibleSize.height*0.6));
	
	auto menu = Menu::create(menuItemLabel, nullptr);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 1);
    return true;
}

void MainMenuScene::GoToGameScene(cocos2d::Ref *sender) {
	auto scene = GameScene::createScene();

	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void MainMenuScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
