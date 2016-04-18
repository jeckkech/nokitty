/*#include "TouchableSprite.h"
void TouchableSprite::onEnter() {
	// before 2.0:
	// CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);

	// since 2.0: 
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(this, 0, true);
	CCSprite::onEnter();
}
void TouchableSprite::onExit() {
	// before 2.0:
	// CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);

	// since 2.0:
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCSprite::onExit();
}
bool TouchableSprite::ccTouchBegan(CCTouch* touch, CCEvent* event) {
	//do whatever you want here
	return true;
}
void TouchableSprite::ccTouchMoved(CCTouch* touch, CCEvent* event) {
	//do what you want
}
void TouchableSprite::ccTouchEnded(CCTouch* touch, CCEvent* event) {
	//do your job here
}*/