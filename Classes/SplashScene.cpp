#include "SplashScene.h"
#include "MainMenuScene.h"
#include "Definitions.h"
#include "ui/UIVideoPlayer.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* SplashScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SplashScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	experimental::ui::VideoPlayer *videoPlayer = experimental::ui::VideoPlayer::create();
	videoPlayer->setFileName("video/intro_splash.mp4");
	this->setContentSize(Size(visibleSize.width / 2, visibleSize.height / 2));
	videoPlayer->setContentSize(Size(visibleSize.width / 2, visibleSize.height / 2));
	videoPlayer->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	videoPlayer->play();
	videoPlayer->setFullScreenEnabled(true);
	videoPlayer->setTouchEnabled(false);
	videoPlayer->addEventListener(CC_CALLBACK_2(SplashScene::videoEventCallback, this));
	this -> addChild(videoPlayer);

    return true;
}

void SplashScene::videoEventCallback(cocos2d::Ref *sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType){
	if(eventType == cocos2d::experimental::ui::VideoPlayer::EventType::COMPLETED || 
		eventType == cocos2d::experimental::ui::VideoPlayer::EventType::PAUSED){
		this->removeAllChildrenWithCleanup(true);
		auto scene = MainMenuScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
	} 
}
void SplashScene::GoToMainMenuScene(float dt) {
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void SplashScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
