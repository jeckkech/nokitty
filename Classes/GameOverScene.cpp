#include "GameOverScene.h"
#include "AdmobHelper.h"

USING_NS_CC;

Scene* GameOverScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameOverScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameOverScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	CCLOG("ADMOB SHOULD START NOW");
	if (AdmobHelper::isAdShowing){
		AdmobHelper::hideAd();
		CCLOG("HIDE ADMOB");
	} else {
		AdmobHelper::showAd();
		CCLOG("SHOW ADMOB");
	}

	return true;
}


void GameOverScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
