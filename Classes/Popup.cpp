#include "Popup.h"
#include "Definitions.h"
#include "GameScene.h"
#include "AdmobHelper.h"

USING_NS_CC;


Scene* Popup::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Popup::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}


bool Popup::init() {

	if (!Layer::init())
	{
		return false;
	}
	this->setName("PopupNode");
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto overlayRect = CCSprite::create();
	
	overlayRect->setVertexRect(Rect(visibleSize.width / 2 - visibleSize.width / 2 / 2, ((visibleSize.height / 2) - (visibleSize / 1,5 / 2)), visibleSize.width / 2, visibleSize.height / 2));
	overlayRect->setColor(Color3B::GRAY);
	overlayRect->setOpacity(30);
	
	auto rectNode = CCDrawNode::create();
	Vec2 rectangle[4];
	rectangle[0] = Vec2(origin.x, origin.y);
	rectangle[1] = Vec2(origin.x, visibleSize.height + origin.y);
	rectangle[2] = Vec2(visibleSize.width + origin.x, visibleSize.height + origin.y);
	rectangle[3] = Vec2(visibleSize.width + origin.x, 0 + origin.y);

	cocos2d::Color4F color(222, 222, 222, 0.5);

	rectNode->drawPolygon(rectangle, 4, color, 0, color);
	rectNode->setCascadeColorEnabled(true);

	this->addChild(rectNode, 1000);	
}

void Popup::GoToGameScene(cocos2d::Ref *sender) {
	CCLOG("GAME RESTART REQUESTED");
	auto scene = GameScene::createScene();
	Director::getInstance()->setNotificationNode(nullptr);
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void Popup::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
